#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#include <stdlib.h>
#include <stdint.h>

#include <atomic>
#include <string>

extern std::atomic_bool running; // running flag

struct Config {
	// Addr string
	int Duration; // Test duration in seconds

	size_t Workers; // TCP Workers
	// MetricPerCon int
	// BatchSend    int
	// RateLimit    []int32
	// SendDelay   time.Duration
	// ConTimeout  time.Duration
	// SendTimeout time.Duration

	size_t UWorkers; // UDP Workers
	                 // UBatchSend  int

	std::string MetricPrefix; // Prefix for generated metric name

	// Verbose      bool

	// StatFile string // write connections stat to file
	// CPUProf  string // write cpu profile info to file
};

void parseArgs(Config &config, int argc, char *argv[]);

#endif /* _CONFIG_HPP_ */
