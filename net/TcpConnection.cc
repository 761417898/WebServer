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
      peerAddr_(peerAddr),
      sharedThis(this) {
      channel_->setReadCallBack(
          std::bind(&TcpConnection::handleRead, this));

     /*channel_->setWriteCallback(
          std::bind(&TcpConnection::handleWrite, this));
      channel_->setCloseCallback(
          std::bind(&TcpConnection::handleClose, this));
      channel_->setErrorCallback(
          std::bind(&TcpConnection::handleError, this));
      LOG_DEBUG << "TcpConnection::ctor[" <<  name_ << "] at " << this
                << " fd=" << sockfd;*/
      socket_->setKeepAlive(true);
}

void TcpConnection::handleRead() {
    char buf[65536];
    ssize_t n = ::read(channel_->fd(), buf, sizeof (buf));
    if (n > 0) {
        messageCallBack_(sharedThis, buf, n);
    } else if (n == 0) {
        handleClose();
    } else {
        handleError();
    }

}

void TcpConnection::handleClose() {
    loop_->assertInLoopThread();
    assert(state_ == kConnected);
    channel_->disableAll();
    closeCallBack_(sharedThis);
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
    connectionCallBack_(sharedThis);
}

void TcpConnection::connectDestroyed() {
    loop_->assertInLoopThread();
    assert(state_ == kConnected);
    setState(kDisConnected);
    channel_->disableAll();
    connectionCallBack_(sharedThis);
    loop_->delChannel(channel_.get());
}

}
