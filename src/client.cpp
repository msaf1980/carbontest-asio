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
	try {
		boost::asio::io_context   io_context;
		boost::system::error_code ec;
		tcp::endpoint             endpoint(
            boost::asio::ip::address::from_string(config.Host), config.Port);
		tcp::socket socket(io_context);

		string             metricPrefix =
		    fmt::format("{:s}.{:d}", config.MetricPrefix, data.Id);
		// string metricString = config.MetricPrefix + "." +
		// std::to_string(data.Id);
		//
		while (running.load()) {
			fmt::memory_buffer out;
			format_to(out, "{:s} {:d} {:d}\n", metricPrefix, 1, 12);
			mutable_buffer buf(out.data(), out.size());
			// std::cout << "TCP " << out.data() << std::endl;
			socket.connect(endpoint, ec);
			if (ec) {
				// An error occurred.
				std::cerr << "tcp error: " << ec.message() << '\n';
			} else {
				size_t len = socket.write_some(buf, ec);
				if (ec) {
					// An error occurred.
					std::cerr << "tcp error: " << ec.message() << '\n';
				}
				socket.close(ec);
				if (ec) {
					// An error occurred.
					std::cerr << "tcp error: " << ec.message() << '\n';
				}
			}
		}
	} catch (std::exception &e) {
		// log fatal error
		std::cerr << "fatal error in tcp thread: " << e.what() << '\n';
	}
	wb.wait();
}

void clientUDPThread(const Config &config, ClientData &data, barrier &wb) {
	wb.wait();
	try {
		boost::asio::io_context   io_context;
		boost::system::error_code ec;
		udp::endpoint             endpoint(
            boost::asio::ip::address::from_string(config.Host), config.Port);
		udp::socket socket(io_context);

		string             metricPrefix =
		    fmt::format("{:s}.{:d}", config.MetricPrefix, data.Id);
		while (running.load()) {
			fmt::memory_buffer out;
			format_to(out, "{:s} {:d} {:d}\n", metricPrefix, 1, 12);
			mutable_buffer buf(out.data(), out.size());
			socket.open(udp::v4(), ec);
			if (ec) {
				// An error occurred.
				std::cerr << "udp error: " << ec.message() << '\n';
			} else {
				socket.send_to(buf, endpoint, 0, ec);
				if (ec) {
					// An error occurred.
					std::cerr << "udp error: " << ec.message() << " " << out.size() << '\n';
				}
				socket.close(ec);
				if (ec) {
					// An error occurred.
					std::cerr << "udp error: " << ec.message() << '\n';
				}
			}
		}
	} catch (std::exception &e) {
		// log fatal error
		std::cerr << "fatal error in udp thread: " << e.what() << '\n';
	}
	wb.wait();
}