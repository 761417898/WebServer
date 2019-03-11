#include "Channel.h"

namespace GaoServer {
	
const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop *loop, int fdArg) :
	loop_(loop), fd_(fdArg), event_(0), revent_(0), index_(-1) {}

void Channel::update() {
	loop_->updateChannel(this);
}

void Channel::handleEvent() {
	if (revent_ & POLLNVAL) {
		//LOG_WARN <<
	}
	if (revent_ & (POLLERR | POLLNVAL)) {
		if (errorCallBack_) errorCallBack_();
	}
	if (revent_ & (POLLIN | POLLPRI | POLLRDHUP)) {
		if (readCallBack_) readCallBack_();
	}
	if (revent_ & POLLOUT) {
		if (writeCallBack_) writeCallBack_();
	}
}

}