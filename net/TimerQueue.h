#ifndef NET_TIMERQUEUE_H
#define NET_TIMERQUEUE_H

#include "Timer.h"
#include "Channel.h"
#include <set>
#include <sys/timerfd.h>
#include <unistd.h>
#include <functional>
#include <vector>
#include <ctime>

namespace GaoServer {

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
    friend class EventLoop;
};

}

#endif
