//
// Created by bong on 22. 9. 21.
//

#ifndef REACTOR_EVENTLOOP_H
#define REACTOR_EVENTLOOP_H

#include <atomic>
#include <memory>
#include <vector>

class EventHandler;
class Poller;

class EventLoop {
public:
    EventLoop();
    ~EventLoop();

    void loop();

    void quit();

    void updateEvent(EventHandler* eventHandler);
    void removeEvent(EventHandler* eventHandler);

private:
    std::atomic<bool> looping_;
    std::atomic<bool> quit_;
    std::unique_ptr<Poller> poller_;
    bool eventHandling_;

    std::vector<EventHandler*> activeChannels_;

};


#endif //REACTOR_EVENTLOOP_H
