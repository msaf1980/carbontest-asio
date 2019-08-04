#include <iostream>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/fiber/barrier.hpp>

#include <runner.hpp>

using boost::thread;
using boost::asio::ip::tcp;
using boost::asio::ip::udp;
using boost::fibers::barrier;
using std::string;

void runClients(const Config &config) {
	int  threadsCount = config.Workers + config.UWorkers;
	Thread *threads = new Thread[threadsCount];
	int  last = 0;

	running.store(1);
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

	running.store(0);
	wb.wait(); // wait for end

	for (int i = 0; i < last; i++) {
		threads[i].t.join();
	}
	delete[] threads;
}
