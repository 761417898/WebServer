#ifndef NET_TIMERQUEUE_H
#define NET_TIMERQUEUE_H

#include "Channel.h"
#include "Timer.h"
#include <set>
#include <sys/timerfd.h>
#include <unistd.h>
#include <functional>

namespace GaoServer {

class TimerQueue {

private:
    typedef std::function<void ()> TimerCallBack;

public:
    TimerQueue();
    ~TimerQueue();

    void addTimer(TimerCallBack cb, int time);

private:
    typedef std::pair<int, Timer*> Entry;
    typedef std::set<Entry> TimerList;

    int timerFd_;
    Channel timerChannel;
    TimerList timers_;

    int createTimerFd();
    static void readTimerFd();
    static void handleRead();
};

}

#endif
