#ifndef CHANNEL_H
#define CHANNEL_H

#include <functional.h>

/*
 *	��pollId�ķ�װ
 *
 */
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
	int revents_;
	int index_;

	EventCallBack readCallBack_;
	EventCallBack writeCallBack_;
	EventCallBack errorCallBack_;
};

}

#endif