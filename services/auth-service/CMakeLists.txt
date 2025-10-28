cmake_minimum_required(VERSION 3.16)
project(auth-service LANGUAGES CXX)

# Find the Drogon and OpenSSL packages
find_package(
    Drogon CONFIG REQUIRED
    OpenSSL REQUIRED
)

file(GLOB_RECURSE HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/repository/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/routes/*.h"
)

file(GLOB_RECURSE CPP_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/repository/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/routes/*.cpp"
)
add_executable(auth-service
    main.cpp
    ${CPP_FILES}
    ${REPOSITORY_SOURCES}
    ${CMAKE_SOURCE_DIR}/common/Env/EnvLoader.cpp
    ${CMAKE_SOURCE_DIR}/common/db/DbConnection.cpp
)

# Include header files
target_include_directories(auth-service PRIVATE
    ${CMAKE_SOURCE_DIR}/common/Env/EnvLoader.h
    ${CMAKE_SOURCE_DIR}/common/db/DbConnection.h
)

# Link the necessary libraries to the target
target_link_libraries(auth-service
    PRIVATE Drogon::Drogon
    PRIVATE OpenSSL::Crypto
)
