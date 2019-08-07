#ifndef _CLIENT_HPP_
#define _CLIENT_HPP_

#include <cstdlib>

#include <boost/fiber/barrier.hpp>
using boost::fibers::barrier;

#include <config.hpp>
#include <netstat.hpp>

struct ClientData {
	size_t        Id;
};

void clientTCPThread(const Config &config, ClientData &data, barrier &wb, NetStatQueue &queue);
void clientUDPThread(const Config &config, ClientData &data, barrier &wb, NetStatQueue &queue);

#endif /* _CLIENT_HPP_ */
