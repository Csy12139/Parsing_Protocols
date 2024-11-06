#include "server.h"

int main() {
    const int port = 8080; // 定义服务器端口

    try {
        initializeWinsock(); // 初始化 Winsock
        SOCKET serverSocket = createServerSocket(); // 创建服务器 socket
        bindSocket(serverSocket, port); // 绑定 socket
        listenForConnections(serverSocket); // 开始监听连接

        std::cout << "Waiting for connections on port " << port << "..." << std::endl;

        sockaddr_in clientAddr; // 定义客户端地址结构
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (sockaddr *) &clientAddr, &clientAddrSize); // 接受客户端连接
        if (clientSocket == INVALID_SOCKET) {
            throw std::runtime_error("Accept failed."); // 接受连接失败时抛出异常
        }

        // 创建线程用于处理接收和发送消息
        std::thread receiveThread(handleClient, clientSocket); // 接收消息的线程
        std::thread sendThread(sendMessage, clientSocket); // 发送消息的线程

        receiveThread.join(); // 等待接收线程结束
        sendThread.join(); // 等待发送线程结束

        cleanup(serverSocket, clientSocket); // 清理资源
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl; // 输出错误信息
        return 1; // 返回错误码
    }

    return 0; // 正常结束
}