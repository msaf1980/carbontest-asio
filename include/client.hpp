#ifndef _CLIENT_HPP_
#define _CLIENT_HPP_

#include <cstdlib>

#include <boost/fiber/barrier.hpp>
using boost::fibers::barrier;

#include <config.hpp>

struct ClientData {
	size_t        Id;
};

void clientTCPThread(const Config &config, ClientData &data, barrier &wb);
void clientUDPThread(const Config &config, ClientData &data, barrier &wb);

#endif /* _CLIENT_HPP_ */
