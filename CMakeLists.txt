cmake_minimum_required(VERSION 3.26)
project(Parsing_Protocols)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(server
        server/server.cpp
)
target_link_libraries(server ws2_32)  # 添加 Winsock 库链接

add_executable(client
        client/client.cpp
)
target_link_libraries(client ws2_32)  # 添加 Winsock 库链接
