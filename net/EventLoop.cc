#include "EventLoop.h"
#include <cassert>
#include <cstdio>
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
    poller_ = std::make_shared<EPoller>(this);
}

EventLoop::~EventLoop() {
	assert(!looping_);
	t_loopInThisThread = NULL;
}

EventLoop* EventLoop::getEventLoopOfCurrentThread() {
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
        for (ChannelList::iterator it = activeChannels_.begin();
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

void EventLoop::updateChannel(ChannelP channel) {
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    poller_->updateChannel(channel);
}

void EventLoop::deleteChannel(ChannelP channel) {
    poller_->epollDel(channel);
}

}
