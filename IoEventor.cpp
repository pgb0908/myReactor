//
// Created by bong on 22. 9. 26.
//

#include "IoEventor.h"
#include "EventLoop.h"

void IoEventor::handle_event() {
}

std::shared_ptr<Handle> IoEventor::get_handle() const {
    return handle_;
}

IoEventor::IoEventor(std::shared_ptr<EventLoop>& loop, int fd) : loop_(loop), fd_(fd){
    handle_ = std::make_shared<Handle>(fd_);
    //loop_->updateEvent(this);
}
