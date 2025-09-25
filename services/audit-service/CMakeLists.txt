cmake_minimum_required(VERSION 3.16)
project(audit-service LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

find_package(Drogon CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)

add_executable(audit-service
    src/main.cpp
    src/AuditRoutes.cpp
    src/AuditService.cpp
    ${CMAKE_SOURCE_DIR}/common/db/DbConnection.cpp
)

target_include_directories(audit-service PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(audit-service PRIVATE Drogon::Drogon JsonCpp::JsonCpp)

set_target_properties(audit-service PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/deploy/bin"
)