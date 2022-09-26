//
// Created by bong on 22. 9. 23.
//

#include "Poller.h"

#include <cerrno>
#include <iostream>
#include <cassert>
#include <cstring>
#include <unistd.h>
#include "EventHandler.h"

namespace
{
    const int kNew = -1;
    const int kAdded = 1;
    const int kDeleted = 2;
}  // namespace

void Poller::poll(int timeoutMs, std::vector<EventHandler *> *activeChannels) {
    int numEvents = ::epoll_wait(epollfd_,
                                 &*events_.begin(),
                                 static_cast<int>(events_.size()),
                                 timeoutMs);
    int savedErrno = errno;

    if (numEvents > 0)
    {
        std::cout << numEvents << " events happended" << std::endl;
        fillActiveChannels(numEvents, activeChannels);
        if (static_cast<size_t>(numEvents) == events_.size())
        {
            events_.resize(events_.size() * 2);
        }
    }
    else if (numEvents == 0)
    {
        std::cout << "nothing happended" << std::endl;
    }
    else
    {
        // error happens, log uncommon ones
        if (savedErrno != EINTR)
        {
            errno = savedErrno;
            std::cout << "EPollEpollPoller::poll()" << std::endl;
        }
    }
    return;
}

void Poller::updateEvent(std::shared_ptr<EventHandler>& event) {
    const int index = event->index();
    //  << " events = " << channel->events() << " index = " << index;
    if (index == kNew || index == kDeleted)  // -1 | 2
    {
        int fd = event->get_handle()->fd();
        if (index == kNew)
        {
            assert(eventHandlerMap.find(fd) == eventHandlerMap.end());
            eventHandlerMap[fd] = event;
        }
        else
        {  // index == kDeleted
            assert(eventHandlerMap.find(fd) != eventHandlerMap.end());
            assert(eventHandlerMap[fd] == event);
        }
        event->setIndex(kAdded);
        update(EPOLL_CTL_ADD, event);
    }else{
        int fd = event->get_handle()->fd();
        (void)fd;
        assert(eventHandlerMap.find(fd) != eventHandlerMap.end());
        assert(eventHandlerMap[fd] == event);

        assert(index == kAdded);
        if (event->isNoneEvent())
        {
            update(EPOLL_CTL_DEL, event);
            event->setIndex(kDeleted);
        }
        else
        {
            update(EPOLL_CTL_MOD, event);
        }
    }
}

void Poller::removeEvent(std::shared_ptr<EventHandler>& event) {
    int fd = event->get_handle()->fd();
    assert(eventHandlerMap.find(fd) != eventHandlerMap.end());
    assert(eventHandlerMap[fd] == event);
    size_t n = eventHandlerMap.erase(fd);
    (void)n;
    assert(n == 1);

    assert(event->isNoneEvent());
    int index = event->index();
    assert(index == kAdded || index == kDeleted);
    if (index == kAdded)
    {
        update(EPOLL_CTL_DEL, event);
    }
    event->setIndex(kNew);
}

void Poller::fillActiveChannels(int numEvents, std::vector<EventHandler *> *activeEventHandlers) const {
    assert(static_cast<size_t>(numEvents) <= events_.size());
    for (int i = 0; i < numEvents; ++i)
    {
        EventHandler *eventHandler = static_cast<EventHandler *>(events_[i].data.ptr);
        int fd = eventHandler->get_handle()->fd();
        EventHandlerMap::const_iterator it = eventHandlerMap.find(fd);
        assert(it != eventHandlerMap.end());
        //assert(it->second == eventHandler);

        eventHandler->setRevents(events_[i].events);
        activeEventHandlers->push_back(eventHandler);
    }
}

void Poller::update(int operation, std::shared_ptr<EventHandler> eventHandler) {
    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    event.events = eventHandler->events();
    event.data.ptr = eventHandler.get();
    int fd = eventHandler->get_handle()->fd();
    if (::epoll_ctl(epollfd_, operation, fd, &event) < 0)
    {
        if (operation == EPOLL_CTL_DEL)
        {
            std::cout << "epoll_ctl op =" << operation <<" fd =" << fd << std::endl;
        }
        else
        {
            std::cout << "epoll_ctl op =" << operation<< " fd =" << fd << std::endl;
        }
    }
}

Poller::Poller(EventLoop *loop) : ownerLoop_(loop),
                                  epollfd_(::epoll_create1(EPOLL_CLOEXEC)),
                                  events_(kInitEventListSize){

}

Poller::~Poller() {
    close(epollfd_);
}
