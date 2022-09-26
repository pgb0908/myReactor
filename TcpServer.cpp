//
// Created by bong on 22. 9. 26.
//

#include "TcpServer.h"

TcpServer::TcpServer(INETAddr addr) : reactorPtr_(new InitiationDispatcher),
                                      addr_(addr),
                                      connectionCallback_(std::dynamic_pointer_cast<EventHandler>(std::make_shared<Acceptor>(addr))),
                                      recvMessageCallback_(nullptr){

    std::cout << "addr : " <<  addr_.toIp()  << ":" << addr_.toPort() << std::endl;

    reactorPtr_->register_handler(connectionCallback_);
};

void TcpServer::start() {
    reactorPtr_->handle_events();
}

void TcpServer::stop() {

}
