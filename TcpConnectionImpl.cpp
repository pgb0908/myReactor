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
    //ioEventPtr_->connectionCall();
}

TcpConnectionImpl::TcpConnectionImpl(std::shared_ptr<EventLoop>& loop, int socketfd, const INETAddr &localAddr, const INETAddr &peerAddr):
        loop_(loop),
        ioEventPtr_(std::make_shared<IoEventor>(loop, socketfd)),
        localAddr_(localAddr),
        peerAddr_(peerAddr){
    std::cout << "new connection:" << peerAddr.toIpPort() << "->" << localAddr.toIpPort() << std::endl;

    //ioEventPtr_->get_handle()->setKeepAlive(true);
    name_ = localAddr.toIpPort() + "--" + peerAddr.toIpPort();

}
