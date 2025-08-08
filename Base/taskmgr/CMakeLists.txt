cmake_minimum_required(VERSION 3.28)
project(day37_taskmgr)

set(CMAKE_CXX_STANDARD 17)

add_executable(day37_taskmgr main.cpp
        Logger.cpp
        Logger.h
        Task.cpp
        Task.h
        TaskManager.cpp
        TaskManager.h
        Command.cpp
        Command.h)
