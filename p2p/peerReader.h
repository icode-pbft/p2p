//
// Created by t0106 on 2019/7/14.
//

#ifndef TCP_TEST_PEERREADER_H
#define TCP_TEST_PEERREADER_H

#include <iostream>
#include <vector>
#include <WinSock2.h>
#include <queue>
#include <map>
#include <mutex>

class peerReader {
private:
    SOCKET clientSocket;
    std::string ipAddress;

public:
    /**
     * key = ip
     * value = the queue on which the message is sent
     */
    static std::map<std::string, std::deque<std::string>> readMap;

    static std::mutex readMapMutex;

public:
    peerReader();

    peerReader(SOCKET clientSocket, const std::string &ipAddress);

    void run();

    // 取出缓冲数据
    std::vector<std::string> readData();
};


#endif //TCP_TEST_PEERREADER_H
