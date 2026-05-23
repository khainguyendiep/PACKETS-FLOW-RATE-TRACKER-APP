#ifndef  CAPTURE_H
#define CAPTURE_H

#include <stdio.h>
#include <pthread.h>
#include <pcap.h>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <pthread.h>
#include <atomic>
#include <unistd.h>
#include <fstream>

#define QUEUE_CAP 8
#define MAX_DEVS 10000

// ── simple queue using mutex ──────────────────────────────────
struct Queue{
    int         data[QUEUE_CAP];
    int         head, tail, count;
    pthread_mutex_t lock;
    pthread_cond_t  not_empty;  // Notification that new data is available
    pthread_cond_t  not_full;   // Notification that spots are available
};

//Those variables available in main
extern Queue				g_queue;
extern std::atomic<int>		running_state;
extern std::atomic<long>	total_packets;

//Pass parameter into threads 
struct capture_args_t{
	char *dev_name = NULL; // name of interface
	char errbuf[PCAP_ERRBUF_SIZE];
    bpf_u_int32 net, mask;
	pcap_if_t *alldevsp = NULL;
	char *listDevs[MAX_DEVS];
	char *list_ipv4_devs[MAX_DEVS];
	char *ipv4_dev; // own IPv4 address
};

void packet_handler(u_char *args,
                    const struct pcap_pkthdr *header,
                    const u_char *packet);

void queue_init(Queue *q);

void queue_push(Queue *q, int val);

int queue_pop(Queue *q);

void *capture_thread(void *arg);

void *handle_thread(void *agr);

#endif
