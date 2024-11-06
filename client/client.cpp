#include "client.h"

int main() {
    const char *serverIp = "127.0.0.1"; // 定义服务器 IP 地址
    const int port = 8080; // 定义服务器端口

    try {
        initializeWinsock(); // 初始化 Winsock
        SOCKET clientSocket = createClientSocket(); // 创建客户端 socket
        connectToServer(clientSocket, serverIp, port); // 连接到服务器

        // 创建线程用于发送和接收消息
        std::thread receiveThread(receiveMessages, clientSocket); // 接收消息的线程
        std::thread sendThread(sendMessages, clientSocket); // 发送消息的线程

        receiveThread.join(); // 等待接收线程结束
        sendThread.join(); // 等待发送线程结束

        cleanup(clientSocket); // 清理资源
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl; // 输出错误信息
        return 1; // 返回错误码
    }

    return 0; // 正常结束
}