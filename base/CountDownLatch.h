#ifndef BASE_COUNTDOWNLATCH_H
#define BASE_COUNTDOWNLATCH_H

#include "Condition.h"
#include "MutexLock.h"
#include "noncopyable.h"

namespace GaoServer {

class CountDownLatch {
private:
	//const
	mutable MutexLock mutex_;
	Condition cond_;
	int count_;
public:
	explicit CountDownLatch(int count);
	void wait();
	void countDown();
};

}

#endif
