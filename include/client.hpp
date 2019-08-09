#ifndef _CLIENT_HPP_
#define _CLIENT_HPP_

#include <cstdlib>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/fiber/barrier.hpp>

using boost::asio::steady_timer;
using boost::asio::ip::tcp;
using boost::asio::ip::udp;
using boost::fibers::barrier;

#include <config.hpp>
#include <netstat.hpp>

#define MAX_MESSAGE_LEN 1024

struct ClientData {
	size_t Id;
};

class Client {
  public:
	NetProto getProto();
	virtual ~Client() { };
	virtual void run(){};
	virtual void stop(){};

  protected:
	NetProto proto_;     // protocol (also used as class ID)
	NetStat stat_;       // connection stat
	chrono_clock start_; // start of last operation
};

class ClientTCP : public Client {
  public:
	ClientTCP(boost::asio::io_context &io_context, const Config &config,
	          size_t id, barrier &wb, NetStatQueue &queue)
	    : config_(config), socket_(io_context),
	      wb_(&wb), queue_(&queue), deadline_(io_context) {
		stat_.Proto = NetProto::TCP;
		stat_.Id = id;
	}

	void start();
	void stop();
	void check_deadline();

  private:
	void start_connect();
	void do_reconnect();
	void do_write();
	void handle_write(const boost::system::error_code &ec, std::size_t length);

	bool stopped_ = false;
	const Config config_;
	tcp::socket socket_;
	barrier *wb_;
	NetStatQueue *queue_;
	steady_timer deadline_;
	char buf_[MAX_MESSAGE_LEN];
};

void clientTCPThread(const Config &config, ClientData &data, barrier &wb,
                     NetStatQueue &queue);
void clientUDPThread(const Config &config, ClientData &data, barrier &wb,
                     NetStatQueue &queue);

#endif /* _CLIENT_HPP_ */
