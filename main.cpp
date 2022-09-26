#include <iostream>
#include <memory>
#include "TcpServer.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    INETAddr addr("127.0.0.1", 12000);
    std::unique_ptr<TcpServer> tcpServer(new TcpServer(addr));
    tcpServer->start();

    return 0;
}
