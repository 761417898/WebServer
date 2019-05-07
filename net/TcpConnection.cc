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
    //printf("this is ~TcpConnection\n");
}

void TcpConnection::handleRead() {
    char buf[65536];
    int savedErrno = 0;
    //ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedErrno);
    ssize_t n = read(channel_->fd(), buf, sizeof (buf));
    if (n > 0) {
        //messageCallBack_(shared_from_this(), &inputBuffer_);
        messageCallBack_(shared_from_this(), buf, n);
    } else if (n == 0) {
        n = read(channel_->fd(), buf, sizeof (buf));
        handleClose();
    } else {
        handleError();
    }

}

void TcpConnection::handleClose() {
    //printf("handleclose processing\n");
    loop_->assertInLoopThread();
    assert(state_ == kConnected || state_ == kDisConnecting);
    setState(kDisConnecting);
    channel_->disableAll();
    loop_->modChannel(channel_.get());
    printf("This is %s handleclose\n", name().c_str());
    closeCallBack_(shared_from_this());
}

void TcpConnection::handleError() {

}

void TcpConnection::handleWrite() {
    loop_->assertInLoopThread();
    if (channel_->isWriting()) {
        ssize_t n = ::write(channel_->fd(), outputBuffer_.peek(), outputBuffer_.readableBytes());
        if (n > 0) {
            outputBuffer_.retrieve(n);
            channel_->disableWriting();
            if (outputBuffer_.readableBytes() == 0) {
                channel_->disableWriting();
                loop_->modChannel(channel_.get());
                if (state_ == kDisConnecting) {
                    shutDownInLoop();
                }
            }
        }
    }
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
    //printf("this is TcpConnection::connectDestroyed\n");
    loop_->assertInLoopThread();
    //if (!(state_ == kConnected || state_ == kDisConnecting))
    //    return;
    assert(state_ == kConnected || state_ == kDisConnecting);
    setState(kDisConnected);
   // channel_->disableAll();
   // loop_->modChannel(channel_.get());
    connectionCallBack_(shared_from_this());
    loop_->delChannel(channel_.get());
    ::close(channel_->fd());
    printf("%s , socket: %d has been closed and deleted\n", name().c_str(), channel_->fd());
}

void TcpConnection::shutdown() {
    if (state_ == kConnected) {
        setState(kDisConnecting);
        loop_->runInLoop(std::bind(&TcpConnection::shutDownInLoop, this));
    }
}

void TcpConnection::shutDownInLoop() {
    loop_->assertInLoopThread();
    if (!channel_->isWriting()) {
        socket_->shutdownWrite();
    }
}

void TcpConnection::send(const std::string &message) {
    if (state_ == kConnected) {
        if (loop_->isInLoopThread()) {
            sendInLoop(message);
        } else {
            loop_->runInLoop(
                        std::bind(&TcpConnection::sendInLoop, this, message));
        }
    }
}

void TcpConnection::sendInLoop(const std::string &message) {
    loop_->assertInLoopThread();
    ssize_t nwrote = 0;
    if (!channel_->isWriting() && outputBuffer_.readableBytes() == 0) {
        nwrote = ::write(channel_->fd(), message.data(), message.size());
        if (nwrote >= 0) {
            if (static_cast<size_t>(nwrote) < message.size()) {

            }
        } else {
                nwrote = 0;
        }
    }
    if (static_cast<size_t>(nwrote) < message.size()) {
        outputBuffer_.append(message.data() + nwrote, message.size() - nwrote);
        if (!channel_->isWriting()) {
            channel_->enableWriting();
            loop_->modChannel(channel_.get());
        }
    }
}

}
