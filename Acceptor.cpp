//
// Created by bong on 22. 9. 21.
//

#include "Acceptor.h"
#include "TcpConnectionImpl.h"


Acceptor::Acceptor(const INETAddr &addr) : addr_(addr),
                                           idleFd_(::open("/dev/null", O_RDONLY | O_CLOEXEC)){
    handle_ = std::make_shared<Handle>(Handle::createNonblockingSocketOrDie(addr.getSockAddr()->sa_family));

    std::cout << "Acceptor addr : " << addr_.toIpPort() << std::endl;

    handle_->bindAddress(addr_);
    handle_->listen();

    setIndex(-1);
    setEvents(EPOLLIN);


/*    if (addr_.toPort() == 0)
    {
        addr_ = INETAddr{Handle::getLocalAddr(handle_->fd())};
    }*/
}

void Acceptor::handle_event() {

    INETAddr peer; // peer 이거 뭐지??
    int newsock = handle_->accept(&addr_);
    if (newsock >= 0) {
        auto newPtr = newConnection(newsock, peer);
        connSet_.insert(newPtr);
        newPtr->connectEstablished();

    }
    else{
        if (errno == EMFILE)
        {
            ::close(idleFd_);
            idleFd_ = handle_->accept(&peer);
            ::close(idleFd_);
            idleFd_ = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
        }
    }
}

std::shared_ptr<Handle> Acceptor::get_handle() const {
    return handle_;
}

std::shared_ptr<TcpConnection> Acceptor::newConnection(int sockfd, INETAddr peer) {
    std::cout << "new connection:fd=" << sockfd
              << " address=" << peer.toIpPort() << std::endl;

    std::shared_ptr<TcpConnectionImpl> newPtr;

    newPtr = std::make_shared<TcpConnectionImpl>(sockfd,
                                                 INETAddr(Handle::getLocalAddr(sockfd)),
                                                 peer);
/*    newPtr->setRecvMsgCallback(recvMessageCallback_);

    newPtr->setConnectionCallback(
            [this](const TcpConnectionPtr &connectionPtr) {
                if (connectionCallback_)
                    connectionCallback_(connectionPtr);
            });
    newPtr->setWriteCompleteCallback(
            [this](const TcpConnectionPtr &connectionPtr) {
                if (writeCompleteCallback_)
                    writeCompleteCallback_(connectionPtr);
            });
    newPtr->setCloseCallback(std::bind(&TcpServer::connectionClosed, this, _1));
            */

    return newPtr;
}
