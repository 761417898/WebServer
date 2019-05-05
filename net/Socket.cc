#include "Socket.h"
#include "InetAddress.h"
#include <netinet/tcp.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstdio>

namespace GaoServer {

Socket::Socket() : sockfd_(::socket(AF_INET, SOCK_STREAM |
                                    SOCK_NONBLOCK | SOCK_CLOEXEC, 0)){
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
    ::bind(sockfd_, addr, sizeof (sockaddr));

    char buf[100];
    localaddr.toIp(buf, sizeof (buf));
    //printf("%d fd, bind IP %s, Port %d\n", sockfd_, buf, localaddr.toPortIpv6());
}

void Socket::listen() {
    int ret = ::listen(sockfd_, SOMAXCONN);
    //printf("listen return %d, %d fd listen\n", ret, sockfd_);
    //sockaddr_in addr = Socket::getSockName(sockfd_);
    //printf("listen debug: ip = %s, port = %d...\n", inet_ntoa(addr.sin_addr), htons( addr.sin_port));
}

int Socket::accept(InetAddress *peeraddr) {
    struct sockaddr_in addr;
    ::memset(&addr, 0, sizeof (addr));
    socklen_t addrlen = static_cast<socklen_t>(sizeof addr);
    //printf("%d fd accept\n", sockfd_);
    if (sockfd_ == 1031) {
        printf("wait");
    }
    int connectFd = ::accept(sockfd_, (sockaddr*)&addr, &addrlen);
    //printf("accepted a new connect fd %d\n", connectFd);
    peeraddr->setSockAddrInet(addr);

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
