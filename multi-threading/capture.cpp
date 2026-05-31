#include "capture.h"
#include "core/logger/logger.h"

#define QUEUE_CAP 8

const std::string LOG_PATH = "/var/log/packets_flow_rate_tracker/packets_flow_rate_tracker.csv";

void packet_handler(u_char *args,
                    const struct pcap_pkthdr *header,
                    const u_char *packet){
	std::atomic_fetch_add(&total_packets, 1);
}

void queue_init(Queue *q) {
    q->head = q->tail = q->count = 0;
    pthread_mutex_init(&q->lock, NULL);
    pthread_cond_init(&q->not_empty, NULL);
    pthread_cond_init(&q->not_full, NULL);
}

void queue_push(Queue *q, int val) {
	//mutex(Mutual Exclusion) lock create a danger zone, at one time it just allow 1 thread go inside, to avoid race condition (Multiple tasks simultaneously access and modify shared data).
    pthread_mutex_lock(&q->lock);

    // If full, blocking a open locking, waiting for customer take away
    while (q->count == QUEUE_CAP)
        pthread_cond_wait(&q->not_full, &q->lock);
	
    q->data[q->tail] = val;

	// protect the queue not overflow
    q->tail = (q->tail + 1) % QUEUE_CAP;
    q->count++;

    pthread_cond_signal(&q->not_empty);  // announcement new data has came
    pthread_mutex_unlock(&q->lock); 
}

int queue_pop(Queue *q) {
    pthread_mutex_lock(&q->lock);

    // if empty, block and wait for the manufacturer to add it. 
    while (q->count == 0)
        pthread_cond_wait(&q->not_empty, &q->lock);

    int val = q->data[q->head];
    q->head = (q->head + 1) % QUEUE_CAP;
    q->count--;

    pthread_cond_signal(&q->not_full);  // announce for producer is available spots
    pthread_mutex_unlock(&q->lock);
    return val;
}

//the duty of producer is put the packets from NIC to the queue.
//Then the cosumer take the packets from the queue to handle.
//The reason why needing to split 2 parts like that because each function handles a different task on the same "chain". While one function is working, another function continues with a different task to ensure the "chain" doesn't break. (It's like a delivery person bringing food to a counter, then the chef picks it up to cook. If the chef does both tasks, the serving of the dish will be delayed).
void *capture_thread(void *arg) {
	struct bpf_program fp;
	capture_args_t *cargs = (capture_args_t*)arg;
    	
	char filter_expression[10000];

	//tcpflags is located in the 13th byte of the TCP header (counting from 0).
	snprintf(filter_expression, sizeof(filter_expression), "((tcp[13] != 0) or udp or icmp) and (not src host %s)", cargs->ipv4_dev);

	//get the network address of interface 
	if (pcap_lookupnet(cargs->dev_name, &cargs->net, &cargs->mask, cargs->errbuf) == -1) {
    	printf("Cannot get netmask for the device\n");
		return NULL;        
	}

	// Open interface in promiscuous mode
    pcap_t *handle = pcap_open_live(
        cargs->dev_name,
        BUFSIZ,   // snaplen
        1,        // promiscuous
        100,      // timeout ms
        cargs->errbuf
    );
    if (!handle) {
        fprintf(stderr, "pcap_open_live: %s\n", cargs->errbuf);
        return NULL;
    }

    // Compile & apply filter
    if (pcap_compile(handle, &fp, filter_expression, 0, cargs->net) == -1) {
        fprintf(stderr, "pcap_compile: %s\n", pcap_geterr(handle));
        pcap_close(handle);
        return NULL;
    }
    if (pcap_setfilter(handle, &fp) == -1) {
        fprintf(stderr, "pcap_setfilter: %s\n", pcap_geterr(handle));
        pcap_freecode(&fp);
        pcap_close(handle);
        return NULL;
    }
	//free memory
    pcap_freecode(&fp);

    // starting capture
    pcap_loop(handle, -1, packet_handler, (u_char*)&cargs);

    pcap_close(handle);
    return NULL;

}
void *handle_thread(void *arg) {
	//this condition is asure that if the program is stop (like the user using ctrl + C), the cosumber will handle all data in the queue before stop. to avoid lost data.
	uint64_t prev = 0;
    //while (std::atomic_load(&running_state) || g_queue.count > 0) {
	std::ofstream clear_file(LOG_PATH, std::ios::trunc);
	clear_file.close();
	while(true){
		sleep(1);
        write_csv(std::atomic_load(&total_packets) - prev, LOG_PATH); 
		prev = atomic_load(&total_packets);
    }
    return NULL;
}
