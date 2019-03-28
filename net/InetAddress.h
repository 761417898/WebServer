#ifndef NET_INETADDRESS_H
#define NET_INETADDRESS_H

#include <stdint.h>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

namespace GaoServer {

class InetAddress
{
public:
    explicit InetAddress(uint16_t port = 0, bool loopbackOnly = false, bool ipv6 = false);
    InetAddress(const std::string ip, uint16_t port, bool ipv6 = false);

    explicit InetAddress(const struct sockaddr_in& addr)
        : addr_(addr)
    { }

    explicit InetAddress(const struct sockaddr_in6& addr)
        : addr6_(addr)
    { }

    void setSockAddrInet6(sockaddr_in6 addr6) {
        addr6_ = addr6;
    }

    void setSockAddrInet(sockaddr_in addr) {
        addr_ = addr;
    }

    const struct sockaddr* getSockAddr() const {
        return (const struct sockaddr*)&addr_;
    }

    const struct sockaddr_in6* getSockAddrIn6() const {
        return &addr6_;
    }

    const struct sockaddr_in* getSockAddrIn() const {
        return &addr_;
    }

    void toIp(char* buf, size_t size) const;
    void toIpv6(char *buf, size_t size) const;

    int toPort() const;
    int toPortIpv6() const;

private:
    union {
        struct sockaddr_in addr_;
        struct sockaddr_in6 addr6_;
    };
};

}

#endif
