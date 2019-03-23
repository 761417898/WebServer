#include "Channel.h"
#include <memory>

namespace GaoServer {
	
const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel() : events_(0), revents_(0) {}

void Channel::handleEvent() {
	if (revents_ & POLLNVAL) {
		//LOG_WARN <<
	}
	if (revents_ & (POLLERR | POLLNVAL)) {
        if (errorCallBack_)
            errorCallBack_();
	}
	if (revents_ & (POLLIN | POLLPRI | POLLRDHUP)) {
        if (readCallBack_)
            readCallBack_();
	}
	if (revents_ & POLLOUT) {
		if (writeCallBack_) writeCallBack_();
	}
}

}
