#include "Socket.h"
#include "InetAddress.h"
#include <netinet/tcp.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <sys/socket.h>

namespace GaoServer {

Socket::Socket() : sockfd_(::socket(AF_INET, SOCK_STREAM |
                                    SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP)){
}

Socket::~Socket() {
    ::close(sockfd_);
}

bool Socket::getTcpInfo(struct tcp_info* tcpi) const
{
    socklen_t len = sizeof(*tcpi);
    ::memset(tcpi, 0, len);
    return ::getsockopt(sockfd_, SOL_TCP, TCP_INFO, tcpi, &len) == 0;
}

void Socket::bindAddress(const InetAddress& localaddr) {
    auto addr = localaddr.getSockAddr();
    ::bind(sockfd_, addr, sizeof (addr));
}

void Socket::listen() {
    ::listen(sockfd_, SOMAXCONN);
}

int Socket::accept(InetAddress *peeraddr) {
    struct sockaddr_in6 addr;
    ::memset(&addr, 0, sizeof (addr));
    socklen_t addrlen = static_cast<socklen_t>(sizeof addr);
    int connectFd = ::accept(sockfd_, (sockaddr*)&addr, &addrlen);

    peeraddr->setSockAddrInet6(addr);

    ::fcntl(connectFd,F_SETFL,::fcntl(connectFd, F_GETFL)|O_NONBLOCK|FD_CLOEXEC);
    return connectFd;
}

void Socket::shutdownWrite() {
    if (::shutdown(sockfd_, SHUT_WR) < 0)
    {
    //    LOG_SYSERR << "sockets::shutdownWrite";
    }
}

void Socket::setTcpNoDelay(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY,
                 &optval, static_cast<socklen_t>(sizeof optval));
}

void Socket::setReuseAddr(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR,
                 &optval, static_cast<socklen_t>(sizeof optval));
}

void Socket::setReusePort(bool on)
{
#ifdef SO_REUSEPORT
    int optval = on ? 1 : 0;
    int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT,
                           &optval, static_cast<socklen_t>(sizeof optval));
    if (ret < 0 && on)
    {
      //LOG_SYSERR << "SO_REUSEPORT failed.";
    }
#else
    if (on)
    {
      //LOG_ERROR << "SO_REUSEPORT is not supported.";
    }
#endif
}

void Socket::setKeepAlive(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE,
                 &optval, static_cast<socklen_t>(sizeof optval));
}

}
