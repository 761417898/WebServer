#include "TcpServer.h"

namespace GaoServer {

TcpServer::TcpServer(EventLoop *loop, const InetAddress& listenAddr, std::string& name)
    : loop_(loop), name_(name), acceptor_(new Acceptor(loop_, listenAddr)),
      started_(false), nextConnId_(1) {}

TcpServer::~TcpServer() {

}

void TcpServer::start() {
    if (!acceptor_->listenning()) {
        loop_->runInLoop(std::bind(&Acceptor::listen, *acceptor_));
    }
}

void TcpServer::newConnection(int sockfd, const InetAddress& peerAddr) {
    loop_->assertInLoopThread();
    char buf[32];
    snprintf(buf, sizeof (buf), "#%d", nextConnId_);
    ++nextConnId_;
    std::string connName = name_ + buf;
    //LOG_INFO << "TcpServer::newConnection ..."
    InetAddress localAddr(Socket::getSockName(sockfd));
    TcpConnectionPtr conn(
                new TcpConnection(loop_, connName, sockfd, localAddr, peerAddr));
    connections_[connName] = conn;
    conn->setConnectionCallBack(connectionCallBack_);
    conn->setMessageCallBack(messageCallBack_);
    conn->connectEstablish();
}

}
