cmake_minimum_required(VERSION 3.16)
project(api_gateway LANGUAGES CXX)

find_package(Drogon CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)

add_executable(api-gateway src/main.cpp)
target_link_libraries(api-gateway PRIVATE Drogon::Drogon JsonCpp::JsonCpp)
