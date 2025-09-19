cmake_minimum_required(VERSION 3.10)

project(messaging-service LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/Zc:__cplusplus /permissive-)
endif()


find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

add_executable(messaging-service
    src/main.cpp
)

target_link_libraries(messaging-service PRIVATE Qt6::Core Qt6::Widgets)

set_target_properties(messaging-service PROPERTIES
    AUTOMOC ON
    AUTORCC ON
    AUTOUIC ON
)


add_custom_command(TARGET messaging-service POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:messaging-service>/i18n
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/client/i18n
        $<TARGET_FILE_DIR:messaging-service>/i18n
    COMMENT "Copie des fichiers de traduction (QM) du client vers messaging-service"
)
