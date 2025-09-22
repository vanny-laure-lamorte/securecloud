cmake_minimum_required(VERSION 3.10)

# PROJECT NAME
project(securecloud LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# MICROSERVICES

## Api Gateway
add_subdirectory(api-gateway)

## Authentication service
add_subdirectory(services/auth-service)

## Messaging service
add_subdirectory(services/messaging-service)

# Client
# Find required Qt6 modules
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets LinguistTools)
add_subdirectory(client)
