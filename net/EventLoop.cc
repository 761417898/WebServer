#include "EventLoop.h"
#include "TimerQueue.h"
#include <cassert>
#include <cstdio>
#include <pthread.h>
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
    timerQueue_ = std::make_shared<TimerQueue>();
    poller_ = std::make_shared<EPoller>();
    addChannel(&(timerQueue_->timerChannel));
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
            printf("\n%d fd is actived\n", (*it)->fd());
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
    printf("fd %d added\n", channel->fd());
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

void EventLoop::addTimer(TimerCallBack cb, timespec time) {
    assertInLoopThread();
    timerQueue_->addTimer(cb, time);
}

}
