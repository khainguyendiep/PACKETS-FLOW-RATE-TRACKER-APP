#ifndef CAPTUREPACKET_H
#define CAPTUREPACKET_H
#include <pcap.h>
#include <chrono>

struct capture_packet_state{
	pcap_t *handle;
	long count_packets;
	std::chrono::steady_clock::time_point timeBegin = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point timeEnd = std::chrono::steady_clock::now();
};
void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

#endif
