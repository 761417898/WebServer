/*
 *    ��fd�ĳ��󣬰�����fd��ע���¼�event��kReadEvent kWriteEvent kNoneEvent����
 *  ��������event�����ú�Ҫupdateһ�£�����ʵ�ʵ�pollerע����¼�����poller��ش��ķ������¼�
 *  revent��������fd�����¼���Ҫִ�еĺ���
 *  errorCallBack_ readCallBack_ writeCallBack_(��function����)��ͨ��handleEvent������
 */


#ifndef NET_CHANNEL_H
#define NET_CHANNEL_H

#include <functional>
#include <memory>
#include <poll.h>

namespace GaoServer {

class Channel {
public:
    typedef std::function<void ()> EventCallBack;
	Channel();
    Channel(int fd) : fd_(fd) {

    }

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
    void setFd(int fd) {
        fd_ = fd;
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
	//	update();
	}
	void enableWriting() {
		events_ |= kWriteEvent;
	//	update();
	}

    void disableReading() {
        events_ &= ~kReadEvent;
    //    update();
    }

	void disableWriting() {
		events_ &= ~kWriteEvent;
	//	update();
	}
	void disableAll() {
		events_ = kNoneEvent;
	//	update();
	}

private:
    //void update();
    //void deleteChannel();

	static const int kNoneEvent;
	static const int kReadEvent;
	static const int kWriteEvent;

	int fd_;
	int events_;
    //�ش����¼�
	int revents_;

	EventCallBack readCallBack_;
	EventCallBack writeCallBack_;
	EventCallBack errorCallBack_;
};

}

#endif
