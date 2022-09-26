//
// Created by bong on 22. 9. 21.
//

#ifndef REACTOR_EVENTHANDLER_H
#define REACTOR_EVENTHANDLER_H

#include "Handle.h"
#include "InitiationDispatcher.h"

class EventHandler{
public:
    EventHandler(int events, int index, int revents):events_(events), index_(index), revents_(revents){
    }

    EventHandler() {};
    virtual void handle_event() = 0;
    virtual std::shared_ptr<Handle> get_handle() const = 0;

    int events(){return events_;};
    int index(){return index_;};
    int revents(){return revents_;};

    int setRevents(int revt)
    {
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

    void enableReading()
    {
        events_ |= InitiationDispatcher::kReadEvent;
    }

    void disableReading()
    {
        events_ &= ~InitiationDispatcher::kReadEvent;
    }

    void enableWriting()
    {
        events_ |= InitiationDispatcher::kWriteEvent;
    }

    void disableWriting()
    {
        events_ &= ~InitiationDispatcher::kWriteEvent;
    }

    bool isWriting() const
    {
        return events_ & InitiationDispatcher::kWriteEvent;
    }

    bool isReading() const
    {
        return events_ & InitiationDispatcher::kReadEvent;
    }

    void updateEvents(int events)
    {
        events_ = events;
    }


private:
    int events_{};
    int index_{};
    int revents_{};
};

#endif //REACTOR_EVENTHANDLER_H
