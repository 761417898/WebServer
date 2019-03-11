#ifndef BASE_THREAD_H
#define BASE_THREAD_H
//对线程的简单封装，包含线程名，ID，要执行的函数，是否开始执行等信息
//包含一个countDown，Thread在startThread后countDown.wait阻塞，必须要等到
//紧挨着实际线程要运行的func真正执行 之前的countDown.signal才能继续执行。
//避免了Thread对象未完成执行线程的任务就析构了
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
	//这两个线程ID不是一回事，pthreadId是pthread_create创建返回的
	//tid才是真正的线程ID（用于表示task_struct，可通过syscall(SYS_gettid)获取）
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
