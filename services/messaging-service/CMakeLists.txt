cmake_minimum_required(VERSION 3.10)

project(messaging-service LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(messaging-service
    src/main.cpp
)