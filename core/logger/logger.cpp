#include "core/logger/logger.h"
#include <chrono>
#include <iomanip>
#include <fstream>

void write_csv(long number_of_packet, const std::string &log_path){
	std::ofstream log_file(log_path, std::ios::app);
	
	auto local_time_now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(local_time_now);
    std::tm* ltm = std::localtime(&now_c);
	std::ostringstream timestamp;
	timestamp << std::put_time(ltm, "%H %M %S");
	if (log_file.is_open()) {
        // dump() with no arguments ensures the JSON is on one single line
        log_file << number_of_packet <<", " << timestamp.str() << std::endl;
        log_file.close();
    }
}
