//
// Created by t0106 on 2019/7/14.
//

#include <iostream>
#include <vector>
#include <thread>
#include <WinSock2.h>
#include <functional>
#include <string>
#include <WS2tcpip.h>

#include "peerNetwork.h"
#include "peerThread.h"


peerNetwork::peerNetwork() {
    this->port = 8015;
    peerThreads = std::vector<peerThread>(0);
    peers = std::vector<std::string>(0);
}

peerNetwork::peerNetwork(int port) {
    this->port = static_cast<u_short>(port);
    peerThreads = std::vector<peerThread>(0);
    peers = std::vector<std::string>(0);
}

/**
 * Establish a connection based on the given IP and port
 * @param ipAddress
 * @param port
 */
void peerNetwork::connectToPeer(std::string ipAddress, int port) {
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == SOCKET_ERROR) {
        std::cout << "peerNetWork connectToPeer(): Socket() error: " << WSAGetLastError() << std::endl;
        return;
    }

    SOCKADDR_IN serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(static_cast<u_short>(port));
//    serverAddr.sin_addr.S_un.S_addr = inet_addr(host.data());
    inet_pton(AF_INET, ipAddress.data(), &serverAddr.sin_addr.S_un.S_addr);



    int receiveValue = connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    if (receiveValue == INVALID_SOCKET) {
        std::cout << "peerNetWork connectToPeer(): socket " << ipAddress << ":" << port << " can't connected." << WSAGetLastError() << std::endl;
        return;
    } else {
        std::cout << "\npeerNetWork connectToPeer(): socket " << ipAddress << ":" << port << " connected.\n" << std::endl;
        peers.emplace_back(ipAddress + ":" + std::to_string(port));

        peerThread pt = peerThread(clientSocket, ipAddress);
        std::thread ptThread(&peerThread::run, pt);
        ptThread.detach();
        peerThreads.emplace_back(pt);
    }
}

/**
 * Listen on port 8015 as a server
 */
void peerNetwork::run() {
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == SOCKET_ERROR) {
        std::cout << "peerNetWork run(): Socket() error: " << WSAGetLastError() << std::endl;
        return;
    }

    SOCKADDR_IN serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(static_cast<u_short>(port));
    /*
     * INADDR_ANY就是指定地址为0.0.0.0的地址，
     * 这个地址事实上表示不确定地址，或“所有地址”、“任意地址”。
     * 一般来说，在各个系统中均定义成为0值。
     */
    serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

    int receiveValue;
    // socket绑定端口
    receiveValue = bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(SOCKADDR_IN));
    if (receiveValue == SOCKET_ERROR) {
        std::cout << "peerNetWork run(): port binding failed: " << WSAGetLastError() << std::endl;
        return;
    }

    // socket监听端口
    // 第二个参数backlog还不知道是什么意思
    receiveValue = listen(serverSocket, 10);
    if (receiveValue == SOCKET_ERROR) {
        std::cout << "peerNetWork run(): listening port failed： " << WSAGetLastError() << std::endl;
        return;
    }

    std::cout << "start listening :" + std::to_string(port) << std::endl;
    while (runFlag) {
        // 监听成功，等待Client端连接
        SOCKADDR_IN clientAddr;
        int lenSOCKADDR = sizeof(SOCKADDR);
        SOCKET connectSocket = accept(serverSocket, (SOCKADDR *) &clientAddr, &lenSOCKADDR);

        if (connectSocket == SOCKET_ERROR) {
            std::cout << "peerNetWork run(): failed accept: " << WSAGetLastError() << std::endl;
            std::cout << "peerNetWork run(): accept failed： " << WSAGetLastError() << std::endl;
        }
        char ipAddress[16] = {0};
        inet_ntop(AF_INET, &clientAddr.sin_addr, ipAddress, sizeof(ipAddress));
        std::cout << "\npeerNetWork run(): accept client IP: " << ipAddress << "\n"<< std::endl;

        peerThread peerThread1 = peerThread(connectSocket, ipAddress);
        peerThreads.emplace_back(peerThread1);

        std::cout<< "peerNetWork run(): number of accepted connection: " <<peerThreads.size() << std::endl;
        std::thread ptThread(&peerThread::run, peerThread1);
        ptThread.detach();
    }
}

/**
 * Broadcast messages to all clients
 * @param data will sent
 */
void peerNetwork::broadcast(const std::string& data) {
    for (auto pt: peerThreads) {
        std::cout << "peerNetWork run(): sent: " << data << std::endl;
        pt.send(data);
    }
}
