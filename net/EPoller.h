#ifndef NET_EPOLL_H
#define NET_EPOLL_H

#include "EventLoop.h"
#include <vector>
#include <map>
#include <sys/epoll.h>
#include "Channel.h"

namespace GaoServer {

class EPoller {
private:
    typedef std::vector<struct epoll_event> EventList;
    typedef std::map<int, ChannelP> ChannelMap;
    int epollfd_;
    EventList events_;
    ChannelMap channels_;

    EventLoop *ownerLoop_;

    static const int kInitEventListSize = 4096;
    static const int kEpollWaitTime = 10000;
public:
    EPoller(EventLoop* loop);
    ~EPoller();

    void epollAdd(ChannelP channel);
    void epollMod(ChannelP channel);
    void epollDel(ChannelP channel);

    std::vector<ChannelP> poll(); 

    void assertInLoopThread() {
        ownerLoop_->assertInLoopThread();
    }

}

}

#endif
