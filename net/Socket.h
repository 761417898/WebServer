#ifndef NET_SOCKET_H
#define NET_SOCKET_H

#include "../base/noncopyable.h"

struct tcp_info;

namespace GaoServer {

class InetAddress;

class Socket : noncopyable
{
public:
    explicit Socket(int sockfd) : sockfd_(sockfd) {
    }

    int fd() const {
        return sockfd_;
    }

    bool getTcpInfo(struct tcp_info*) const;

    void bindAddress(const InetAddress& localaddr);

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
