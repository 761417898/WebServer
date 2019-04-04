#include "EventLoopThreadPool.h"
#include "EventLoop.h"
#include "EventLoopThread.h"

namespace GaoServer {
    EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop, const string& nameArg)
        : baseLoop_(baseLoop), name_(nameArg), 
    {}
}

