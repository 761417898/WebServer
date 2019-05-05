#include "CountDownLatch.h"
#include "Condition.h"
#include "MutexLock.h"

namespace GaoServer {

CountDownLatch::CountDownLatch(int count) :
	mutex_(), cond_(mutex_), count_(count) {
	
}

void CountDownLatch::wait() {
	MutexLockGuard lock(mutex_);
	while (count_ > 0) {
		cond_.wait();
	}
}

void CountDownLatch::countDown() {
    //MutexLock
	MutexLockGuard lock(mutex_);
	--count_;
	if (count_ == 0) {
		cond_.notifyAll();
	}
}

}
