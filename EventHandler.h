//
// Created by bong on 22. 9. 21.
//

#ifndef REACTOR_EVENTHANDLER_H
#define REACTOR_EVENTHANDLER_H

#include "Handle.h"
#include "InitiationDispatcher.h"

class EventHandler{
public:
    virtual int handle_event() = 0;
    virtual Handle get_handle() const = 0;

    int events(){return events_;};
    int index(){return index_;};
    int revents(){return revents_;};

    int setRevents(int revt)
    {
        // LOG_TRACE<<"revents="<<revt;
        revents_ = revt;
        return revt;
    };

    void setIndex(int index)
    {
        index_ = index;
    };

    bool isNoneEvent() const
    {
        return events_ == InitiationDispatcher::kNoneEvent;
    };

private:
    int events_;
    int index_;
    int revents_;
};

#endif //REACTOR_EVENTHANDLER_H
