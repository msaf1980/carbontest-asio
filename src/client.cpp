#include <unistd.h>

#include <atomic>
#include <chrono>
#include <iostream>
#include <string>

#include <cstring>

#include <boost/asio.hpp>

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
		case boost::asio::error::timed_out:
		case boost::asio::error::operation_aborted:
		case boost::asio::error::connection_aborted:
			return NetErr::TIMEOUT;
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

NetProto Client ::getProto() { return stat_.Proto; }

//###########################################################
// ClientTCP
ClientTCP::ClientTCP(boost::asio::io_context &io_context, const Config &config,
                     size_t id, NetStatQueue &queue)
    : config_(config), io_context_(&io_context), socket_(io_context),
      queue_(&queue), deadline_(io_context) {
	stat_.Proto = NetProto::TCP;
	stat_.Id = id;
	// socket_.set_option(tcp::acceptor::reuse_address(true));
	int enable = 1;
	setsockopt(socket_.native_handle(), SOL_SOCKET, SO_REUSEADDR, &enable,
			   sizeof(enable));
}

void ClientTCP::start() { start_connect(); }

void ClientTCP::stop() {
	stopped_ = true;
	boost::asio::post(*io_context_, [this]() {
		boost::system::error_code ec;
		if (socket_.is_open()) {
			socket_.close(ec);
		}
		deadline_.cancel(ec);
	});
}

void ClientTCP::check_deadline() {
	if (stopped_)
		return;

	// Check whether the deadline has passed. We compare the deadline against
	// the current time since a new asynchronous operation may have moved the
	// deadline before this actor had a chance to run.
	if (deadline_.expiry() <= steady_timer::clock_type::now()) {
		// The deadline has passed. The socket is closed so that any outstanding
		// asynchronous operations are cancelled.
		if (socket_.is_open()) {
			boost::system::error_code ec;
			socket_.close(ec);
		}
		// There is no longer an active deadline. The expiry is set to the
		// maximum time point so that the actor takes no action until a new
		// deadline is set.
		deadline_.expires_at(steady_timer::time_point::max());
	}
}

void ClientTCP::do_reconnect() {
	if (stopped_)
		return;
	boost::system::error_code ec;
	deadline_.cancel(ec);
	if (socket_.is_open()) {
		socket_.close(ec);
		LOG_VERBOSE << "Close TCP session " << stat_.Id;
	}
	start_connect();
}

void ClientTCP::start_connect() {
	if (stopped_)
		return;

	LOG_VERBOSE << "Starting TCP session " << stat_.Id;
	stat_.Type = NetOper::CONNECT;
	start_ = TIME_NOW;

	tcp::endpoint endpoint(boost::asio::ip::address::from_string(config_.Host),
	                       config_.Port);
	// Set a deadline for the connect operation.
	deadline_.expires_after(
	    boost::asio::chrono::milliseconds(config_.ConTimeout));

	// Start the asynchronous connect operation.
	socket_.async_connect(endpoint, [this](boost::system::error_code ec) {
		auto end = TIME_NOW;

		boost::system::error_code ec_ignored;
		// deadline_.cancel(ec);
		NetStatSet(stat_, ec_ignored, start_, end);
		stat_.Size = 0;
		if (!socket_.is_open()) {
			// If the socket is closed at this time then
			// the timeout handler must have run first.
			stat_.Error = NetErr::TIMEOUT;
			queue_->enqueue(stat_);
			start_connect();
		} else {
			NetStatSet(stat_, ec, start_, end);
			queue_->enqueue(stat_);
			if (ec) {
				if (stat_.Error == NetErr::ERROR) {
					LOG_DEBUG << "Connect TCP session " << stat_.Id
					            << " error unknown: " << ec.message();
				}
				do_reconnect();
			} else {
				do_write();
			}
		}
	});

	check_deadline();
}

void ClientTCP::do_write() {
	if (stopped_)
		return;

	LOG_VERBOSE << "Write TCP session " << stat_.Id;
	stat_.Type = NetOper::SEND;
	start_ = TIME_NOW;

	fmt::memory_buffer out;

	auto timeStamp = std::chrono::duration_cast<std::chrono::seconds>(
	                     start_.time_since_epoch())
	                     .count();
	format_to(out, "{:s}.{:d} {:d} {:d}\n", config_.MetricPrefix, stat_.Id,
	          timeStamp % 60 + stat_.Id, timeStamp);

	deadline_.expires_after(boost::asio::chrono::milliseconds(config_.Timeout));

	boost::asio::async_write(
	    socket_, boost::asio::buffer(out.data(), out.size()),
	    boost::bind(&ClientTCP::handle_write, this, _1, _2));

	check_deadline();
}

void ClientTCP::handle_write(const boost::system::error_code &ec,
                             std::size_t length) {
	auto end = TIME_NOW;
	NetStatSet(stat_, ec, start_, end);
	if (!socket_.is_open()) {
		// If the socket is closed at this time then
		// the timeout handler must have run first.
		stat_.Error = NetErr::TIMEOUT;
		queue_->enqueue(stat_);
		start_connect();
	} else {
		if (ec) {
			if (stat_.Error == NetErr::ERROR) {
				LOG_DEBUG << "Write TCP session " << stat_.Id
				            << " error unknown: " << ec.message();
			}
			stat_.Size = 0;
			queue_->enqueue(stat_);
			do_reconnect();
		} else {
			LOG_VERBOSE << "Write TCP session " << stat_.Id
			            << " done: " << length;
			stat_.Size = length;
			queue_->enqueue(stat_);
			do_reconnect();
		}
	}
}

//###########################################################
// ClientUDP

void ClientUDP::start() { do_write(); }

void ClientUDP::stop() { stopped_ = true; }

void ClientUDP::do_write() {
	if (stopped_)
		return;

	LOG_VERBOSE << "Write UDP session " << stat_.Id;
	stat_.Type = NetOper::SEND;
	start_ = TIME_NOW;

	fmt::memory_buffer out;

	auto timeStamp = std::chrono::duration_cast<std::chrono::seconds>(
	                     start_.time_since_epoch())
	                     .count();
	format_to(out, "{:s}.{:d} {:d} {:d}\n", config_.MetricPrefix, stat_.Id,
	          timeStamp % 60 + stat_.Id, timeStamp);

	udp::socket socket(*io_context_, udp::endpoint(udp::v4(), 0));
	udp::endpoint endpoint(boost::asio::ip::address::from_string(config_.Host),
	                       config_.Port);

	socket.async_send_to(boost::asio::buffer(out.data(), out.size()), endpoint,
	                     boost::bind(&ClientUDP::handle_write, this, _1, _2));
}

void ClientUDP::handle_write(const boost::system::error_code &ec,
                             std::size_t length) {
	auto end = TIME_NOW;
	NetStatSet(stat_, ec, start_, end);
	if (ec) {
		if (stat_.Error == NetErr::ERROR) {
			LOG_WARNING << "Write TCP session " << stat_.Id
			            << " error unknown: " << ec.message();
		}
		stat_.Size = 0;
		queue_->enqueue(stat_);
	} else {
		LOG_VERBOSE << "Write TCP session " << stat_.Id << " done: " << length;
		stat_.Size = length;
		queue_->enqueue(stat_);
	}
	do_write();
}
