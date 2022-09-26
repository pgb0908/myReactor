//
// Created by bong on 22. 9. 26.
//

#ifndef REACTOR_TCPSERVER_H
#define REACTOR_TCPSERVER_H

#include "InitiationDispatcher.h"
#include "INETAddr.h"
#include "EventHandler.h"
#include "Acceptor.h"
#include "AppLogging.h"

class TcpServer {
public:
    explicit TcpServer(INETAddr addr);
    ~TcpServer()=default;

    void start();
    void stop();


private:
    std::unique_ptr<InitiationDispatcher> reactorPtr_;
    INETAddr addr_;

    std::shared_ptr<EventHandler> recvMessageCallback_;
    std::shared_ptr<EventHandler> connectionCallback_;


};


#endif //REACTOR_TCPSERVER_H
