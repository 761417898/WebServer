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
    quit_ = false;

    while (!quit_) {
        activeChannels_.clear();
        activeChannels_ = poller_->poll();
        for (ChannelList::iteartor it = activeChannels_.begin();
            it != activeChannels_.end(); ++it) {
                (*it)->handleEvent();
        }
    }
	//LOG_TRACE << "EventLoop " << this << " stop looping";
	looping_ = false;
}

void EventLoop::quit() {
    quit_ = true;
    //wakeup
}

void EventLoop::updateChannel(Channel* channel) {
    assert(channel->ownerloop() == this);
    assertInLoopThread();
    poller_->updateChannel(channel);
}

}
