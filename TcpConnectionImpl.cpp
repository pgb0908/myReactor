//
// Created by bong on 22. 9. 26.
//

#include <iostream>
#include "TcpConnectionImpl.h"

void TcpConnectionImpl::send(const char *msg, size_t len) {

}

void TcpConnectionImpl::send(const std::string &msg) {

}

void TcpConnectionImpl::send(std::string &&msg) {

}

void TcpConnectionImpl::send(const MsgBuffer &buffer) {

}

void TcpConnectionImpl::send(const std::shared_ptr<std::string> &msgPtr) {

}

void TcpConnectionImpl::send(const std::shared_ptr<MsgBuffer> &msgPtr) {

}

bool TcpConnectionImpl::connected() const {
    return false;
}

void TcpConnectionImpl::connectEstablished() {
    std::cout << "established!!" << std::endl;
}

TcpConnectionImpl::TcpConnectionImpl(int socketfd, const INETAddr &localAddr, const INETAddr &peerAddr) {

}
