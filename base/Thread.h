#ifndef BASE_THREAD_H
#define BASE_THREAD_H
//���̵߳ļ򵥷�װ�������߳�����ID��Ҫִ�еĺ������Ƿ�ʼִ�е���Ϣ
//����һ��countDown��Thread��startThread��countDown.wait����������Ҫ�ȵ�
//������ʵ���߳�Ҫ���е�func����ִ�� ֮ǰ��countDown.signal���ܼ���ִ�С�
//������Thread����δ���ִ���̵߳������������
#include "CountDownLatch.h"
#include "noncopyable.h"
#include <pthread.h>
#include <functional>
#include <string>
#include <sys/syscall.h>
#include <unistd.h>

namespace GaoServer {

class Thread : noncopyable {
public:
	typedef std::function<void ()> ThreadFunc;
private:
	void setDefaultName();
	bool started_;
	bool joined_;
	//�������߳�ID����һ���£�pthreadId��pthread_create�������ص�
	//tid�����������߳�ID�����ڱ�ʾtask_struct����ͨ��syscall(SYS_gettid)��ȡ��
	pthread_t pthreadID_;
	pid_t tid_;
	ThreadFunc	func_;
	std::string name_;
	CountDownLatch latch_;
public:
	explicit Thread(const ThreadFunc&, const std::string& name = std::string());
	~Thread();
	void start();
	int join();
	bool started() const{
		return started_;
	}
	pid_t tid() const{
		return tid_;
	}
	const std::string& name() const {
		return name_;
	}
};

}

#endif
