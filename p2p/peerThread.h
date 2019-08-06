//
// Created by t0106 on 2019/7/14.
//

#ifndef TCP_TEST_PEERTHREAD_H
#define TCP_TEST_PEERTHREAD_H

#include <WinSock2.h>

#include "peerWriter.h"
#include "peerReader.h"

class peerThread {
private:
    SOCKET clientSocket;

public:
    // ex: 192.168.10.10
    std::string ipAddress;
    peerWriter peerWriter1 = peerWriter();
    peerReader peerReader1 = peerReader();

public:
    peerThread();

    peerThread(SOCKET clientSocket, const std::string &ipAddress);

    void run();

    void send(std::string data);
};


#endif //TCP_TEST_PEERTHREAD_H
