#ifndef NET_POLLER_H
#define NET_POLLER_H

#include "../base/noncopyable.h"
#include <vector>
#include <map>

namespace GaoServer {
    struct pollfd;
    
    class Channel;
    class Poller : noncopyable {
    public:
        typedef std::vector<Channel*> ChannelList;

        Poller(EventLoop* loop);
        ~Poller();

        Timestamp poll(int timeoutMs, ChannelList* activeChannels);

        void updateChannel(Channel *channel);
        void assertInLoopThread() {
            ownerLoop_->assertInLoopThread();
        }
    private:
        void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;

        typedef std::vector<struct pollfd> PollFdList;
        typedef std::map<int, Channel*> ChannelMap;

        EventLoop* ownerLoop_;
        //实际的pollfd数组
        PollFdList pollfds_;
        //fd到channel*的映射
        ChannelMap channels_;
    }

}

#endif
