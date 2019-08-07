//
// Created by t0106 on 2019/7/14.
//

#ifndef TCP_TEST_PEERNETWORK_H
#define TCP_TEST_PEERNETWORK_H

#include <iostream>
#include <vector>

#include "peerThread.h"

/**
 * P2P网络，负责处理peer之间的通信
 */
class peerNetwork {

private:
    /** listening port */
    int port;
    bool runFlag = true;

    static std::mutex networkMutex;
public:
    /** a collection of clients that have established a connection */
    std::vector<peerThread> peerThreads;
    /** a collection of client information for which a connection has been established */
    std::vector<std::string> peers;

    peerNetwork();

    explicit peerNetwork(int port);

    void connectToPeer(std::string ipAddress, int port);

    void run();

    void broadcast(const std::string& data);
};


#endif //TCP_TEST_PEERNETWORK_H
