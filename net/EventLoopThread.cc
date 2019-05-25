#include "EventLoopThread.h"
#include "EventLoop.h"

namespace GaoServer {

EventLoopThread::EventLoopThread(const ThreadInitCallback& cb,
                                 const std::string& name)
  : loop_(),
    exiting_(false),
    thread_(std::bind(&EventLoopThread::threadFunc, this), name),
    mutex_(),
    cond_(mutex_),
    callback_(cb) {
}

EventLoopThread::~EventLoopThread() {
    exiting_ = true;
    if (loop_ != NULL) {
        loop_->quit();
        thread_.join();
    }
}

EventLoop* EventLoopThread::startLoop() {
    assert(!thread_.started());
    thread_.start();

    EventLoop* loop = NULL;
    {
        MutexLockGuard lock(mutex_);
        while (loop_ == NULL) {
            cond_.wait();
        }
        loop = loop_;
    }
    return loop;
}

void EventLoopThread::threadFunc() {
    EventLoop loop;

    if (callback_) {
        callback_(&loop);
    }

    {
        MutexLockGuard lock(mutex_);
        loop_ = &loop; //主线程通过loop的地址对其进行访问
        cond_.notify();
    }

    loop.loop();
    //assert(exiting_);
    MutexLockGuard lock(mutex_);
    loop_ = NULL;
}



}
