#ifndef NET_EVENTLOOPTHREADPOOL_H
#define NET_EVENTLOOPTHREADPOOL_H

#include <functional>
#include <memory>
#include <vector>
#include "../base/noncopyable.h"

namespace GaoServer {

    class EventLoop;
    class EventLoopThread;

    class EventLoopThreadPool : noncopyable {
        public:
        typedef std::function<void (EventLoop*)> ThreadInitCallBack;

        EventLoopThreadPool(EventLoop *baseLoop, const string& nameArg);
        ~EventLoopThreadPool();
        void setThreadNum(int numThreads) {
            numThreads_ = numThreads;
        }
        void start(const ThreadInitCallBack &cb = ThreadInitCallBack());

        EventLoop* getNextLoop();
        EventLoop* getLoopForHash(size_t hashCode);
        std::vector<EventLoop*> getAllLoops();

        bool started() const {
            return started_;
        }

        const string &name() const {
            return name_;
        }

        private:
        int numThreads_;
        string name_;
        bool started_;
        int next_;
        std::vector<std::unique_ptr<EventLoopThread> > threads_;
        std::vector<EventLoop*> loops_;
    }


} //namespace GaoServer

#endif
