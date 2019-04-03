#include "TcpConnection.h"

namespace GaoServer {

TcpConnection::TcpConnection(EventLoop* loop,
                             const std::string& nameArg,
                             int sockfd,
                             const InetAddress& localAddr,
                             const InetAddress& peerAddr)
    : state_(kConnecting),
      loop_(loop),
      name_(nameArg),
      socket_(new Socket(sockfd)),
      channel_(new Channel(sockfd)),
      localAddr_(localAddr),
      peerAddr_(peerAddr) {
    //不能这么用，这样sharedThis和TcpServer里的shared_ptr指向同一个对象
    //但两个智能指针引用计数都是１，会造成同一对象析构两次
      //sharedThis_(this)
      channel_->setReadCallBack(
          std::bind(&TcpConnection::handleRead, this));
      channel_->setWriteCallBack(
          std::bind(&TcpConnection::handleWrite, this));
      channel_->setCloseCallBack(
          std::bind(&TcpConnection::handleClose, this));
      channel_->setErrorCallBack(
          std::bind(&TcpConnection::handleError, this));
     // LOG_DEBUG << "TcpConnection::ctor[" <<  name_ << "] at " << this
     //           << " fd=" << sockfd;*/
      socket_->setKeepAlive(true);
}

TcpConnection::~TcpConnection() {
    printf("this is ~TcpConnection\n");
}

void TcpConnection::handleRead() {
    char buf[65536];
    int savedErrno = 0;
    ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedErrno);
    if (n > 0) {
        messageCallBack_(shared_from_this(), &inputBuffer_);
        //messageCallBack_(shared_from_this(), buf, n);
    } else if (n == 0) {
        handleClose();
    } else {
        handleError();
    }

}

void TcpConnection::handleClose() {
    printf("handleclose processing\n");
    loop_->assertInLoopThread();
    assert(state_ == kConnected);
    channel_->disableAll();
    closeCallBack_(shared_from_this());
}

void TcpConnection::handleError() {

}

void TcpConnection::handleWrite() {

}

void TcpConnection::connectEstablish() {
    loop_->assertInLoopThread();
    assert(state_ == kConnecting);
    setState(kConnected);
    channel_->enableReading();
    loop_->addChannel(channel_.get());
    connectionCallBack_(shared_from_this());
}

void TcpConnection::connectDestroyed() {
    printf("this is TcpConnection::connectDestroyed\n");
    loop_->assertInLoopThread();
    assert(state_ == kConnected);
    setState(kDisConnected);
    channel_->disableAll();
    connectionCallBack_(shared_from_this());
    loop_->delChannel(channel_.get());
}

}
