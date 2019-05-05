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

    bool waitForSeconds(int seconds);
};

}

#endif //BASE_CONDITION
