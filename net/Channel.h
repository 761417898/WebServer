/*
 *    对fd的抽象，包括该fd关注的事件event（kReadEvent kWriteEvent kNoneEvent用于
 *  方便设置event，设置后要update一下，更新实际的poller注册的事件），poller后回传的发生的事件
 *  revent，还包括fd发生事件后要执行的函数
 *  errorCallBack_ readCallBack_ writeCallBack_(由function定义)，通过handleEvent来调用
 */


#ifndef NET_CHANNEL_H
#define NET_CHANNEL_H

#include <functional.h>

namespace GaoServer {

class EventLoop;

class Channel {
public:
	typedef function<void ()> EventCallBack;
	Channel(EventLoop* loop, int fd);
	void handleEvent();
	void setReadCallBack(const EventCallBack &cb) {
		readCallBack_ = cb;
	}
	void setWriteCallBack(const EventCallBack &cb) {
		writeCallBack_ = cb;
	}
	void setErrorCallBack(const EventCallBack &cb) {
		errorCallBack_ = cb;
	}
	int fd() const {
		return fd_;
	}
	int events() const {
		return events_;
	}
	void setRevents(int reve) {
		revents_ = reve;
	}
	bool isNoneEvent() const {
		return events_ == kNoneEvent;
	}
	void enableReading() {
		events_ |= kReadEvent;
		update();
	}
	void enableWriting() {
		events_ |= kWriteEvent;
		update();
	}
	void disableWriting() {
		events_ &= ~kWriteEvent;
		update();
	}
	void disableAll() {
		events_ = kNoneEvent;
		update();
	}

	//for poller
	int index() {
		return index_;
	}
	void setIndex(int index) {
		index_ = index;
	}
	EventLoop *ownerLoop() {
		return loop_;
	}
private:
	void update();
	
	static const int kNoneEvent;
	static const int kReadEvent;
	static const int kWriteEvent;

	EventLoop *loop_;
	const int fd_;
	int events_;
    //回传的事件
	int revents_;
	int index_;

	EventCallBack readCallBack_;
	EventCallBack writeCallBack_;
	EventCallBack errorCallBack_;
};

}

#endif
