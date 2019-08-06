//
// Created by t0106 on 2019/7/14.
//

#include <fstream>
#include <mutex>
#include <string>

#include "peerReader.h"

std::map<std::string, std::deque<std::string>> peerReader::readMap;
std::mutex peerReader::readMapMutex;

peerReader::peerReader() {}

/**
 * 构造函数 初始时即给static map赋予key值
 */
peerReader::peerReader(SOCKET clientSocket, const std::string &ipAddress) : clientSocket(clientSocket),
                                                                       ipAddress(ipAddress) {
    std::deque<std::string> readQueue;
    peerReader::readMap[this->ipAddress] = readQueue;
}

/**
 * Monitor for incoming messages
 */
void peerReader::run() {
    char receiveMsgBuff[200];
    memset(receiveMsgBuff, 0, sizeof(receiveMsgBuff));

    // result = 0: socket disconnected
    // result < 0: error
    // result > 0: length of message received
    while (true) {
        int result = recv(clientSocket, receiveMsgBuff, sizeof(receiveMsgBuff), 0);
        if (result < 0) {
            std::cout << "peerReader run(): recv return value which < 0" << std::endl;
            break;
        } else {
            std::cout << "peerReader run(): \"" << receiveMsgBuff << "\"" << std::endl;

            std::string msg(receiveMsgBuff);

            // 去除结尾的换行符 \r\n
            while (msg.back() == '\r' || msg.back() == '\n') {
                msg.pop_back();
            }
            //todo:存入队列 2019年8月3日15:17:23 田泽鑫
            readMapMutex.lock();
            std::deque<std::string> &temp = peerReader::readMap[this->ipAddress];
            if (std::find(temp.begin(), temp.end(), msg) == temp.end() || temp.empty()) {
                temp.push_back(msg);
            }
            readMapMutex.unlock();
        }
        //todo:沉睡？
//        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // 正常情况下会一直while循环阻塞
    std::cout << "peerReader run(): socket disconnected " << std::endl;
}

/**
 * Reads all messages from a buffered queue
 * @return a collection of all messages
 */
std::vector<std::string> peerReader::readData() {
    std::vector<std::string> buffer;

    readMapMutex.lock();
    while (!peerReader::readMap[this->ipAddress].empty()) {
        buffer.emplace_back(peerReader::readMap[this->ipAddress].front());
        peerReader::readMap[this->ipAddress].pop_front();
    }
    readMapMutex.unlock();

    return buffer;
}








