#ifndef _STAT_HPP_
#define _STAT_HPP_

#include <cstdint>
#include <cstdlib>

#include <chrono>

#include <blockingconcurrentqueue.h>

enum NetOper { CONNECT, SEND, RECV }; // update NetiOperStr after change this
extern const char * NetOperStr[];

enum NetErr {
	OK = 0,
	ERROR,
	LOOKUP,
	PIPE,
	TIMEOUT,
	REFUSED,
	RESET,
	UNREACHEABLE
}; // update NetErrStr after change this
extern const char * NetErrStr[];

enum NetProto { TCP = 0, UDP }; // update NetProtoStr after change this
extern const char * NetProtoStr[];

// Network operation statistic
struct NetStat {
	int Id;
	NetProto Proto;
	NetOper Type;
	uint64_t TimeStamp; // timestamp with milliseconds
	uint64_t Elapsed;   // microseconds
	NetErr Error;
	size_t Size;
};

struct QueuePapam : moodycamel::ConcurrentQueueDefaultTraits {
	static const size_t MAX_SUBQUEUE_SIZE = 10000000;
};

typedef moodycamel::BlockingConcurrentQueue<NetStat, QueuePapam> NetStatQueue;

typedef std::chrono::time_point<std::chrono::high_resolution_clock> chrono_clock;

#define TIME_NOW std::chrono::high_resolution_clock::now()

#endif /* _STAT_HPP_ */
