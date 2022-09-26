//
// Created by bong on 22. 9. 21.
//

#include <cassert>
#include "EventLoop.h"
#include "EventHandler.h"
#include "Poller.h"

const int kPollTimeMs = 5000;

EventLoop::EventLoop() : looping_(false), quit_(false),eventHandling_(false),
                        poller_(new Poller(this)) {

}

EventLoop::~EventLoop() {
    std::cout << "EventLoop is destructed " << std::endl;
    struct timespec delay = {0, 1000000}; /* 1 msec */

    quit();

    while (looping_.load(std::memory_order_acquire))
    {
        nanosleep(&delay, nullptr);
    }

}

void EventLoop::quit() {
    quit_.store(true, std::memory_order_release);
}

void EventLoop::loop() {
    assert(!looping_);

    looping_.store(true, std::memory_order_release);
    quit_.store(false, std::memory_order_release);

    while (!quit_.load(std::memory_order_acquire)) {
        activeChannels_.clear();

        std::cout << "looping..." << std::endl;
        poller_->poll(kPollTimeMs, &activeChannels_);

        eventHandling_ = true;
        for (auto it = activeChannels_.begin(); it != activeChannels_.end(); ++it)
        {
            auto event_handler = *it;
            event_handler->handle_event();
        }
        eventHandling_ = false;
    }
}

void EventLoop::updateEvent(std::shared_ptr<EventHandler>& eventHandler) {
    poller_->updateEvent(eventHandler);
}

void EventLoop::removeEvent(std::shared_ptr<EventHandler>& eventHandler) {
    poller_->removeEvent(eventHandler);
}
