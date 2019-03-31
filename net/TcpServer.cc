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
    ssize_t n = connections_.erase(conn->name());
    assert(n == 1);
    //loop_->queueInLoop()
    /*
     * 　这里把connectionDestroyed加到执行队列里就可以返回了，返回到
     *　Channel HandleEvent中，这样HandleEvent函数就可以正常结束了。由于
     *　我们传递的是bind，所以该connection直到connectionDestroyed执行后
     *　才会被析构掉。
     */
    loop_->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
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
