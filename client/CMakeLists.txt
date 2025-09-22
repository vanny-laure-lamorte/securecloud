cmake_minimum_required(VERSION 3.16)
project(client LANGUAGES CXX)

# Set the C++ standard to C++17 and require it
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compile flags for MSVC
if (MSVC)
    add_compile_options(/Zc:__cplusplus /permissive-)
endif()

# Find required Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets LinguistTools)

# Define the executable
add_executable(client
    src/main.cpp
     ${CMAKE_SOURCE_DIR}/common/src/TranslationManager.cpp
)

set(HEADER_FILES
    common/TranslationManager.h
)

# Link Qt libraries
target_link_libraries(client
    PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui
)

# Translation source files
set(TS_FILES
    ${CMAKE_SOURCE_DIR}/common/i18n/fr_FR.ts
    ${CMAKE_SOURCE_DIR}/common/i18n/es_ES.ts
    ${CMAKE_SOURCE_DIR}/common/i18n/en_US.ts
)

# Ensure lrelease is found
get_target_property(_lrelease_location Qt6::lrelease LOCATION)
if (NOT _lrelease_location)
    message(FATAL_ERROR "Could not find lrelease tool (Qt6::lrelease)")
endif()

# Output directory for .qm files
set(QM_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/i18n)
file(MAKE_DIRECTORY ${QM_OUTPUT_DIR})

# List to collect .qm files
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

# Add custom target to generate translations
add_custom_target(translations ALL DEPENDS ${QM_FILES})

# Make the executable depend on translations
add_dependencies(client translations)

# Copy .qm files into the executable folder after build
add_custom_command(TARGET client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${QM_OUTPUT_DIR}
            $<TARGET_FILE_DIR:client>/i18n
)
