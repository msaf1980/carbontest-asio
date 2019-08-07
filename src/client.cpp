#include <unistd.h>

#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
//#include <thread>
#include <chrono>

#include <cstring>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/basic_resolver_query.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/use_future.hpp>

#include <boost/asio/buffer.hpp>

#include <concurrentqueue.h>

#include <fmt/format.h>

#include <plog/Log.h>

#include <client.hpp>

using boost::asio::mutable_buffer;
using boost::asio::ip::tcp;
using boost::asio::ip::udp;
using std::string;

NetErr NetErrorFromEc(const boost::system::error_code &ec) {
	if (ec) {
		switch (ec.value()) {
		case boost::system::errc::connection_refused:
			return NetErr::REFUSED;
		case boost::system::errc::connection_reset:
			return NetErr::RESET;
		case boost::system::errc::broken_pipe:
			return NetErr::PIPE;
		default:
			return NetErr::ERROR;
			// enum NetErr { PIPE, TIMEOUT, UNREACHEABLE, LOOKUP, REFUSED, RESET
			// };
		}
	} else {
		return NetErr::OK;
	}
}

void NetStatSet(NetStat &stat, const boost::system::error_code &ec,
                const chrono_clock &start, const chrono_clock &end) {
	stat.Error = NetErrorFromEc(ec);
	stat.TimeStamp = std::chrono::duration_cast<std::chrono::milliseconds>(
	                     start.time_since_epoch())
	                     .count();
	stat.Elapsed =
	    std::chrono::duration_cast<std::chrono::microseconds>(end - start)
	        .count();
}

void set_timeout_ms(struct timeval *tv, int ms) {
	tv->tv_sec = ms / 1000;
	tv->tv_usec = ms % 1000 * 1000;
}

int set_recv_timeout(int sock_fd, struct timeval *tv) {
	return setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &tv,
	                  sizeof(struct timeval));
}

int set_send_timeout(int sock_fd, struct timeval *tv) {
	return setsockopt(sock_fd, SOL_SOCKET, SO_SNDTIMEO, tv,
	                  sizeof(struct timeval));
}

void clientTCPThread(const Config &config, ClientData &data, barrier &wb,
                     NetStatQueue &queue) {
	wb.wait();
	LOG_VERBOSE << "Starting TCP thread " << data.Id;
	try {
		boost::asio::io_context io_context;
		boost::system::error_code ec;
		tcp::endpoint endpoint(
		    boost::asio::ip::address::from_string(config.Host), config.Port);
		tcp::socket socket(io_context);

		string metricPrefix =
		    fmt::format("{:s}.{:d}", config.MetricPrefix, data.Id);

		NetStat stat;
		stat.Id = data.Id;
		stat.Proto = NetProto::TCP;

		struct timeval con_timeout;
		set_timeout_ms(&con_timeout, config.ConTimeout);
		struct timeval timeout;
		set_timeout_ms(&timeout, config.Timeout);

		while (running.load()) {
			fmt::memory_buffer out;
			format_to(out, "{:s} {:d} {:d}\n", metricPrefix, 1, 12);
			mutable_buffer buf(out.data(), out.size());
			
			set_recv_timeout(socket.native_handle(), &con_timeout);
			set_send_timeout(socket.native_handle(), &con_timeout);
			auto start = TIME_NOW;
			socket.connect(endpoint, ec);
			auto end = TIME_NOW;
			stat.Type = NetOper::CONNECT;
			NetStatSet(stat, ec, start, end);
			stat.Size = 0;
			queue.enqueue(stat);
			if (ec) {
				if (stat.Error == NetErr::ERROR) {
					LOG_VERBOSE << "TCP thread " << data.Id
					            << " connect: " << ec.message();
				}
			} else {
				set_recv_timeout(socket.native_handle(), &timeout);
				set_send_timeout(socket.native_handle(), &timeout);
				auto start = TIME_NOW;
				stat.Size = socket.write_some(buf, ec);
				auto end = TIME_NOW;
				stat.Type = NetOper::SEND;
				NetStatSet(stat, ec, start, end);
				if (ec) {
					stat.Size = 0;
					if (stat.Error == NetErr::ERROR) {
						LOG_VERBOSE << "TCP thread " << data.Id
						            << " write: " << ec.message();
					}
				}
				queue.enqueue(stat);
				socket.close(ec);
			}
		}
	} catch (std::exception &e) {
		// log fatal error
		LOG_ERROR << "TCP thread " << data.Id << ": " << e.what();
	}
	wb.wait();
	LOG_VERBOSE << "Shutdown TCP thread " << data.Id;
}

void clientUDPThread(const Config &config, ClientData &data, barrier &wb,
                     NetStatQueue &queue) {
	wb.wait();
	LOG_VERBOSE << "Starting UDP thread " << data.Id;
	try {
		boost::asio::io_context io_context;
		boost::system::error_code ec;
		udp::endpoint endpoint(
		    boost::asio::ip::address::from_string(config.Host), config.Port);
		udp::socket socket(io_context);

		string metricPrefix =
		    fmt::format("{:s}.{:d}", config.MetricPrefix, data.Id);

		NetStat stat;
		stat.Id = data.Id;
		stat.Proto = NetProto::UDP;

		while (running.load()) {
			fmt::memory_buffer out;
			format_to(out, "{:s} {:d} {:d}\n", metricPrefix, 1, 12);
			mutable_buffer buf(out.data(), out.size());
			auto start = TIME_NOW;
			socket.open(udp::v4(), ec);
			auto end = TIME_NOW;
			stat.Type = NetOper::CONNECT;
			NetStatSet(stat, ec, start, end);
			stat.Size = 0;
			queue.enqueue(stat);
			if (ec) {
				// An error occurred.
				if (stat.Error == NetErr::ERROR) {
					LOG_ERROR << "UDP thread " << data.Id
					          << " connect: " << ec.message();
				}
			} else {
				auto start = TIME_NOW;
				stat.Size = socket.send_to(buf, endpoint, 0, ec);
				auto end = TIME_NOW;
				stat.Type = NetOper::SEND;
				NetStatSet(stat, ec, start, end);
				if (ec) {
					stat.Size = 0;
					if (stat.Error == NetErr::ERROR) {
						LOG_ERROR << "UDP thread " << data.Id
						          << " write: " << ec.message();
					}
				}
				socket.close(ec);
				queue.enqueue(stat);
			}
		}
	} catch (std::exception &e) {
		// log fatal error
		LOG_ERROR << "TCP thread " << data.Id << ": " << e.what();
	}
	wb.wait();
	LOG_VERBOSE << "Shutdown UDP thread " << data.Id;
}
