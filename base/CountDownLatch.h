#ifndef BASE_COUNTDOWNLATCH_H
#define BASE_COUNTDOWNLATCH_H

#include "Condition.h"
#include "MutexLock.h"
#include "noncopyable.h"

//门闸,确保func真正得到了执行，外层Thread的start才会返回

namespace GaoServer {

class CountDownLatch : noncopyable {
private:
	//const 函数中可以改变的属性
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