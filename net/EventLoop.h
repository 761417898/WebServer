#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "../base/noncopyable.h"
#include "../base/Thread.h"
#include "../base/CurrentThread.h"
#include <cassert>
#include <poll.h>

namespace GaoServer {

class EventLoop : noncopyable {
private:
	void abortNotInLoopThread();
    //是否在运行
	bool looping_;
    //运行所在的线程ID
	const pid_t threadId_;
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
