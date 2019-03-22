#include "EPoller.h"
#include "EventLoop.h"
#include <memory>
#include <cstdio>

namespace GaoServer {
	
	EPoller::EPoller(EventLoop* loop) : epollfd_(epoll_create1(EPOLL_CLOEXEC)), events_(kInitEventListSize), ownerLoop_(loop) {
	}

	EPoller::~EPoller() {
        printf("  析构EPOLL  \n\n");
		::close(epollfd_);
        printf("  正常关闭  \n");
	}

	void EPoller::epollAdd(ChannelP channel) {
		assertInLoopThread();
        channels_.insert(std::pair<int, ChannelP>(channel->fd(), channel));
        auto it = channels_.find(channel->fd());
        printf("%d   插入成功\n", it->first);
		struct epoll_event ev;
		ev.events = channel->events();
		ev.data.fd = channel->fd();

		epoll_ctl(epollfd_, EPOLL_CTL_ADD, channel->fd(), &ev);
	}

    void EPoller::epollMod(ChannelP channel) {
		assertInLoopThread();

		struct epoll_event ev;
		ev.events = channel->events();
		ev.data.fd = channel->fd();

		epoll_ctl(epollfd_, EPOLL_CTL_MOD, channel->fd(), &ev);
	}

	void EPoller::epollDel(ChannelP channel) {
		assertInLoopThread();

		struct epoll_event ev;
		ev.events = channel->events();
		ev.data.fd = channel->fd();

		epoll_ctl(epollfd_, EPOLL_CTL_DEL, channel->fd(), &ev);
    
        auto it = channels_.find(channel->fd());
//        printf("%d  shanchu成功\n", it->first);
        channels_.erase(channel->fd());
	}

    void EPoller::updateChannel(ChannelP channel) {
        if (channels_.find(channel->fd()) != channels_.end()) {
            epollMod(channel);
        } else {
            epollAdd(channel);
        }
    }

	std::vector<ChannelP> EPoller::poll(int timeoutMs) {
		assertInLoopThread();
		std::vector<ChannelP> ret;

		int nfds = epoll_wait(epollfd_, &*events_.begin(), static_cast<int>(events_.size()), timeoutMs);

		for (int idx = 0; idx < nfds; ++idx) {
			epoll_event ev = events_[idx];
			ChannelP channel = channels_.find(ev.data.fd)->second;
			channel->setRevents(ev.events);
			ret.push_back(channel);
		}

		return ret;
	}

    void EPoller::assertInLoopThread() {
        ownerLoop_->assertInLoopThread();
    }

}
