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
    messageCallBack_(sharedThis, buf, n);
}

}
