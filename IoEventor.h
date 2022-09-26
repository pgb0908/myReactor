//
// Created by bong on 22. 9. 26.
//

#ifndef REACTOR_IOEVENTOR_H
#define REACTOR_IOEVENTOR_H


#include "EventHandler.h"

class IoEventor : public EventHandler{
public:
    explicit IoEventor(std::shared_ptr<EventLoop>& loop, int fd);

    // todo 클이라언트가 전송 한데이터를 read한 다음 +a 해서 write로 되돌려줌
    void handle_event() override;

    std::shared_ptr<Handle> get_handle() const override;
private:
    std::shared_ptr<EventLoop> loop_;
    std::shared_ptr<Handle> handle_;
    int fd_;
};


#endif //REACTOR_IOEVENTOR_H
