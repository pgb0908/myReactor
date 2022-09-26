//
// Created by bong on 22. 9. 26.
//

#ifndef REACTOR_TCPCONNECTIONIMPL_H
#define REACTOR_TCPCONNECTIONIMPL_H

#include "TcpConnection.h"
#include "INETAddr.h"
#include "EventHandler.h"

class TcpConnectionImpl : public TcpConnection{
public:
    TcpConnectionImpl() = default;
    TcpConnectionImpl(std::shared_ptr<EventLoop>& loop, int socketfd,
                      const INETAddr &localAddr,
                      const INETAddr &peerAddr);
    virtual ~TcpConnectionImpl() = default;

    bool connected() const override;

    void connectEstablished() override;

private:

    enum class ConnStatus
    {
        Disconnected,
        Connecting,
        Connected,
        Disconnecting
    };
    ConnStatus status_{ConnStatus::Connecting};

    std::shared_ptr<EventHandler> ioEventPtr_;
    INETAddr localAddr_, peerAddr_;
    std::string name_;
    std::shared_ptr<EventLoop> loop_;

};


#endif //REACTOR_TCPCONNECTIONIMPL_H
