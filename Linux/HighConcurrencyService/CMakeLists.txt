cmake_minimum_required(VERSION 3.20)
project(CSProject C)

set(CMAKE_C_STANDARD 11)
# 查找 pthread 库
find_package(Threads REQUIRED)
add_executable(CSProject main.c
        server.h
        server.c
        hash_map.h
        hash_map.c)

# 将 pthread 链接到项目中
target_link_libraries(CSProject PRIVATE Threads::Threads)