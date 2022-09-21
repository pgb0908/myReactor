//
// Created by bong on 22. 9. 21.
//

#ifndef REACTOR_INITIATIONDISPATCHER_H
#define REACTOR_INITIATIONDISPATCHER_H

enum class EventType
{
    ACCEPT_EVENT = 01,
    READ_EVENT = 02,
    WRITE_EVENT = 04,
    TIMEOUT_EVENT = 010,
    SIGNAL_EVENT = 020,
    CLOSE_EVENT = 040
};

class EventHandler;

class InitiationDispatcher{

public:
    int register_handler(EventHandler *eh, EventType et);

    int remove_handler(EventHandler *eh, EventType et);

    int handle_events(EventHandler *timeout = 0);
};

#endif //REACTOR_INITIATIONDISPATCHER_H
