#include <stdexcept>
#include <string>

#include <cxxopts.hpp>

#include <config.hpp>

using std::string;

std::atomic_bool running;

void parseArgs(Config &config, int argc, char *argv[]) {
	cxxopts::Options options(
	    argv[0], "Load testing of carbon daemons (relay, cache, etc)");
	options.positional_help("[optional args]").show_positional_help();

	// clang-format off
	options.add_options()
		("h,help", "Print help")
		("l,loglevel", "Log level",
	                      cxxopts::value<string>()->default_value("INFO"))
		("p,prefix", "Metric prefix",
	                      cxxopts::value<string>()->default_value("test"))
		("d,duration", "Test duration (in seconds)",
	                      cxxopts::value<int>()->default_value("10"))
		("w,workers", "TCP workers", cxxopts::value<int>()->default_value("10"))
		("u,uworkers", "UDP workers", cxxopts::value<int>()->default_value("0"))
	;
	// clang-format on

	auto result = options.parse(argc, argv);
	if (result.count("help")) {
		std::cout << options.help({"", "Group"}) << std::endl;
		exit(0);
	}

	int    num;
	string arg;
	try {
		arg = "workers";
		num = result[arg].as<int>();
		config.Workers = num;
		arg = "uworkers";
		num = result[arg].as<int>();
		if (num >= 0)
			config.UWorkers = num;
		arg = "duration";
		num = result[arg].as<int>();
		if (num >= 0)
			config.Duration = num;
		arg = "prefix";
		config.MetricPrefix = result[arg].as<string>();
	} catch (std::bad_cast &e) {
		throw std::runtime_error("parameter '" + arg + "' has incorrect type");
	}
}
