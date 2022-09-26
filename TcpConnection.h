//
// Created by bong on 22. 9. 26.
//

#ifndef REACTOR_TCPCONNECTION_H
#define REACTOR_TCPCONNECTION_H

#include <string>
#include <memory>
#include "MsgBuffer.h"

class TcpConnection{
public:
    TcpConnection() = default;
    virtual ~TcpConnection(){};

    virtual void send(const char *msg, size_t len) = 0;
    virtual void send(const std::string &msg) = 0;
    virtual void send(std::string &&msg) = 0;
    virtual void send(const MsgBuffer &buffer) = 0;
    virtual void send(const std::shared_ptr<std::string> &msgPtr) = 0;
    virtual void send(const std::shared_ptr<MsgBuffer> &msgPtr) = 0;

    virtual bool connected() const = 0;

    virtual void connectEstablished() = 0;
};

#endif //REACTOR_TCPCONNECTION_H
