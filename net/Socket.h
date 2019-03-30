#ifndef NET_SOCKET_H
#define NET_SOCKET_H

#include "../base/noncopyable.h"
#include <sys/socket.h>
#include <stdint.h>
#include <string.h>
#include <netinet/in.h>

struct tcp_info;

namespace GaoServer {

class InetAddress;

class Socket : noncopyable
{
public:
    explicit Socket(int sockfd) : sockfd_(sockfd) {
    }

    explicit Socket();
    
    ~Socket();

    int fd() const {
        return sockfd_;
    }

    bool getTcpInfo(struct tcp_info*) const;

    void bindAddress(const InetAddress& localaddr);

    static sockaddr_in getSockName(int sockfd) {
        sockaddr_in localAddr;
        memset(&localAddr, 0, sizeof (localAddr));
        socklen_t addrLen = static_cast<socklen_t>(sizeof localAddr);
        if (::getsockname(sockfd, (sockaddr*)(&localAddr), &addrLen) < 0) {
        //    LOG_SYSERR << "sockets::getLocalAddr";
        }
        return localAddr;
    }

    void listen();

    int accept(InetAddress* peeraddr);

    void shutdownWrite();

    void setTcpNoDelay(bool on);
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void setKeepAlive(bool on);
private:
    const int sockfd_;
};

}

#endif
