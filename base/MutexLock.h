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

class MutexLock {
public:
    MutexLock();
    ~MutexLock();
    void lock();
    void unlock();
    pthread_mutex_t *get();
private:
    pthread_mutex_t mutex_;
    //友元类不受访问权限影响
    friend class Condition;
};

class MutexLockGuard {
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
};

}

#endif
