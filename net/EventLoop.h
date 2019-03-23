#ifndef NET_EVENTLOOP_H
#define NET_EVENTLOOP_H

#include "../base/noncopyable.h"
#include "../base/Thread.h"
#include "../base/CurrentThread.h"
#include "EPoller.h"
#include "Channel.h"
#include <cassert>
#include <vector>
#include <poll.h>
#include <memory>

namespace GaoServer {

class EventLoop : noncopyable {
private:

    typedef std::vector<Channel*> ChannelList;
    //是否在运行
	bool looping_;
    bool quit_;
    //运行所在的线程ID
	const pid_t threadId_;
    std::shared_ptr<EPoller> poller_;
    ChannelList activeChannels_;

    void abortNotInLoopThread() {
    //
    }

public:
	EventLoop();
	~EventLoop();

    void loop();
	
	void quit();
    //检测是否在当前进程运行，CurrentThread宏定义在../base/CurrentThread.h
	bool isInLoopThread() const {
		return threadId_ == CurrentThread::tid();
	}
	void assertInLoopThread() {
		if (!isInLoopThread()) {
			abortNotInLoopThread();
		}
	}

    void modChannel(Channel* channel);

    void addChannel(Channel* channel);

    void delChannel(Channel* channel);

	EventLoop* getEventLoopOfCurrentThread();

    void setPoller(std::shared_ptr<EPoller> poller) {
        poller_ = poller;
    }

};

}

#endif
