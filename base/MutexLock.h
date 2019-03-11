/*
 *  对互斥锁的简单封装,直接mtx.lock mtx.unlock进程操作
 *设置友元类Condition，以便Condition能拿到实际的互斥锁pthread_mutex_t
 */


#ifndef BASE_MUTEXLOCK_H
#define BASE_MUTEXLOCK_H

#include "noncopyable.h"
#include <pthread.h>
#include <cstdio>

namespace GaoServer {

class MutexLock : noncopyable {
public:
    MutexLock() {
        pthread_mutex_init(&mutex_, NULL);
    }
    ~MutexLock() {
        pthread_mutex_lock(&mutex_);
        pthread_mutex_destroy(&mutex_);
    }
    void lock() {
        pthread_mutex_lock(&mutex_);
    }
    void unlock() {
        pthread_mutex_unlock(&mutex_);
    }
    pthread_mutex_t *get() {
        return &mutex_;
    }
private:
    pthread_mutex_t mutex_;
    //友元类不受访问权限影响
    friend class Condition;
};

class MutexLockGuard : noncopyable {
private:
	MutexLock &mutex_;
public:
	explicit MutexLockGuard(MutexLock &mutex) :
	mutex_(mutex) {
		mutex_.lock();
	}
	~MutexLockGuard() {
		mutex_.unlock();
	}
}

}

#endif
