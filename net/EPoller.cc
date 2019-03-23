#include "EPoller.h"
#include <memory>
#include <unistd.h>
#include <cstdio>

namespace GaoServer {
	
    EPoller::EPoller() : epollfd_(epoll_create1(EPOLL_CLOEXEC)), events_(kInitEventListSize) {
	}

	EPoller::~EPoller() {
		::close(epollfd_);
	}

    void EPoller::epollAdd(Channel* channel) {
		struct epoll_event ev;
		ev.events = channel->events();
        ev.data.ptr = channel;
		epoll_ctl(epollfd_, EPOLL_CTL_ADD, channel->fd(), &ev);
	}

    void EPoller::epollMod(Channel* channel) {
		struct epoll_event ev;
		ev.events = channel->events();
        ev.data.ptr = channel;
		epoll_ctl(epollfd_, EPOLL_CTL_MOD, channel->fd(), &ev);
	}

    void EPoller::epollDel(Channel* channel) {
		struct epoll_event ev;
		ev.events = channel->events();
        ev.data.ptr = channel;
		epoll_ctl(epollfd_, EPOLL_CTL_DEL, channel->fd(), &ev);;
	}

    void EPoller::poll(std::vector<Channel*>& ret, int timeoutMs) {
		int nfds = epoll_wait(epollfd_, &*events_.begin(), static_cast<int>(events_.size()), timeoutMs);

		for (int idx = 0; idx < nfds; ++idx) {
			epoll_event ev = events_[idx];
            Channel* channel = (Channel*)ev.data.ptr;
  //          printf("\n%d\n", ev.data.fd);
			channel->setRevents(ev.events);
			ret.push_back(channel);
		}
	}

}
