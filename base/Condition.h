/*
 *   对条件变量的简单封装，使用时就cond.wait cond.signal就可以
 */


#ifndef BASE_CONDITION
#define BASE_CONDITION

#include "noncopyable.h"
#include "MutexLock.h"
#include <pthread.h>
#include <errno.h>
#include <time.h>

namespace GaoServer {

class Condition {
private:
    MutexLock &mutex_;
    pthread_cond_t cond_;
public:
    explicit Condition(MutexLock &mutex) : 
        mutex_(mutex) {
        pthread_cond_init(&cond_, NULL);
    }
    
    ~Condition() {
        pthread_cond_destroy(&cond_);
    }

    void wait() {
        pthread_cond_wait(&cond_, mutex_.get());
    }

    void notify() {
        pthread_cond_signal(&cond_);
    }

    void notifyAll() {
        pthread_cond_broadcast(&cond_);
    }

    bool waitForSeconds(int seconds) {
        struct timespec abstime;
        clock_gettime(CLOCK_REALTIME, &abstime);
        abstime.tv_sec += static_cast<time_t>(seconds);
        //设置了阻塞时间，超时返回ETIMEDOUT
        return ETIMEDOUT == pthread_cond_timedwait(&cond_, mutex_.get(), &abstime);
    }
};

}

#endif //BASE_CONDITION
