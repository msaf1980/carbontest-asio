#ifndef _RUNNER_HPP_
#define _RUNNER_HPP_

#include <client.hpp>
#include <config.hpp>
#include <boost/thread.hpp>

struct Thread {
	struct ClientData data;
	boost::thread       t;
};

void runClients(const Config &config);

#endif /* _RUNNER_HPP_ */
