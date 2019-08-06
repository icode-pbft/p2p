//
// Created by t0106 on 2019/7/14.
//
#include <thread>
#include <functional>

#include "peerThread.h"

/**
 * p2p通讯线程
 * 在接受套接字后，分成两个独立的线程，一个用于输入数据，一个用于输出数据，因此单向数据不会阻塞
 */
peerThread::peerThread() {}

peerThread::peerThread(SOCKET clientSocket, const std::string &ipAddress) : clientSocket(clientSocket),
                                                                       ipAddress(ipAddress) {
    this->peerWriter1 = peerWriter(clientSocket, ipAddress);
    this->peerReader1 = peerReader(clientSocket, ipAddress);
}

/**
 * Create read and write threads
 */
void peerThread::run() {
    std::cout << "\npeerThread run(): Got connection from " << ipAddress << std::endl;

    std::thread peerReaderThread(&peerReader::run, peerReader1);
    peerReaderThread.detach();

    std::thread peerWriterThread(&peerWriter::run, peerWriter1);
    peerWriterThread.detach();
}

/**
 * Send a message to the socket
 * @param data will sent
 */
void peerThread::send(std::string data) {
    peerWriter1.write(data);
}


