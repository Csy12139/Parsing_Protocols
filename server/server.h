
#ifndef PARSING_PROTOCOLS_SERVER_H
#define PARSING_PROTOCOLS_SERVER_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

// 初始化 Winsock 库
void initializeWinsock() {
    WSADATA wsaData;
    // 初始化 Winsock，指定使用版本 2.2
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw std::runtime_error("WSAStartup failed."); // 初始化失败时抛出异常
    }
}

// 创建服务器 socket
SOCKET createServerSocket() {
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0); // 创建 TCP socket
    if (serverSocket == INVALID_SOCKET) {
        throw std::runtime_error("Socket creation failed."); // 创建失败时抛出异常
    }
    return serverSocket; // 返回有效的 socket 描述符
}

// 绑定 socket 到指定端口
void bindSocket(SOCKET serverSocket, int port) {
    sockaddr_in serverAddr; // 定义服务器地址结构
    serverAddr.sin_family = AF_INET; // 使用 IPv4
    serverAddr.sin_addr.s_addr = INADDR_ANY; // 监听所有可用接口
    serverAddr.sin_port = htons(port); // 转换端口为网络字节序

    // 绑定 socket
    if (bind(serverSocket, (sockaddr *) &serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        throw std::runtime_error("Bind failed."); // 绑定失败时抛出异常
    }
}

// 开始监听连接请求
void listenForConnections(SOCKET serverSocket) {
    // 开始监听连接，允许最多 5 个连接排队
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        throw std::runtime_error("Listen failed."); // 监听失败时抛出异常
    }
}

// 处理客户端的消息
void handleClient(SOCKET clientSocket) {
    char buffer[1024] = {0}; // 缓冲区用于接收消息
    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0); // 接收客户端消息
        if (bytesReceived > 0) {
            std::cout << "Client: " << buffer << std::endl; // 打印接收到的消息
            memset(buffer, 0, sizeof(buffer)); // 清空缓冲区以便接收下条消息
        }
    }
}

// 发送消息到客户端
void sendMessage(SOCKET clientSocket) {
    while (true) {
        std::string message;
        std::cout << "Server: "; // 提示用户输入消息
        std::getline(std::cin, message); // 从命令行读取输入
        send(clientSocket, message.c_str(), message.size(), 0); // 发送消息到客户端
    }
}

// 清理资源
void cleanup(SOCKET serverSocket, SOCKET clientSocket) {
    closesocket(clientSocket); // 关闭客户端 socket
    closesocket(serverSocket); // 关闭服务器 socket
    WSACleanup(); // 清理 Winsock
}

#endif //PARSING_PROTOCOLS_SERVER_H
