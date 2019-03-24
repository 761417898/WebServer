#ifndef NET_TIMER_H
#define NET_TIMER_H

#include <functional>

namespace GaoServer {

class Timer {

private:
    typedef std::function<void ()> TimerCallBack;

public:


    TimerCallBack cb_;

};

}

#endif
