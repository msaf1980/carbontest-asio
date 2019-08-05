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
		("H,host", "IP Address to connect",
	                      cxxopts::value<string>()->default_value("127.0.0.1"))
		("P,port", "Port to connect",
	                      cxxopts::value<int>()->default_value("2003"))
		("p,prefix", "Metric prefix",
	                      cxxopts::value<string>()->default_value("test"))
		("d,duration", "Test duration (in seconds)",
	                      cxxopts::value<int>()->default_value("10"))
		("w,workers", "TCP workers", cxxopts::value<int>()->default_value("10"))
		("u,uworkers", "UDP workers", cxxopts::value<int>()->default_value("0"))
		("m,metrics", "Metrics, sended in one TCP connection",
	                      cxxopts::value<int>()->default_value("1"))
		("S,send_delay", "Send delay (in milliseconds)",
	                      cxxopts::value<int>()->default_value("0"))
		("c,con_timeout", "Connection timeout (in milliseconds)",
	                      cxxopts::value<int>()->default_value("100"))
		("s,send_timeout", "Send timeout (in milliseconds)",
	                      cxxopts::value<int>()->default_value("500"))
	;
	// clang-format on

	auto result = options.parse(argc, argv);
	if (result.count("help")) {
		std::cout << options.help({"", "Group"}) << std::endl;
		exit(0);
	}

	string arg;
	try {
		arg = "workers";
		config.Workers = result[arg].as<int>();
		if (config.Workers < 0)
			throw std::invalid_argument(arg);

		arg = "uworkers";
		config.UWorkers = result[arg].as<int>();
		if (config.UWorkers < 0)
			throw std::invalid_argument(arg);

		arg = "duration";
		config.Duration = result[arg].as<int>();
		if (config.Duration <= 0)
			throw std::invalid_argument(arg);

		arg = "host";
		config.Host = result[arg].as<string>();

		arg = "port";
		config.Port = result[arg].as<int>();
		if (config.Port <= 0)
			throw std::invalid_argument(arg);

		arg = "prefix";
		config.MetricPrefix = result[arg].as<string>();

		arg = "metrics";
		config.MetricPerCon = result[arg].as<int>();
		if (config.MetricPerCon <= 0)
			throw std::invalid_argument(arg);

		arg = "send_delay";
		config.SendDelay = result[arg].as<int>();
		if (config.SendDelay < 0)
			throw std::invalid_argument(arg);

		arg = "con_timeout";
		config.ConTimeout = result[arg].as<int>();
		if (config.ConTimeout < 0)
			throw std::invalid_argument(arg);

		arg = "send_timeout";
		config.SendTimeout = result[arg].as<int>();
		if (config.SendTimeout < 0)
			throw std::invalid_argument(arg);

	} catch (std::bad_cast &e) {
		throw std::runtime_error("parameter '" + arg + "' has incorrect type");
	} catch (std::invalid_argument &e) {
		throw std::invalid_argument("parameter '" + std::string(e.what()) +
		                            "' has incorrect value");
	}
}
