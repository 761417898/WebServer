#ifndef NET_TIMERQUEUE_H
#define NET_TIMERQUEUE_H

#include "Channel.h"
#include "Timer.h"
#include <set>
#include <sys/timerfd.h>
#include <unistd.h>
#include <functional>
#include <vector>
#include <ctime>

namespace GaoServer {

bool operator<(const timespec& a, const timespec& b) {
    if (a.tv_sec == b.tv_sec)
        return a.tv_nsec < b.tv_nsec;
    return a.tv_sec < b.tv_sec;
}

class TimerQueue {

private:
    typedef std::function<void ()> TimerCallBack;

public:
    TimerQueue();
    ~TimerQueue();

    void addTimer(TimerCallBack cb, timespec time);

private:
    typedef std::pair<timespec, Timer*> Entry;
    typedef std::set<Entry> TimerList;

    int timerFd_;
    Channel timerChannel;
    TimerList timers_;

    int createTimerFd();
    void readTimerFd();
    void handleRead();
    std::vector<Entry> getExpired();

    void resetTime(timespec time);
};

}

#endif
