#ifndef NET_EVENTLOOPTHREADPOOL_H
#define NET_EVENTLOOPTHREADPOOL_H

#include <functional>
#include <memory>
#include <vector>
#include "../base/noncopyable.h"

namespace GaoServer {

<<<<<<< HEAD
class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : noncopyable {
public:
    typedef std::function<void(EventLoop*)> ThreadInitCallback;

    EventLoopThreadPool(EventLoop* baseLoop, const std::string& nameArg);
    ~EventLoopThreadPool();
    void setThreadNum(int numThreads) { numThreads_ = numThreads; }
    void start(const ThreadInitCallback& cb = ThreadInitCallback());

    EventLoop* getNextLoop();
    EventLoop* getLoopForHash(size_t hashCode);

    std::vector<EventLoop*> getAllLoops();

    bool started() const {
        return started_;
    }

    const std::string& name() const {
        return name_;
    }

private:
    EventLoop* baseLoop_;
    std::string name_;
    bool started_;
    int numThreads_;
    //next thread idx
    int next_;
    std::vector<std::unique_ptr<EventLoopThread> > threads_;
    std::vector<EventLoop*> loops_;

};

}
=======
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
>>>>>>> 7ece46e59ca884e366f7d7f4b64021a1b0ac4811

#endif
