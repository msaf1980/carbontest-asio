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

	int Workers;      // TCP Workers
	int MetricPerCon; // Metrics, sended in one connection (TCP)

	int UWorkers; // UDP Workers

	// RateLimit    []int32
	int SendDelay; // Send delay in milliseconds

	int ConTimeout;  // Connection timeout
	int SendTimeout; // Send timeout

	std::string MetricPrefix; // Prefix for generated metric name

	plog::Severity LogLevel;

	// Verbose      bool

	// StatFile string // write connections stat to file
	// CPUProf  string // write cpu profile info to file
};

void parseArgs(Config &config, int argc, char *argv[]);

#endif /* _CONFIG_HPP_ */
