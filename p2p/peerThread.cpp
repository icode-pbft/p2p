//
// Created by t0106 on 2019/7/14.
//
#include <thread>
#include <functional>

#include "peerThread.h"

/**
 * p2pͨѶ�߳�
 * �ڽ����׽��ֺ󣬷ֳ������������̣߳�һ�������������ݣ�һ������������ݣ���˵������ݲ�������
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


