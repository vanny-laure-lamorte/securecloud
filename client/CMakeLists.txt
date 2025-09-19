cmake_minimum_required(VERSION 3.16)

find_package(Drogon CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)

add_library(clientlib
    src/ClientApp.cpp
)
target_include_directories(clientlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(clientlib PUBLIC Drogon::Drogon JsonCpp::JsonCpp)

add_executable(client
    src/main.cpp
)
target_link_libraries(client PRIVATE clientlib)
