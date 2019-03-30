#include "EventLoop.h"
#include "Acceptor.h"

namespace GaoServer {

Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddr)
    : loop_(loop), acceptSocket_(), acceptChannel_(acceptSocket_.fd()), listenning_(false) {
    acceptSocket_.setReuseAddr(true);
    acceptSocket_.bindAddress(listenAddr);
    acceptChannel_.setReadCallBack(std::bind(&Acceptor::handleRead, this));
}

void Acceptor::handleRead() {
    loop_->assertInLoopThread();
    InetAddress peerAddr(0);
    printf("%d received connect request\n", acceptSocket_.fd());
    int connfd = acceptSocket_.accept(&peerAddr);
    if (connfd > 0) {
        newConnectionCallBack_(connfd, peerAddr);
    } else {
        //LOG
        ::close(connfd);
    }
}

void Acceptor::listen() {
    loop_->assertInLoopThread();
    listenning_ = true;
    acceptSocket_.listen();
    acceptChannel_.enableReading();
    loop_->addChannel(&acceptChannel_);
}

}
