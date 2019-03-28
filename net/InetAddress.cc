#include <arpa/inet.h>
#include "InetAddress.h"
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>

namespace GaoServer {

InetAddress::InetAddress(uint16_t port, bool loopbackOnly, bool ipv6) {
    if (ipv6) {
        memset(&addr6_, 0, sizeof (addr6_));
        addr6_.sin6_family = AF_INET6;
        in6_addr ip = loopbackOnly ? in6addr_loopback : in6addr_any;
        addr6_.sin6_addr = ip;
        addr6_.sin6_port = htons(port);
    } else {
        memset(&addr_, 0, sizeof (addr_));
        addr_.sin_family = AF_INET;
        in_addr_t ip = loopbackOnly ? INADDR_LOOPBACK : INADDR_ANY;
        addr_.sin_addr.s_addr = htonl(ip);
        addr_.sin_port = htons(port);
    }
}

InetAddress::InetAddress(const std::string ip, uint16_t port, bool ipv6) {

}

}

