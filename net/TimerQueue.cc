#include "TimerQueue.h"

namespace GaoServer {

TimerQueue::TimerQueue() : timerFd_(createTimerFd()), timerChannel(),
        timers_() {
    timerChannel.setFd(timerFd_);
    timerChannel.setReadCallBack(handleRead);
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

    //std::vector<Entry> expired = get
}

int TimerQueue::createTimerFd() {
    int fd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    return fd;
}

}
