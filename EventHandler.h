//
// Created by bong on 22. 9. 21.
//

#ifndef REACTOR_EVENTHANDLER_H
#define REACTOR_EVENTHANDLER_H

#include "Handle.h"
#include "InitiationDispatcher.h"

class EventHandler{
public:
    virtual int handle_event(EventType et) = 0;
    virtual Handle get_handle() const = 0;
};

#endif //REACTOR_EVENTHANDLER_H
