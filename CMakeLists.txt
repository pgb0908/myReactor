cmake_minimum_required(VERSION 3.23)
project(reactor)

set(CMAKE_CXX_STANDARD 11)

add_executable(reactor main.cpp InitiationDispatcher.h EventHandler.h Acceptor.h InitiationDispatcher.cpp Acceptor.cpp Handle.cpp Handle.h INETAddr.cpp INETAddr.h EventLoop.cpp EventLoop.h)
