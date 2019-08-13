#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/use_future.hpp>

#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Log.h>

#include <fmt/format.h>

#include <netstat.hpp>
#include <runner.hpp>

using boost::thread;
using boost::asio::ip::tcp;
using boost::asio::ip::udp;

using std::map;
using std::string;
using std::vector;

chrono_clock start, end;
map<string, uint64_t> stat_count;

void dequeueStat(const Config &config, std::fstream &file,
                 NetStatQueue &queue) {
	NetStat stat;
	while (queue.try_dequeue(stat)) {
		string name = fmt::format("{}.{}.{}", NetProtoStr[stat.Proto],
		                          NetOperStr[stat.Type], NetErrStr[stat.Error]);
		stat_count[name]++;

		file << stat.TimeStamp << "\t" << stat.Id << "\t"
		     << NetProtoStr[stat.Proto] << "\t" << NetOperStr[stat.Type] << "\t"
		     << NetErrStr[stat.Error] << "\t" << stat.Elapsed << "\t"
		     << stat.Size << "\n";

		if (file.fail()) {
			throw std::runtime_error(config.StatFile + " " + strerror(errno));
		}
	}
}

void dequeueThread(const Config &config, NetStatQueue &queue) {
	LOG_VERBOSE << "Starting dequeue thread";
	try {
		std::fstream file;
		file.open(config.StatFile, std::ios_base::in);
		if (file.good()) {
			file.close();
			throw std::runtime_error(config.StatFile + " already exist");
		}
		file.open(config.StatFile, std::ios_base::out);
		if (file.fail()) {
			throw std::runtime_error(config.StatFile + " " + strerror(errno));
		}
		file << "Timestamp\tConId\tProto\tType\tStatus\tElapsed(us)\tSize\n";
		if (file.fail()) {
			throw std::runtime_error(config.StatFile + " " + strerror(errno));
		}

		while (running.load()) {
			dequeueStat(config, file, queue);
			boost::this_thread::sleep(boost::posix_time::milliseconds(100));
		}

		file.close();
	} catch (std::exception &e) {
		running.store(false);
		// fatal error
		LOG_FATAL << "dequeue thread: " << e.what();
	}
	LOG_VERBOSE << "Shutdown dequeue thread";
}

void runClients(const Config &config) {
	static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
	plog::init(config.LogLevel, &consoleAppender);

	LOG_INFO << "Starting with " << config.Workers << " TCP clients and "
	         << config.UWorkers << " UDP clients";

	NetStatQueue queue;

	boost::asio::io_context io_context;
	boost::asio::io_service::work work(io_context);

	vector<std::shared_ptr<ClientTCP>> clientsTCP;
	vector<std::shared_ptr<ClientUDP>> clientsUDP;

	if (config.Workers > 0) {
		clientsTCP.resize(config.Workers);
	}
	if (config.UWorkers > 0) {
		clientsUDP.resize(config.UWorkers);
	}

	boost::thread thread_q;

	running.store(true);

	thread_q =
	    thread(dequeueThread, std::ref(config), std::ref(queue));

	boost::this_thread::sleep(boost::posix_time::milliseconds(100));
	if (!running.load())
		return;

	int thread_count = config.Threads;
	if (thread_count > 3)
		thread_count--;
	LOG_INFO << "Thread count " << thread_count;

	for (int i = 0; i < config.Workers; i++) {
		clientsTCP[i] = std::make_shared<ClientTCP>(io_context, config, i, queue);
		clientsTCP[i]->start();
	}
	for (int i = 0; i < config.UWorkers; i++) {
		clientsUDP[i] = std::make_shared<ClientUDP>(io_context, config, i, queue);
		clientsUDP[i]->start();
	}

	start = TIME_NOW;

	boost::thread_group threads_ioc;
	for (int i = 0; i < thread_count; ++i) {
		threads_ioc.create_thread(
		    boost::bind(&boost::asio::io_context::run, &io_context));
	}

	for (int i = 0; running.load() && i < config.Duration * 10; i++) {
		boost::this_thread::sleep(boost::posix_time::milliseconds(100));
	}

	LOG_INFO << "Shutting down";
	for (int i = 0; i < config.Workers; i++) {
		clientsTCP[i]->stop();
	}
	end = TIME_NOW;

	io_context.stop();

	running.store(false);

	threads_ioc.join_all();
	thread_q.join();

	using float_seconds = std::chrono::duration<double>;
	auto duration =
	    std::chrono::duration_cast<float_seconds>(end - start).count();
	if (duration > 0) {
		std::cout << std::fixed;
		std::cout << "Test duration " << duration << " s" << std::endl;
		for (auto &it : stat_count) {
			std::cout << it.first << ": " << it.second << " ("
			          << it.second / duration << " op/s)" << std::endl;
		}
	}
}
