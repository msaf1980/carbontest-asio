#include <atomic>
#include <iostream>
#include <string>

#include "config.hpp"
#include "runner.hpp"

int main(int argc, char *argv[]) {
	Config config;
	try {
		parseArgs(config, argc, argv);
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		exit(1);
	}

	runClients(config);
}
