//
// Created by bong on 22. 9. 21.
//

#include <cstring>
#include <cassert>
#include "Handle.h"

#define	TCP_NODELAY		 1  /* Don't delay send to coalesce packets  */
#define	TCP_MAXSEG		 2  /* Set maximum segment size  */
#define TCP_CORK		 3  /* Control sending of partial frames  */
#define TCP_KEEPIDLE		 4  /* Start keeplives after this period */
#define TCP_KEEPINTVL		 5  /* Interval between keepalives */
#define TCP_KEEPCNT		 6  /* Number of keepalives before death */
#define TCP_SYNCNT		 7  /* Number of SYN retransmits */
#define TCP_LINGER2		 8  /* Life time of orphaned FIN-WAIT-2 state */
#define TCP_DEFER_ACCEPT	 9  /* Wake up listener only when data arrive */
#define TCP_WINDOW_CLAMP	 10 /* Bound advertised window */
#define TCP_INFO		 11 /* Information about this connection. */
#define	TCP_QUICKACK		 12 /* Bock/reenable quick ACKs.  */
#define TCP_CONGESTION		 13 /* Congestion control algorithm.  */
#define TCP_MD5SIG		 14 /* TCP MD5 Signature (RFC2385) */
#define TCP_COOKIE_TRANSACTIONS	 15 /* TCP Cookie Transactions */
#define TCP_THIN_LINEAR_TIMEOUTS 16 /* Use linear timeouts for thin streams*/
#define TCP_THIN_DUPACK		 17 /* Fast retrans. after 1 dupack */
#define TCP_USER_TIMEOUT	 18 /* How long for loss retry before timeout */
#define TCP_REPAIR		 19 /* TCP sock is under repair right now */
#define TCP_REPAIR_QUEUE	 20 /* Set TCP queue to repair */
#define TCP_QUEUE_SEQ		 21 /* Set sequence number of repaired queue. */
#define TCP_REPAIR_OPTIONS	 22 /* Repair TCP connection options */
#define TCP_FASTOPEN		 23 /* Enable FastOpen on listeners */
#define TCP_TIMESTAMP		 24 /* TCP time stamp */
#define TCP_NOTSENT_LOWAT	 25 /* Limit number of unsent bytes in
				       write queue.  */
#define TCP_CC_INFO		 26 /* Get Congestion Control
				       (optional) info.  */
#define TCP_SAVE_SYN		 27 /* Record SYN headers for new
				       connections.  */
#define TCP_SAVED_SYN		 28 /* Get SYN headers recorded for
				       connection.  */
#define TCP_REPAIR_WINDOW	 29 /* Get/set window parameters.  */
#define TCP_FASTOPEN_CONNECT	 30 /* Attempt FastOpen with connect.  */
#define TCP_ULP			 31 /* Attach a ULP to a TCP connection.  */
#define TCP_MD5SIG_EXT		 32 /* TCP MD5 Signature with extensions.  */
#define TCP_FASTOPEN_KEY	 33 /* Set the key for Fast Open (cookie).  */
#define TCP_FASTOPEN_NO_COOKIE	 34 /* Enable TFO without a TFO cookie.  */
#define TCP_ZEROCOPY_RECEIVE	 35
#define TCP_INQ			 36 /* Notify bytes available to read
				       as a cmsg on read.  */
#define TCP_CM_INQ		 TCP_INQ
#define TCP_TX_DELAY		 37 /* Delay outgoing packets by XX usec.  */

#define TCP_REPAIR_ON		 1
#define TCP_REPAIR_OFF		 0
#define TCP_REPAIR_OFF_NO_WP	 -1

Handle::~Handle() {
    std::cout << "Socket deconstructed:" << sockFd_;
    if (sockFd_ >= 0)  close(sockFd_);

}

bool Handle::isSelfConnect(int sockfd) {
    struct sockaddr_in6 localaddr = getLocalAddr(sockfd);
    struct sockaddr_in6 peeraddr = getPeerAddr(sockfd);
    if (localaddr.sin6_family == AF_INET)
    {
        const struct sockaddr_in *laddr4 =
                reinterpret_cast<struct sockaddr_in *>(&localaddr);
        const struct sockaddr_in *raddr4 =
                reinterpret_cast<struct sockaddr_in *>(&peeraddr);
        return laddr4->sin_port == raddr4->sin_port &&
               laddr4->sin_addr.s_addr == raddr4->sin_addr.s_addr;
    }
    else if (localaddr.sin6_family == AF_INET6)
    {
        return localaddr.sin6_port == peeraddr.sin6_port &&
               memcmp(&localaddr.sin6_addr,
                      &peeraddr.sin6_addr,
                      sizeof localaddr.sin6_addr) == 0;
    }
    else
    {
        return false;
    }
}

