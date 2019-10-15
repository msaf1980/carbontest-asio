#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#include <stdint.h>
#include <stdlib.h>

#include <atomic>
#include <string>

#include <plog/Severity.h>

extern std::atomic_bool running; // running flag

struct Config {
	std::string Host;
	int Port;

	int Duration; // Test duration in seconds

	int Threads; // Threads count

	int Workers;      // TCP Workers
	int MetricPerCon; // Metrics, sended in one connection (TCP)

	int UWorkers; // UDP Workers

	// RateLimit    []int32
	int Delay; // Send delay in milliseconds

	int ConTimeout;  // Connection timeout
	int Timeout; // Send timeout

	std::string MetricPrefix; // Prefix for generated metric name

	plog::Severity LogLevel;

	std::string StatFile; // write connections stat to file
};

void parseArgs(Config &config, int argc, char *argv[]);

#endif /* _CONFIG_HPP_ */
