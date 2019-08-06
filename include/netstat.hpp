#ifndef _STAT_HPP_
#define _STAT_HPP_

#include <cstdlib>
#include <cstdint>

#include <blockingconcurrentqueue.h>

enum NetOper { CONNECT, SEND, RECV };

enum NetErr { OK, ERROR, NET_EOF, TIMEOUT, LOOKUP, REFUSED, RESET };

enum NetProto { TCP, UDP };

// Network operation statistic
struct NetStat {
	int Id;
	NetProto Proto;
	NetOper Type;
	uint64_t TimeStamp;
	uint64_t Elapsed;
	NetErr Error;
	size_t Size;
};

struct QueuePapam: moodycamel::ConcurrentQueueDefaultTraits {
	static const size_t MAX_SUBQUEUE_SIZE = 10000000;
};

typedef moodycamel::BlockingConcurrentQueue<NetStat, QueuePapam> NetStatQueue;

#endif /* _STAT_HPP_ */
