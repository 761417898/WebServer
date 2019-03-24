#include "TimerQueue.h"

namespace GaoServer {

TimerQueue::TimerQueue() : timerFd_(createTimerFd()), timerChannel(),
        timers_() {
    timerChannel.setFd(timerFd_);
    timerChannel.setReadCallBack(std::bind(&TimerQueue::handleRead, this));
    timerChannel.enableReading();
}

void TimerQueue::readTimerFd() {
    uint64_t buf;
    ssize_t ret = ::read(timerFd_, &buf, sizeof buf);
    if (ret != sizeof buf) {
        //LOG...
    }
}

void TimerQueue::handleRead() {
    readTimerFd();

    auto expiredTimers = getExpired();
    for (auto it = expiredTimers.begin(); it != expiredTimers.end(); ++it) {
        it->second->run();
    }
}

std::vector<std::pair<timespec, Timer*> > TimerQueue::getExpired() {
    timespec now;
    clock_gettime(CLOCK_REALTIME, &now);

    std::vector<std::pair<timespec, Timer*> > ret;
    auto it = timers_.begin();
    while (it != timers_.end() && it->first < now) {
        ret.push_back(*it);
        ++it;
    }
    auto start = timers_.begin();
    auto end = timers_.end();
    timers_.erase(start, end);

    if (it != timers_.end()) {
        timespec tim;
        tim.tv_sec = it->first.tv_sec - now.tv_sec;
        tim.tv_nsec = it->first.tv_nsec - now.tv_nsec;
        resetTime(tim);
    }

    return ret;
}

int TimerQueue::createTimerFd() {
    int fd = ::timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK | TFD_CLOEXEC);
    return fd;
}

void TimerQueue::resetTime(timespec time) {
    struct itimerspec new_value;
    new_value.it_value.tv_sec = time.tv_sec; //第一次到期的时间
    new_value.it_value.tv_nsec = time.tv_nsec;
    timerfd_settime(timerFd_, 0, &new_value, NULL);
}

void TimerQueue::addTimer(TimerCallBack cb, timespec time) {
    Timer *timer = ::new Timer(cb);

    bool earliestChanged = false;
    auto it = timers_.begin();
    if (it == timers_.end() || time < it->first) {
      earliestChanged = true;
    }
    timers_.insert(Entry(time, timer));

    if (earliestChanged) {
        auto it = timers_.begin();
        timespec tim, now;
        clock_gettime(CLOCK_REALTIME, &now);
        if (time < now) {
            //LOG << ...
        }
        tim.tv_sec = time.tv_sec - now.tv_sec;
        tim.tv_nsec = time.tv_nsec - now.tv_nsec;
        resetTime(tim);
    }
}

}
