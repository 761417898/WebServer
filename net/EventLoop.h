#ifndef NET_EVENTLOOP_H
#define NET_EVENTLOOP_H

#include "../base/noncopyable.h"
#include "../base/Thread.h"
#include "../base/CurrentThread.h"
#include "EPoller.h"
#include <cassert>
#include <vector>
#include <poll.h>
#include <memory>

namespace GaoServer {

class EventLoop : noncopyable {
private:
	void abortNotInLoopThread();

    typedef std::vector<Channel*> ChannelList;
    //是否在运行
	bool looping_;
    bool quit_;
    //运行所在的线程ID
	const pid_t threadId_;
    shared_ptr<EPoller> poller_;
    ChannelList activeChannels_;
public:
	EventLoop();
	~EventLoop();
	
	void loop();
    //检测是否在当前进程运行，CurrentThread宏定义在../base/CurrentThread.h
	bool isInLoopThread() const {
		return threadId_ == CurrentThread::tid();
	}
	void assertInLoopThread() {
		if (!isInLoopThread()) {
			abortNotInLoopThread();
		}
	}

	EventLoop* getEventLoopOfCurrentThread();

};

}

#endif
