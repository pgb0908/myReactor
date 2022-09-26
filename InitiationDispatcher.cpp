//
// Created by bong on 22. 9. 21.
//

#include "InitiationDispatcher.h"
#include <iostream>
#include <cstring>
#include "EventHandler.h"
#include "EventLoop.h"
#include <poll.h>


const int InitiationDispatcher::kNoneEvent = 0;
const int InitiationDispatcher::kReadEvent = POLLIN | POLLPRI;
const int InitiationDispatcher::kWriteEvent = POLLOUT;

void InitiationDispatcher::register_handler(std::shared_ptr<EventHandler>& eh) {
    loop_->updateEvent(eh);
}

void InitiationDispatcher::remove_handler(std::shared_ptr<EventHandler>&  eh) {
    loop_->removeEvent(eh);
}

void InitiationDispatcher::handle_events() {
    loop_->loop();
}

InitiationDispatcher::~InitiationDispatcher() = default;

InitiationDispatcher::InitiationDispatcher() : loop_(new EventLoop) {

}
