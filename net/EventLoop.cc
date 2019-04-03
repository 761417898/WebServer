#include "EventLoop.h"
#include "TimerQueue.h"
#include <cassert>
#include <cstdio>
#include <pthread.h>
#include <sys/eventfd.h>
//#include "base/Logging.h"

namespace GaoServer {

__thread EventLoop* t_loopInThisThread = NULL;

int createEventfd()
{
  int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  return evtfd;
}

EventLoop::EventLoop() :
    looping_(false), quit_(false), threadId_(CurrentThread::tid()),
    poller_(new EPoller()), timerQueue_(new TimerQueue()), callingPendingFunctors_(false),
    wakeupFd_(createEventfd()), wakeupChannel_(new Channel(wakeupFd_)) {
	//LOG_TRACE << "EventLoop create " << this << " in thread " << threadId_;
	if (t_loopInThisThread) {
		//LOG_FATAL
	} else {
		t_loopInThisThread = this;
	}
    printf("timerQueue fd ");
    addChannel(&(timerQueue_->timerChannel));
    wakeupChannel_->setReadCallBack(
          std::bind(&EventLoop::handleRead, this));
    wakeupChannel_->enableReading();
    printf("wakeup fd ");
    addChannel(wakeupChannel_.get());
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
    printf("\n......loopping.....\n");
    while (!quit_) {
        activeChannels_.clear();
        poller_->poll(activeChannels_);
        for (ChannelList::iterator it = activeChannels_.begin();
            it != activeChannels_.end(); ++it) {
                printf("\n%d fd is actived, handling event:\n", (*it)->fd());
                (*it)->handleEvent();

        }
        doPendingFunctors();
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

void EventLoop::wakeup() {
    uint64_t one = 1;
    ssize_t n = write(wakeupFd_, &one, sizeof one);
}

void EventLoop::handleRead()
{
  uint64_t one = 1;
  ssize_t n = ::read(wakeupFd_, &one, sizeof one);
}

void EventLoop::runInLoop(Functor cb) {
    if (isInLoopThread()) {
        cb();
    } else {
        queueInLoop(std::move(cb));
    }
}

void EventLoop::queueInLoop(Functor cb) {
    {
    MutexLockGuard lock(mutex_);
    pendingFunctors_.push_back(std::move(cb));
    }
    if (!isInLoopThread() || callingPendingFunctors_) {
        wakeup();
    }
}

void EventLoop::doPendingFunctors() {
    std::vector<Functor> functors;
    callingPendingFunctors_ = true;
    {
    MutexLockGuard lock(mutex_);
    functors.swap(pendingFunctors_);
    }

    for (const Functor& functor : functors)
    {
        printf("this is pending func\n");
        functor();
    }
    callingPendingFunctors_ = false;
}


}
