//
// Created by bong on 22. 9. 21.
//

#ifndef REACTOR_HANDLE_H
#define REACTOR_HANDLE_H

#include "INETAddr.h"
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

class Handle {
public:
    static int createNonblockingSocketOrDie(int family)
    {
        int sock = ::socket(family,
                            SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,
                            IPPROTO_TCP);

        if (sock < 0)
        {
            std::cout << "sockets::createNonblockingOrDie";
            exit(1);
        }
        std::cout << "sock=" << sock << std::endl;
        return sock;
    }

    static int getSocketError(int sockfd)
    {
        int optval;
        socklen_t optlen = static_cast<socklen_t>(sizeof optval);
        if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0)
        {
            return errno;
        }
        else
        {
            return optval;
        }
    }

    static void setNonBlockAndCloseOnExec(int sockfd)
    {
        // non-block
        int flags = ::fcntl(sockfd, F_GETFL, 0);
        flags |= O_NONBLOCK;
        int ret = ::fcntl(sockfd, F_SETFL, flags);
        // TODO check

        // close-on-exec
        flags = ::fcntl(sockfd, F_GETFD, 0);
        flags |= FD_CLOEXEC;
        ret = ::fcntl(sockfd, F_SETFD, flags);
        // TODO check

        (void)ret;
    }

    static int connect(int sockfd, const INETAddr &addr)
    {
        if (addr.isIpV6())
            return ::connect(sockfd,
                             addr.getSockAddr(),
                             static_cast<socklen_t>(
                                     sizeof(struct sockaddr_in6)));
        else
            return ::connect(sockfd,
                             addr.getSockAddr(),
                             static_cast<socklen_t>(
                                     sizeof(struct sockaddr_in)));
    }

    static bool isSelfConnect(int sockfd);

    explicit Handle(int sockfd) : sockFd_(sockfd)
    {
    }
    ~Handle();
    /// abort if address in use
    void bindAddress(const INETAddr &localaddr);
    /// abort if address in use
    void listen();
    int accept(INETAddr *peeraddr);
    void closeWrite();
    int read(char *buffer, uint64_t len);
    int fd()
    {
        return sockFd_;
    }
    static struct sockaddr_in6 getLocalAddr(int sockfd);
    static struct sockaddr_in6 getPeerAddr(int sockfd);

    void setTcpNoDelay(bool on);
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void setKeepAlive(bool on);
    int getSocketError();

protected:
    int sockFd_;


};


#endif //REACTOR_HANDLE_H
