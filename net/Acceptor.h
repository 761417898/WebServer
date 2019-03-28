#ifndef NET_ACCEPTOR_H
#define NET_ACCEPTOR_H

#include "Socket.h"
#include "InetAddress.h"
#include "../base/noncopyable.h"
#include "Channel.h"
#include <functional>

namespace GaoServer {

class EventLoop;
class Channel;

class Acceptor : noncopyable {
public:
    typedef std::function<void (int sockfd, const InetAddress&)> NewConnectionCallBack;
    Acceptor(EventLoop* loop, const InetAddress& listenAddr);

    //for error, non-const references can't bind temp objects.
    void setNewConnectionCallBack(const NewConnectionCallBack& cb) {
        newConnectionCallBack_ = cb;
    }

    bool listenning() const {
        return listenning_;
    }
    void listen();
private:
    void handleRead();
    EventLoop* loop_;
    Socket acceptSocket_;
    Channel acceptChannel_;
    NewConnectionCallBack newConnectionCallBack_;
    bool listenning_;
};

}

#endif
