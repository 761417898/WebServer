#include "EPoller.h"

namespace GaoServer {
	
	EPoller::EPoller(EventLoop* loop) : epollfd_(epoll_create1(EPOLL_CLOEXEC)), events_(kInitEventListSize), ownerLoop_(loop) {
	}

	EPoller::~EPoller() {
		::close(epollfd_);
	}

	EPoller::epollAdd(ChannelP channel) {
		assertInLoopThread();
		ChannelMap.insert(make_pair(channel->fd(), channel));

		struct eppll_event ev;
		ev.events = channel->event();
		ev.data.fd = channel->fd();

		epoll_ctl(epollfd_, EPOLL_CTL_ADD, &ev);
	}

    EPoller::epollMod(ChannelP channel) {
		assertInLoopThread();

		struct eppll_event ev;
		ev.events = channel->event();
		ev.data.fd = channel->fd();

		epoll_ctl(epollfd_, EPOLL_CTL_MOD, &ev);
	}

	EPoller::epollDel(ChannelP channel) {
		assertInLoopThread();

		struct eppll_event ev;
		ev.events = channel->event();
		ev.data.fd = channel->fd();

		epoll_ctl(epollfd_, EPOLL_CTL_DEL, &ev);
	}

	std::vector<ChannelP> EPoller::poll(int timeoutMs = kEpollWaitTime) {
		assertInLoopThread();
		std::vector<ChannelP> ret;

		int nfds = epoll_wait(epollfd_, &*events_.begin(), static_cast<int>(events_.begin()), timeoutMs);

		for (int idx = 0; idx < nfds; ++idx) {
			epoll_event ev = events_[idx];
			ChannelP channel = ChannelMap.find(ev.data.fd)->second;
			channel->setRevents(ev.events);
			ret.push_back(channel);
		}

		return ret;
	}

}
