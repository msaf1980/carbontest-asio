#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/fiber/barrier.hpp>

#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Log.h>

#include <fmt/format.h>

#include <netstat.hpp>
#include <runner.hpp>

using boost::thread;
using boost::asio::ip::tcp;
using boost::asio::ip::udp;
using boost::fibers::barrier;

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

void dequeueThread(const Config &config, barrier &wb, NetStatQueue &queue) {
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
		wb.wait();

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

	int clientsCount = 0;
	vector<Client *> clients;

	if (config.Workers > 0) {
		clientsCount += config.Workers;
	}

	clients.reserve(clientsCount);

	boost::thread thread_q;
	int last = 0;

	running.store(true);
	barrier wb(2);

	thread_q =
	    thread(dequeueThread, std::ref(config), std::ref(wb), std::ref(queue));

	boost::this_thread::sleep(boost::posix_time::milliseconds(100));
	if (!running.load())
		return;

	for (int i = 0; i < config.Workers; i++) {
		ClientTCP *c = new ClientTCP(io_context, config, i, wb, queue);
		clients.push_back(c);
		c->start();
		last++;
	}
	// for (int i = 0; i < config.UWorkers; i++) {
	// threads[last].data.Id = i;
	// threads[last].t =
	// thread(clientUDPThread, std::ref(config),
	// std::ref(threads[last].data), std::ref(wb), std::ref(queue));
	// last++;
	//}

	boost::thread thread_ioc([&io_context]() { io_context.run(); });

	wb.wait();

	start = TIME_NOW;
	for (int i = 0; running.load() && i < config.Duration * 10; i++) {
		boost::this_thread::sleep(boost::posix_time::milliseconds(100));
	}

	LOG_INFO << "Shutting down";
	for (int i = 0; i < last; i++) {
		Client *client = clients[i];
		switch (client->getProto()) {
		case NetProto::TCP: {
			ClientTCP *c = static_cast<ClientTCP *>(client);
			c->stop();
			break;
		}
			// case NetProto::UDP:
			// std::static_cast<ClientUDP*>(client)->close();
			// break;
		default:
			LOG_ERROR << "unhandled close client type " << client->getProto();
		}
	}
	running.store(false);

	end = TIME_NOW;

	thread_q.join();
	thread_ioc.join();
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
