//
// Created by bong on 22. 9. 21.
//

#ifndef REACTOR_ACCEPTOR_H
#define REACTOR_ACCEPTOR_H

#include "EventHandler.h"
#include "INETAddr.h"

class Acceptor: public EventHandler{
public:
    explicit Acceptor(const INETAddr &addr);

    int handle_event(EventType et) override;

    Handle get_handle();

private:
    Handle handle_;
    INETAddr addr_;
};


#endif //REACTOR_ACCEPTOR_H
