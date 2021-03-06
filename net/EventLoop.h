#ifndef NET_EVENTLOOP_H
#define NET_EVENTLOOP_H

#include "../base/noncopyable.h"
#include "../base/Thread.h"
#include "../base/CurrentThread.h"
#include "EPoller.h"
#include "Channel.h"
#include "TimerQueue.h"
#include <cassert>
#include <vector>
#include <poll.h>
#include <memory>
#include <functional>
#include <ctime>

namespace GaoServer {

class EventLoop {
private:
    typedef std::function<void()> Functor;
    typedef std::vector<Channel*> ChannelList;
    typedef std::function<void ()> TimerCallBack;
    //是否在运行
	bool looping_;
    bool quit_;
    //运行所在的线程ID
	const pid_t threadId_;
    std::shared_ptr<EPoller> poller_;
    ChannelList activeChannels_;

    std::shared_ptr<TimerQueue> timerQueue_;

    void abortNotInLoopThread() {
    //
    }
    MutexLock mutex_;
    bool callingPendingFunctors_; /* atomic */
    std::vector<Functor> pendingFunctors_;
    void wakeup();
    void doPendingFunctors();
    //used for wakeup
    int wakeupFd_;
    std::unique_ptr<Channel> wakeupChannel_;
    void handleRead();

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
    //run after
    void addTimer(TimerCallBack cb, timespec time);

	EventLoop* getEventLoopOfCurrentThread();

    void setPoller(std::shared_ptr<EPoller> poller) {
        poller_ = poller;
    }

    void queueInLoop(Functor cb);

    void runInLoop(Functor cb);

};

}

#endif
