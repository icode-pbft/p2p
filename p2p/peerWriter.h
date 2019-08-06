//
// Created by t0106 on 2019/7/14.
//

#ifndef TCP_TEST_PEERWRITER_H
#define TCP_TEST_PEERWRITER_H

#include <iostream>
#include <vector>
#include <WinSock2.h>
#include <queue>
#include <map>
#include <mutex>

class peerWriter {
private:
    SOCKET clientSocket;
    std::string ipAddress;
public:
    /**
     * key = ip
     * value = the queue on which the message is sent
     */
    static std::map<std::string ,std::deque<std::string>> writeMap;

    static std::mutex writeMapMutex;
public:
    peerWriter();

    peerWriter(SOCKET clientSocket, const std::string &ipAddress);

    void run();

    void write(std::string msg);
};


#endif //TCP_TEST_PEERWRITER_H
