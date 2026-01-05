cmake_minimum_required(VERSION 3.16)
project(auth-service LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the Drogon and OpenSSL packages
find_package(
    Drogon CONFIG REQUIRED
    OpenSSL REQUIRED
)

file(GLOB_RECURSE HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/repository/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/routes/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/service/*.h"
)

file(GLOB_RECURSE CPP_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/repository/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/routes/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/service/*.cpp"
)

set(COMMON_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../../common/Env/EnvLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../common/db/DbConnection.cpp
)

add_executable(auth-service
    main.cpp
    ${CPP_FILES}
    ${COMMON_SOURCES}
)

# Include header files
target_include_directories(auth-service PRIVATE
    ${HEADER_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../common
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

# Link the necessary libraries to the target
target_link_libraries(auth-service
    PRIVATE Drogon::Drogon
    PRIVATE OpenSSL::Crypto
    PRIVATE PostgreSQL::PostgreSQL
)

set_target_properties(auth-service PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build"
)