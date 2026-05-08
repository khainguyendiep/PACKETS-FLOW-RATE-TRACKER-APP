#include "core/logger/logger.h"
#include <fstream>

void write_csv(long number_of_packet, std::string timestamp, const std::string &log_path){
	std::ofstream log_file(log_path, std::ios::app);
	if (log_file.is_open()) {
        // dump() with no arguments ensures the JSON is on one single line
        log_file << number_of_packet <<", " << timestamp << std::endl;
        log_file.close();
    }
}
