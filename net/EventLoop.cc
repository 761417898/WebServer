#include "EventLoop.h"
#include <cassert>
//#include "base/Logging.h"

namespace GaoServer {

__thread EventLoop* t_loopInThisThread = NULL;

EventLoop::EventLoop() :
	looping_(false), threadId_(CurrentThread::tid()) {
	//LOG_TRACE << "EventLoop create " << this << " in thread " << threadId_;
	if (t_loopInThisThread) {
		//LOG_FATAL
	} else {
		t_loopInThisThread = this;
	}
}

EventLoop::~EventLoop() {
	assert(!looping_);
	t_loopInThisThread = NULL;
}

EventLoop* EventThread::getEventLoopOfCurrentThread() {
	return t_loopInThisThread;
}

void EventLoop::loop() {
	assert(!looping_);
	assertInLoopThread();
	looping_ = true;

	::poll(NULL, 0, 5*1000);

	//LOG_TRACE << "EventLoop " << this << " stop looping";
	looping_ = false;
}

}
