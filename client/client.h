#ifndef PARSING_PROTOCOLS_CLIENT_H
#define PARSING_PROTOCOLS_CLIENT_H

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

// 创建客户端 socket
SOCKET createClientSocket() {
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0); // 创建 TCP socket
    if (clientSocket == INVALID_SOCKET) {
        throw std::runtime_error("Socket creation failed."); // 创建失败时抛出异常
    }
    return clientSocket; // 返回有效的 socket 描述符
}

// 连接到服务器
void connectToServer(SOCKET clientSocket, const char *ip, int port) {
    sockaddr_in serverAddr; // 定义服务器地址结构
    serverAddr.sin_family = AF_INET; // 使用 IPv4
    serverAddr.sin_port = htons(port); // 转换端口为网络字节序
    inet_pton(AF_INET, ip, &serverAddr.sin_addr); // 将 IP 地址从文本转换为二进制

    // 连接到服务器
    if (connect(clientSocket, (sockaddr *) &serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        throw std::runtime_error("Connection failed."); // 连接失败时抛出异常
    }
}

// 接收服务器发送的消息
void receiveMessages(SOCKET clientSocket) {
    char buffer[1024] = {0}; // 缓冲区用于接收消息
    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0); // 接收服务器消息
        if (bytesReceived > 0) {
            std::cout << "Server: " << buffer << std::endl; // 打印接收到的消息
            memset(buffer, 0, sizeof(buffer)); // 清空缓冲区以便接收下条消息
        }
    }
}

// 发送消息到服务器
void sendMessages(SOCKET clientSocket) {
    while (true) {
        std::string message;
        std::cout << "Client: "; // 提示用户输入消息
        std::getline(std::cin, message); // 从命令行读取输入
        send(clientSocket, message.c_str(), message.size(), 0); // 发送消息到服务器
    }
}

// 清理资源
void cleanup(SOCKET clientSocket) {
    closesocket(clientSocket); // 关闭客户端 socket
    WSACleanup(); // 清理 Winsock
}

#endif //PARSING_PROTOCOLS_CLIENT_H
