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
    poller_ = std::make_shared<EPoller>();
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
        poller_->poll(activeChannels_);
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

void EventLoop::addChannel(Channel* channel) {
    assertInLoopThread();
    poller_->epollAdd(channel);
}

void EventLoop::modChannel(Channel* channel) {
    assertInLoopThread();
    poller_->epollMod(channel);
}

void EventLoop::delChannel(Channel* channel) {
    assertInLoopThread();
    poller_->epollDel(channel);
}

}
