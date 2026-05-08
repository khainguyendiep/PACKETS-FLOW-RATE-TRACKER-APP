#include <pcap.h> 
#include "capture.h"
#include "core/logger/logger.h"
#include "core/utils/utils.h"

const std::string log_path = "/var/log/packets_flow_rate_tracker/packets_flow_rate_tracker.log";

void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet){
	capture_packet_state *state = (capture_packet_state*)args; //using local variables
	state->count_packets++;
	if(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - state->timeBegin).count() > 1000){
		write_csv(state->count_packets, format_timestamp_from_header(header), log_path);
		state->count_packets = 0;
		state->timeBegin = std::chrono::steady_clock::now();
	}
}
