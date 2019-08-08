#ifndef _CLIENT_HPP_
#define _CLIENT_HPP_

#include <cstdlib>

#include <boost/asio/io_service.hpp>
#include <boost/fiber/barrier.hpp>
using boost::fibers::barrier;

#include <config.hpp>
#include <netstat.hpp>

struct ClientData {
	size_t        Id;
};

void clientTCPSession(boost::asio::io_service &io_svc, const Config &config, ClientData &data, barrier &wb, NetStatQueue &queue);
void clientUDPSession(boost::asio::io_service &io_svc, const Config &config, ClientData &data, barrier &wb, NetStatQueue &queue);

#endif /* _CLIENT_HPP_ */
