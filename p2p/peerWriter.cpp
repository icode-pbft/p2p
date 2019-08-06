//
// Created by t0106 on 2019/7/14.
//
#include <thread>
#include <string>
#include <fstream>
#include <mutex>

#include "peerWriter.h"

std::map<std::string ,std::deque<std::string>> peerWriter::writeMap;
std::mutex peerWriter::writeMapMutex;

peerWriter::peerWriter() {}

peerWriter::peerWriter(SOCKET clientSocket, const std::string &ipAddress) : clientSocket(clientSocket),
                                                                       ipAddress(ipAddress) {
    std::deque<std::string> writeQueue;
    peerWriter::writeMap[this->ipAddress] = writeQueue;
}

/**
 * Listen for a message in the write buffer, if so, take it out and send it
 */
void peerWriter::run() {
    while (true) {
        std::string sendMsg;

        // Reads the first element in the message queue and pops it
        writeMapMutex.lock();
        if (!peerWriter::writeMap[this->ipAddress].empty()) {
            sendMsg = peerWriter::writeMap[this->ipAddress].front();
            peerWriter::writeMap[this->ipAddress].pop_front();
        }
        writeMapMutex.unlock();

        if (!sendMsg.empty()) {
            sendMsg += "\r\n";
            if (send(clientSocket, sendMsg.data(), static_cast<int>(sendMsg.size()), 0) < 0) {
                std::cout << "sending ERROR!!! -->send " + sendMsg + " to " + ipAddress << std::endl;
                break;
            } else {
                std::cout << "sending success -->send " + sendMsg + " to " + ipAddress << std::endl;
            }
            sendMsg = "";
        }

        // todo:线程沉睡
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::cout << "peerWriter run():Peer " << ipAddress << " disconnected." << std::endl;
}

/**
 * Messages to be sent are saved to the buffer
 */
void peerWriter::write(std::string msg) {
    // Save the message to the end of the queue
    writeMapMutex.lock();
    std::deque<std::string> &temp = peerWriter::writeMap[this->ipAddress];
    if (std::find(temp.begin(), temp.end(), msg) == temp.end() || temp.empty()) {
        temp.push_back(msg);
    }
    writeMapMutex.unlock();
}





