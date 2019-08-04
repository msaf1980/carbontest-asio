#include <iostream>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/fiber/barrier.hpp>

#include <runner.hpp>
#include <timer.hpp>

using boost::thread;
using boost::asio::ip::tcp;
using boost::asio::ip::udp;
using boost::fibers::barrier;
using std::string;

void runClients(const Config &config) {
	size_t  threadsCount = config.Workers + config.UWorkers;
	Thread *threads = new Thread[threadsCount];
	size_t  last = 0;

	running.store(1);
	barrier wb(threadsCount + 1);

	for (size_t i = 0; i < config.Workers; i++) {
		threads[last].data.Id = i;
		threads[last].t = thread(clientTCPThread, std::ref(config),
		                         std::ref(threads[last].data), std::ref(wb));
		last++;
	}
	for (size_t i = 0; i < config.UWorkers; i++) {
		threads[last].data.Id = i;
		threads[last].t = thread(clientUDPThread, std::ref(config),
		                         std::ref(threads[last].data), std::ref(wb));
		last++;
	}

	wb.wait(); // wait for start
	sleep(config.Duration);

	running.store(0);
	wb.wait(); // wait for end

	for (size_t i = 0; i < last; i++) {
		threads[i].t.join();
	}
	delete[] threads;
}
