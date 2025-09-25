cmake_minimum_required(VERSION 3.16)
project(auth-service LANGUAGES CXX)

# Find the Drogon and OpenSSL packages
find_package(
    Drogon CONFIG REQUIRED
    OpenSSL REQUIRED
)

# Define the executable target 'auth-service' with its source files
add_executable(auth-service
    src/main.cpp
    src/AuthServiceRoutes.cpp
    ${CMAKE_SOURCE_DIR}/common/HashUtils/HashUtils.cpp
    ${CMAKE_SOURCE_DIR}/common/Env/EnvLoader.cpp
    ${CMAKE_SOURCE_DIR}/common/db/DbConnection.cpp
)

# Include header files
target_include_directories(auth-service PRIVATE
    ${CMAKE_SOURCE_DIR}/common/HashUtils.h
    ${CMAKE_SOURCE_DIR}/common/Env/EnvLoader.h
    ${CMAKE_SOURCE_DIR}/common/db/DbConnection.h
)

# Link the necessary libraries to the target
target_link_libraries(auth-service
    PRIVATE Drogon::Drogon
    PRIVATE OpenSSL::Crypto
)
