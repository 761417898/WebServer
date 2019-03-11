#ifndef BASE_COUNTDOWNLATCH_H
#define BASE_COUNTDOWNLATCH_H

#include "Condition.h"
#include "MutexLock.h"
#include "noncopyable.h"

//��բ,ȷ��func�����õ���ִ�У����Thread��start�Ż᷵��

namespace GaoServer {

class CountDownLatch : noncopyable {
private:
	//const �����п��Ըı������
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