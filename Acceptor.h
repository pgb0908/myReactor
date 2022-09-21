//
// Created by bong on 22. 9. 21.
//

#ifndef REACTOR_ACCEPTOR_H
#define REACTOR_ACCEPTOR_H

#include "EventHandler.h"

class Acceptor: public EventHandler{
public:
    Acceptor(const INET_Addr &addr);

    virtual int handle_event (EventType et);

    Handle get_handle();

private:
    Sock_acceptor acceptor_;
};


#endif //REACTOR_ACCEPTOR_H
