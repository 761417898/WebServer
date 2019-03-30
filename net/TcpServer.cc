#include "TcpServer.h"

namespace GaoServer {

TcpServer::TcpServer(EventLoop *loop, const InetAddress& listenAddr, std::string& name)
    : loop_(loop), name_(name), acceptor_(new Acceptor(loop_, listenAddr)),
      started_(false), nextConnId_(1) {
    acceptor_->setNewConnectionCallBack(
          std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2));
}

TcpServer::~TcpServer() {

}

void TcpServer::start() {
    acceptor_->listen();
}

void TcpServer::removeConnection(const TcpConnectionPtr &conn) {
    loop_->assertInLoopThread();
    connections_.erase(conn->name());
    //loop_->queueInLoop()
    conn->connectDestroyed();
}

void TcpServer::newConnection(int sockfd, const InetAddress& peerAddr) {
    loop_->assertInLoopThread();
    char buf[32];
    snprintf(buf, sizeof (buf), "#%d", nextConnId_);
    ++nextConnId_;
    std::string connName = name_ + buf;
    //LOG_INFO << "TcpServer::newConnection ..."
    printf("TcpServer::newConnection\n");
    InetAddress localAddr(Socket::getSockName(sockfd));
    TcpConnectionPtr conn(
                new TcpConnection(loop_, connName, sockfd, localAddr, peerAddr));
    connections_[connName] = conn;
    conn->setConnectionCallBack(connectionCallBack_);
    conn->setMessageCallBack(messageCallBack_);
    conn->setCloseCallBack(std::bind(&TcpServer::removeConnection, this, std::placeholders::_1));
    //conn->setCloseCallBack(removeConnection);
    conn->connectEstablish();
}

}
