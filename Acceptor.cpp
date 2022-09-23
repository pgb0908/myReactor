//
// Created by bong on 22. 9. 21.
//

#include "Acceptor.h"


Acceptor::Acceptor(const INETAddr &addr) : handle_(0) {

}

int Acceptor::handle_event() {
    return 0;
}

Handle Acceptor::get_handle() {
    return handle_;
}
