cmake_minimum_required(VERSION 3.16)

project(api_gateway LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Drogon CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)

set(SRC_FILES
    src/main.cpp
    src/GatewayWs.cpp
)

set(HEADER_FILES
    include/GatewayWs.h
)

add_executable(api-gateway
    ${SRC_FILES}
    ${HEADER_FILES}
)

target_include_directories(api-gateway PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(api-gateway
    PRIVATE
        Drogon::Drogon
        JsonCpp::JsonCpp
)