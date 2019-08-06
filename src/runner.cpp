#include <iostream>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/fiber/barrier.hpp>

#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Log.h>

#include <runner.hpp>
#include <netstat.hpp>

using boost::thread;
using boost::asio::ip::tcp;
using boost::asio::ip::udp;
using boost::fibers::barrier;
using std::string;

void dequeueThread(const Config &config, ClientData &data, barrier &wb) {
	wb.wait();
	LOG_VERBOSE << "Starting dequeue thread " << data.Id;
	try {

	} catch (std::exception &e) {
		running.store(false);
		// log fatal error
		LOG_ERROR << "dequeue thread : " << e.what();
	}
	wb.wait();
	LOG_VERBOSE << "Shutdown dequeue thread " << data.Id;
}

void runClients(const Config &config) {
	static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
	plog::init(config.LogLevel, &consoleAppender);

	LOG_INFO << "Starting with " << config.Workers << " TCP clients and "
	          << config.UWorkers << " UDP clients";

	int threadsCount = config.Workers + config.UWorkers;

	NetStatQueue queue;

	Thread *threads = new Thread[threadsCount];
	int last = 0;

	running.store(true);
	barrier wb(threadsCount + 1);

	for (int i = 0; i < config.Workers; i++) {
		threads[last].data.Id = i;
		threads[last].t = thread(clientTCPThread, std::ref(config),
		                         std::ref(threads[last].data), std::ref(wb));
		last++;
	}
	for (int i = 0; i < config.UWorkers; i++) {
		threads[last].data.Id = i;
		threads[last].t = thread(clientUDPThread, std::ref(config),
		                         std::ref(threads[last].data), std::ref(wb));
		last++;
	}

	wb.wait(); // wait for start
	sleep(config.Duration);

	LOG_INFO << "Shutting down";
	running.store(false);
	wb.wait(); // wait for end

	for (int i = 0; i < last; i++) {
		threads[i].t.join();
	}
	delete[] threads;
}
