//
// Created by bong on 22. 9. 26.
//

#include <iostream>
#include "TcpConnectionImpl.h"
#include "IoEventor.h"

bool TcpConnectionImpl::connected() const {
    return status_ == ConnStatus::Connected;
}

void TcpConnectionImpl::connectEstablished() {
    std::cout << "established!!" << std::endl;
    assert(status_ == ConnStatus::Connecting);

    ioEventPtr_->enableReading();
    status_ =  ConnStatus::Connected;

    // todo connetion callback, IoEventor가 여러 이벤트를 컬백으로 처리할 수 있도록 해야함
/*    if (ioEventPtr_->connectionCallback_){
        connectionCallback_();
    }*/
}

TcpConnectionImpl::TcpConnectionImpl(int socketfd, const INETAddr &localAddr, const INETAddr &peerAddr):
        ioEventPtr_(std::make_shared<IoEventor>()),
        localAddr_(localAddr),
        peerAddr_(peerAddr){
    std::cout << "new connection:" << peerAddr.toIpPort() << "->" << localAddr.toIpPort() << std::endl;

/*    ioEventPtr_->setReadCallback(
            std::bind(&TcpConnectionImpl::readCallback, this));
    ioEventPtr_->setWriteCallback(
            std::bind(&TcpConnectionImpl::writeCallback, this));
    ioEventPtr_->setCloseCallback(
            std::bind(&TcpConnectionImpl::handleClose, this));
    ioEventPtr_->setErrorCallback(
            std::bind(&TcpConnectionImpl::handleError, this));*/
    //ioEventPtr_->get_handle()->setKeepAlive(true);
    name_ = localAddr.toIpPort() + "--" + peerAddr.toIpPort();

}
