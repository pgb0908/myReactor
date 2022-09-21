//
// Created by bong on 22. 9. 21.
//

#include <cstring>
#include "INETAddr.h"

static const in_addr_t kInaddrAny = INADDR_ANY;
static const in_addr_t kInaddrLoopback = INADDR_LOOPBACK;

INETAddr::INETAddr(uint16_t port, bool loopbackOnly, bool ipv6) : isIpV6_(ipv6){
    if (ipv6)
    {
        memset(&addr6_, 0, sizeof(addr6_));
        addr6_.sin6_family = AF_INET6;
        in6_addr ip = loopbackOnly ? in6addr_loopback : in6addr_any;
        addr6_.sin6_addr = ip;
        addr6_.sin6_port = htons(port);
    }
    else
    {
        memset(&addr_, 0, sizeof(addr_));
        addr_.sin_family = AF_INET;
        in_addr_t ip = loopbackOnly ? kInaddrLoopback : kInaddrAny;
        addr_.sin_addr.s_addr = htonl(ip);
        addr_.sin_port = htons(port);
    }
    isUnspecified_ = false;
}

INETAddr::INETAddr(const std::string &ip, uint16_t port, bool ipv6) : isIpV6_(ipv6){
    if (ipv6)
    {
        memset(&addr6_, 0, sizeof(addr6_));
        addr6_.sin6_family = AF_INET6;
        addr6_.sin6_port = htons(port);
        if (::inet_pton(AF_INET6, ip.c_str(), &addr6_.sin6_addr) <= 0)
        {
            return;
        }
    }
    else
    {
        memset(&addr_, 0, sizeof(addr_));
        addr_.sin_family = AF_INET;
        addr_.sin_port = htons(port);
        if (::inet_pton(AF_INET, ip.c_str(), &addr_.sin_addr) <= 0)
        {
            return;
        }
    }
    isUnspecified_ = false;
}

std::string INETAddr::toIp() const {
    char buf[64];
    if (addr_.sin_family == AF_INET)
    {
        ::inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof(buf));
    }
    else if (addr_.sin_family == AF_INET6)
    {
        ::inet_ntop(AF_INET6, &addr6_.sin6_addr, buf, sizeof(buf));
    }

    return buf;
}

std::string INETAddr::toIpPort() const {
    char buf[64] = "";
    uint16_t port = ntohs(addr_.sin_port);
    snprintf(buf, sizeof(buf), ":%u", port);
    return toIp() + std::string(buf);
}

uint16_t INETAddr::toPort() const {
    return ntohs(portNetEndian());
}

bool INETAddr::isIntranetIp() const {
    if (addr_.sin_family == AF_INET)
    {
        uint32_t ip_addr = ntohl(addr_.sin_addr.s_addr);
        if ((ip_addr >= 0x0A000000 && ip_addr <= 0x0AFFFFFF) ||
            (ip_addr >= 0xAC100000 && ip_addr <= 0xAC1FFFFF) ||
            (ip_addr >= 0xC0A80000 && ip_addr <= 0xC0A8FFFF) ||
            ip_addr == 0x7f000001)

        {
            return true;
        }
    }
    else
    {
        auto addrP = ip6NetEndian();
        // Loopback ip
        if (*addrP == 0 && *(addrP + 1) == 0 && *(addrP + 2) == 0 &&
            ntohl(*(addrP + 3)) == 1)
            return true;
        // Privated ip is prefixed by FEC0::/10 or FE80::/10, need testing
        auto i32 = (ntohl(*addrP) & 0xffc00000);
        if (i32 == 0xfec00000 || i32 == 0xfe800000)
            return true;
        if (*addrP == 0 && *(addrP + 1) == 0 && ntohl(*(addrP + 2)) == 0xffff)
        {
            // the IPv6 version of an IPv4 IP address
            uint32_t ip_addr = ntohl(*(addrP + 3));
            if ((ip_addr >= 0x0A000000 && ip_addr <= 0x0AFFFFFF) ||
                (ip_addr >= 0xAC100000 && ip_addr <= 0xAC1FFFFF) ||
                (ip_addr >= 0xC0A80000 && ip_addr <= 0xC0A8FFFF) ||
                ip_addr == 0x7f000001)

            {
                return true;
            }
        }
    }
    return false;
}

bool INETAddr::isLoopbackIp() const {
    if (!isIpV6())
    {
        uint32_t ip_addr = ntohl(addr_.sin_addr.s_addr);
        if (ip_addr == 0x7f000001)
        {
            return true;
        }
    }
    else
    {
        auto addrP = ip6NetEndian();
        if (*addrP == 0 && *(addrP + 1) == 0 && *(addrP + 2) == 0 &&
            ntohl(*(addrP + 3)) == 1)
            return true;
        // the IPv6 version of an IPv4 loopback address
        if (*addrP == 0 && *(addrP + 1) == 0 && ntohl(*(addrP + 2)) == 0xffff &&
            ntohl(*(addrP + 3)) == 0x7f000001)
            return true;
    }
    return false;
}

uint32_t INETAddr::ipNetEndian() const {
    return addr_.sin_addr.s_addr;
}

const uint32_t *INETAddr::ip6NetEndian() const {
    return addr6_.sin6_addr.s6_addr32;
}
