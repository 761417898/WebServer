#ifndef NET_EPOLL_H
#define NET_EPOLL_H

#include <vector>
#include <sys/epoll.h>
#include <map>
#include "Channel.h"

namespace GaoServer {

class EPoller {
private:
    typedef std::vector<epoll_event> EventList;
    //typedef std::map<int, ChannelP> ChannelMap;

    int epollfd_;
    EventList events_;
    //ChannelMap channels_;
public:
    static const int kInitEventListSize = 4096;
    static const int kEpollWaitTime = 5000;
public:
    EPoller();
    ~EPoller();

    void epollAdd(Channel* channel);
    void epollMod(Channel* channel);
    void epollDel(Channel* channel);

    void poll(std::vector<Channel*>& ret, int timeoutMs = kEpollWaitTime);
};

}

#endif
