#ifndef _RUNNER_HPP_
#define _RUNNER_HPP_

#include <boost/fiber/all.hpp>

#include <client.hpp>
#include <config.hpp>

struct Client {
	struct ClientData Data;
	boost::fibers::fiber fb;
};

void runClients(const Config &config);

#endif /* _RUNNER_HPP_ */
