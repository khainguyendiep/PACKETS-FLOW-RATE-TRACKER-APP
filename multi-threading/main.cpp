#include "capture.h"
#include "core/network/networkUtils.h"
#include <cstdio>

Queue           	g_queue;
std::atomic<int>	running_state = 1; // state of program running (1) or not (0)
std::atomic<long>	total_packets = 0; // number of packets has been handle (go through all pipline)
//const int MAX_DEVS = 10000; //just a random number
					
int main() {
	// ----------------------------------------------------------------------------------------------------------------------
	printf("Starting capture packets\n");
    queue_init(&g_queue);
	
	// Init counters
    atomic_init(&total_packets, 0);
	
	capture_args_t cargs;
	
	//--- list all dev available ---------------------------------------
	for(int i=0; i<MAX_DEVS; i++){
		cargs.listDevs[i] = NULL;
		cargs.list_ipv4_devs[i] = NULL;	
	}
	if(pcap_findalldevs(&cargs.alldevsp, cargs.errbuf) == -1){
		printf("Finding netword card error!!!");
		return (2);
	}
	choosingDev(cargs.alldevsp, cargs.listDevs, &cargs.dev_name, cargs.list_ipv4_devs, &cargs.ipv4_dev);
	//------------------------------------------------------------------

    pthread_t tid_cap, tid_hand; //thread id

	//create 2 distinct thread
    pthread_create(&tid_cap, NULL, capture_thread, &cargs); //producer (capture_thread)
    pthread_create(&tid_hand, NULL, handle_thread, NULL); //consumer (handle_thread)
	sleep(5);

	//sending a signal to all thread that value of running_state changed to 0 (other values on cache of each thread are outdated)
    atomic_store(&running_state, 0);

    // waking up consumer if it is waiting
    pthread_cond_signal(&g_queue.not_empty);

    pthread_join(tid_cap, NULL);
    pthread_join(tid_hand, NULL);

    printf("Handled: %ld packets\n", std::atomic_load(&total_packets));
}
