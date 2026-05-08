#include "utils.h"
#include <pcap.h>
#include <chrono>
#include <sstream>
#include <iomanip>

// ISO 8601 format
std::string format_timestamp_from_header(const struct pcap_pkthdr *header) {
    struct tm *tm_info = std::gmtime(&header->ts.tv_sec);
    char buf[16]; 
    strftime(buf, sizeof(buf), "%H:%M:%S", tm_info);
    return std::string(buf);
}
