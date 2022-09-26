//
// Created by bong on 22. 9. 26.
//

#ifndef REACTOR_APPLOGGING_H
#define REACTOR_APPLOGGING_H


#include "EventHandler.h"

class AppLogging : public EventHandler{
public:
    void handle_event() override;

    std::shared_ptr<Handle> get_handle() const override;
};


#endif //REACTOR_APPLOGGING_H
