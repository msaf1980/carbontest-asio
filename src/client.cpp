#include <unistd.h>

#include <atomic>
#include <iostream>
#include <string>
#include <thread>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/basic_resolver_query.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/use_future.hpp>

#include <boost/asio/buffer.hpp>

#include <fmt/format.h>

#include <client.hpp>

using boost::asio::mutable_buffer;
using boost::asio::ip::tcp;
using boost::asio::ip::udp;
using std::string;

void clientTCPThread(const Config &config, ClientData &data, barrier &wb) {
	wb.wait();

	boost::asio::io_context   io_context;
	boost::system::error_code ec;
	tcp::endpoint endpoint(boost::asio::ip::address::from_string(config.Host), config.Port);
	tcp::socket             socket(io_context);

	fmt::memory_buffer out;
	string             metricPrefix =
	    fmt::format("{:s}.{:d}", config.MetricPrefix, data.Id);
	// string metricString = config.MetricPrefix + "." +
	// std::to_string(data.Id);
	while (running.load()) {
		format_to(out, "{:s} {:d} {:d}\n", metricPrefix, 1, 12);
		mutable_buffer buf(out.data(), out.size());
		// std::cout << "TCP " << out.data() << std::endl;
		socket.connect(endpoint, ec);
		if (ec) {
			// An error occurred.
		} else {
			size_t len = socket.write_some(buf, ec);
			socket.close(ec);
		}
	}
	wb.wait();
}

void clientUDPThread(const Config &config, ClientData &data, barrier &wb) {
	wb.wait();
	boost::asio::io_context io_context;
	while (running.load()) {
		usleep(100);
		std::cout << "UDP " << data.Id << std::endl;
	}
	wb.wait();
}
