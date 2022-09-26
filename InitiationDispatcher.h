//
// Created by bong on 22. 9. 21.
//

#ifndef REACTOR_INITIATIONDISPATCHER_H
#define REACTOR_INITIATIONDISPATCHER_H

#include <unordered_map>
#include <memory>
#include <sys/epoll.h>

#define MAX_CONN 15


class EventHandler;
class EventLoop;

class InitiationDispatcher{

public:
    InitiationDispatcher();
    ~InitiationDispatcher();

    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    void register_handler(std::shared_ptr<EventHandler>& eh);

    void remove_handler(std::shared_ptr<EventHandler>&  eh);

    void handle_events();


private:
    std::unique_ptr<EventLoop> loop_;
};

#endif //REACTOR_INITIATIONDISPATCHER_H
