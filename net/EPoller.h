#ifndef NET_EPOLL_H
#define NET_EPOLL_H

#include <vector>
#include <map>
#include <sys/epoll.h>
#include "Channel.h"

namespace GaoServer {

class EventLoop;

class EPoller {
private:
    typedef std::vector<epoll_event> EventList;
    typedef std::map<int, ChannelP> ChannelMap;
    int epollfd_;
    EventList events_;
    ChannelMap channels_;

    EventLoop *ownerLoop_;
public:
    static const int kInitEventListSize = 4096;
    static const int kEpollWaitTime = 10000;
public:
    EPoller(EventLoop* loop);
    ~EPoller();

public:
    void epollAdd(ChannelP channel);
    void epollMod(ChannelP channel);
    void epollDel(ChannelP channel);
public:
    void updateChannel(ChannelP channel);
    std::vector<ChannelP> poll(int timeoutMs = kEpollWaitTime); 

    void assertInLoopThread();
};

}

#endif
