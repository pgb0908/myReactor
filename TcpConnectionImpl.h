//
// Created by bong on 22. 9. 26.
//

#ifndef REACTOR_TCPCONNECTIONIMPL_H
#define REACTOR_TCPCONNECTIONIMPL_H

#include "TcpConnection.h"
#include "INETAddr.h"

class TcpConnectionImpl : public TcpConnection{
public:
    TcpConnectionImpl() = default;
    TcpConnectionImpl(int socketfd,
                      const INETAddr &localAddr,
                      const INETAddr &peerAddr);
    virtual ~TcpConnectionImpl() = default;

    void send(const char *msg, size_t len) override;

    void send(const std::string &msg) override;

    void send(std::string &&msg) override;

    void send(const MsgBuffer &buffer) override;

    void send(const std::shared_ptr<std::string> &msgPtr) override;

    void send(const std::shared_ptr<MsgBuffer> &msgPtr) override;

    bool connected() const override;

    void connectEstablished() override;

};


#endif //REACTOR_TCPCONNECTIONIMPL_H
