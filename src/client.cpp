#include <unistd.h>

#include <iostream>
#include <string>
#include <thread>
#include <atomic>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/use_future.hpp>

#include <client.hpp>

void clientTCPThread(const Config &config, ClientData &data, barrier &wb) {
	wb.wait();
	while (running.load()) {
		usleep(100);
		std::cout << "TCP " << data.Id << std::endl;
	}
	wb.wait();
}

void clientUDPThread(const Config &config, ClientData &data, barrier &wb) {
	wb.wait();
	while (running.load()) {
		usleep(100);
		std::cout << "UDP " << data.Id << std::endl;
	}
	wb.wait();
}