void Handle::bindAddress(const INETAddr &localaddr) {
    assert(sockFd_ > 0);
    int ret;
    if (localaddr.isIpV6())
        ret = ::bind(sockFd_, localaddr.getSockAddr(), sizeof(sockaddr_in6));
    else
        ret = ::bind(sockFd_, localaddr.getSockAddr(), sizeof(sockaddr_in));

    if (ret == 0)
        return;
    else
    {
        std::cout << ", Bind address failed at " << localaddr.toIpPort();
        exit(1);
    }
}

void Handle::listen() {
    assert(sockFd_ > 0);
    int ret = ::listen(sockFd_, SOMAXCONN);
    if (ret < 0)
    {
        std::cout  << "listen failed";
        exit(1);
    }
}

int Handle::accept(INETAddr *peeraddr) {
    struct sockaddr_in6 addr6;
    memset(&addr6, 0, sizeof(addr6));
    socklen_t size = sizeof(addr6);

    int connfd = ::accept4(sockFd_,
                           (struct sockaddr *)&addr6,
                           &size,
                           SOCK_NONBLOCK | SOCK_CLOEXEC);

    if (connfd >= 0)
    {
        peeraddr->setSockAddrInet6(addr6);
    }
    return connfd;
}

void Handle::closeWrite() {
    if (::shutdown(sockFd_, SHUT_WR) < 0)
    {
        std::cout << "sockets::shutdownWrite";
    }
}

int Handle::read(char *buffer, uint64_t len) {
    return ::read(sockFd_, buffer, len);
}

struct sockaddr_in6 Handle::getLocalAddr(int sockfd) {
    struct sockaddr_in6 localaddr;
    memset(&localaddr, 0, sizeof(localaddr));
    socklen_t addrlen = static_cast<socklen_t>(sizeof localaddr);
    if (::getsockname(sockfd,
                      static_cast<struct sockaddr *>((void *)(&localaddr)),
                      &addrlen) < 0)
    {
        std::cout << "sockets::getLocalAddr";
    }
    return localaddr;
}

struct sockaddr_in6 Handle::getPeerAddr(int sockfd) {
    struct sockaddr_in6 peeraddr;
    memset(&peeraddr, 0, sizeof(peeraddr));
    socklen_t addrlen = static_cast<socklen_t>(sizeof peeraddr);
    if (::getpeername(sockfd,
                      static_cast<struct sockaddr *>((void *)(&peeraddr)),
                      &addrlen) < 0)
    {
        std::cout  << "sockets::getPeerAddr";
    }
    return peeraddr;
}

void Handle::setTcpNoDelay(bool on) {
    int optval = on ? 1 : 0;

    ::setsockopt(sockFd_,
                 IPPROTO_TCP,
                 TCP_NODELAY,
                 &optval,
                 static_cast<socklen_t>(sizeof optval));
}

void Handle::setReuseAddr(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(sockFd_,
                 SOL_SOCKET,
                 SO_REUSEADDR,
                 &optval,
                 static_cast<socklen_t>(sizeof optval));
}

void Handle::setReusePort(bool on) {
    int optval = on ? 1 : 0;

    int ret = ::setsockopt(sockFd_,
                           SOL_SOCKET,
                           SO_REUSEPORT,
                           &optval,
                           static_cast<socklen_t>(sizeof optval));
    if (ret < 0 && on)
    {
        std::cout << "SO_REUSEPORT failed.";
    }
}

void Handle::setKeepAlive(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(sockFd_,
                 SOL_SOCKET,
                 SO_KEEPALIVE,
                 &optval,
                 static_cast<socklen_t>(sizeof optval));

}

int Handle::getSocketError() {
    int optval;
    socklen_t optlen = static_cast<socklen_t>(sizeof optval);

    if (::getsockopt(sockFd_, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0)
    {
        return errno;
    }
    else
    {
        return optval;
    }
}
