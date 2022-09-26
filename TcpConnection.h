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

    virtual bool connected() const = 0;
    virtual void connectEstablished() = 0;
};

#endif //REACTOR_TCPCONNECTION_H
