//
// Created by bong on 22. 9. 23.
//

#ifndef REACTOR_POLLER_H
#define REACTOR_POLLER_H

#include <vector>
#include <sys/epoll.h>
#include <map>

class EventLoop;
class EventHandler;

using EventList = std::vector<struct epoll_event>;

class Poller {
public:
    explicit Poller(EventLoop *loop);
    ~Poller();

    void poll(int timeoutMs, std::vector<EventHandler *> *activeChannels);
    void updateEvent(EventHandler *event);
    void removeEvent(EventHandler *event);

private:
    EventLoop *ownerLoop_;
    static const int kInitEventListSize = 16;
    int epollfd_;
    EventList events_;
    void update(int operation, EventHandler *channel);
    using EventHandlerMap = std::map<int, EventHandler *>;
    EventHandlerMap eventHandlerMap;
    void fillActiveChannels(int numEvents, std::vector<EventHandler *> *activeChannels) const;
};


#endif //REACTOR_POLLER_H
