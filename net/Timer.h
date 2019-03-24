#ifndef NET_TIMER_H
#define NET_TIMER_H

#include <functional>

namespace GaoServer {

class Timer {

private:
    typedef std::function<void ()> TimerCallBack;
    TimerCallBack cb_;

public:
    Timer(TimerCallBack cb) : cb_(cb) {}

    void run() {
        cb_();
    }
};

}

#endif
