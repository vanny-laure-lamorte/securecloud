cmake_minimum_required(VERSION 3.16)
project(client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/Zc:__cplusplus /permissive-)
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ---- Dépendances ----
find_package(Drogon CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets LinguistTools WebSockets)

# ---- Sources ----
file(GLOB_RECURSE SRC_FILES
    src/*.cpp
    ${CMAKE_SOURCE_DIR}/common/src/TranslationManager.cpp
)

file(GLOB_RECURSE HEADER_FILES
    include/*.h
    ${CMAKE_SOURCE_DIR}/common/include/TranslationManager.h
)

# ---- Exécutable ----
add_executable(client ${SRC_FILES} ${HEADER_FILES})

target_include_directories(client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/common/include
)

target_link_libraries(client
    PRIVATE
        Drogon::Drogon
        JsonCpp::JsonCpp
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::WebSockets
)

# ---- Translations ----
set(TS_FILES
    ${CMAKE_SOURCE_DIR}/common/i18n/fr_FR.ts
    ${CMAKE_SOURCE_DIR}/common/i18n/es_ES.ts
    ${CMAKE_SOURCE_DIR}/common/i18n/en_US.ts
)

get_target_property(_lrelease_location Qt6::lrelease LOCATION)
if (NOT _lrelease_location)
    message(FATAL_ERROR "Could not find lrelease tool (Qt6::lrelease)")
endif()

set(QM_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/i18n)
file(MAKE_DIRECTORY ${QM_OUTPUT_DIR})

set(QM_FILES)
foreach(ts_file IN LISTS TS_FILES)
    get_filename_component(lang ${ts_file} NAME_WE)
    set(qm_file "${QM_OUTPUT_DIR}/${lang}.qm")

    add_custom_command(
        OUTPUT ${qm_file}
        COMMAND ${_lrelease_location} ${ts_file} -qm ${qm_file}
        DEPENDS ${ts_file}
        COMMENT "Compiling translation ${ts_file} to ${qm_file}"
        VERBATIM
    )

    list(APPEND QM_FILES ${qm_file})
endforeach()

add_custom_target(translations ALL DEPENDS ${QM_FILES})
add_dependencies(client translations)

add_custom_command(TARGET client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${QM_OUTPUT_DIR}
            $<TARGET_FILE_DIR:client>/i18n
)
