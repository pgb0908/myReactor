cmake_minimum_required(VERSION 3.23)
project(reactor)

set(CMAKE_CXX_STANDARD 11)

add_executable(reactor main.cpp InitiationDispatcher.h EventHandler.h Acceptor.h)
