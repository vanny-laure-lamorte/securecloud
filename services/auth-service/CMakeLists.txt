cmake_minimum_required(VERSION 3.16)
project(auth-service LANGUAGES CXX)

# Find the Drogon and OpenSSL packages
find_package(
    Drogon CONFIG REQUIRED
    OpenSSL REQUIRED
)

# FetchContent to bring in dotenv-cpp
include(FetchContent)
FetchContent_Declare(
    dotenv-cpp
    GIT_REPOSITORY https://github.com/laserpants/dotenv-cpp.git
    GIT_TAG        v0.2.0
)
FetchContent_MakeAvailable(dotenv-cpp)

# Define the executable target 'auth-service' with its source files
add_executable(auth-service
    src/main.cpp
    src/AuthServiceRoutes.cpp
    ${CMAKE_SOURCE_DIR}/common/HashUtils/HashUtils.cpp
)

# Link the necessary libraries to the target
target_link_libraries(auth-service
    PRIVATE Drogon::Drogon
    PRIVATE OpenSSL::Crypto
    PRIVATE dotenv-cpp::dotenv-cpp
)
