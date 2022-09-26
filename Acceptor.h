//
// Created by bong on 22. 9. 21.
//

#ifndef REACTOR_ACCEPTOR_H
#define REACTOR_ACCEPTOR_H

#include "EventHandler.h"
#include "INETAddr.h"
#include "TcpConnection.h"
#include <set>

class Acceptor: public EventHandler{
public:
    explicit Acceptor(const std::shared_ptr<EventLoop>& loop, const INETAddr &addr);

    void handle_event() override;

    std::shared_ptr<Handle> get_handle() const override;

private:
    std::shared_ptr<Handle> handle_;
    INETAddr addr_;
    int idleFd_;
    std::set<std::shared_ptr<TcpConnection>> connSet_;
    std::shared_ptr<EventLoop> loop_;

    std::shared_ptr<TcpConnection> newConnection(int newsock, INETAddr addr);
};


#endif //REACTOR_ACCEPTOR_H
