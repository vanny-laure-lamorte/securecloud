cmake_minimum_required(VERSION 3.16)

project(messaging-service LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Drogon CONFIG REQUIRED)

set(SRC_FILES
    src/main.cpp
    src/MessagingWs.cpp
    src/repository/GroupRepository.cpp
    src/repository/MessageRepository.cpp
)

set(COMMON_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../../common/Env/EnvLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../common/db/DbConnection.cpp
)

add_executable(messaging-service
    ${SRC_FILES}
    ${COMMON_SOURCES}
)

target_include_directories(messaging-service
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(messaging-service
    PRIVATE Drogon::Drogon
)
