cmake_minimum_required(VERSION 3.16)
project(client LANGUAGES CXX)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 Core and LinguistTools modules (LinguistTools needed for lrelease)
find_package(Qt6 REQUIRED COMPONENTS Core LinguistTools)

# Define the executable target named 'client' with the main source file
add_executable(client
    src/main.cpp
)

# Link Qt Core library to the client executable
target_link_libraries(client PRIVATE Qt6::Core)

# List of translation source (.ts) files to process
set(TS_FILES
    i18n/fr_FR.ts
    i18n/es_ES.ts
    i18n/en_EN.ts
)

# Initialize the list of compiled translation (.qm) files
set(QM_FILES)

# For each .ts file, create a corresponding .qm file in the build directory
foreach(ts_file IN LISTS TS_FILES)
    get_filename_component(lang ${ts_file} NAME_WE)
    set(qm_file "${CMAKE_CURRENT_BINARY_DIR}/i18n/${lang}.qm")

    add_custom_command(
        OUTPUT ${qm_file}
        COMMAND ${Qt6_LRELEASE_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/${ts_file} -qm ${qm_file}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${ts_file}
        COMMENT "Compiling translation ${ts_file}"
        VERBATIM
    )

    list(APPEND QM_FILES ${qm_file})
endforeach()

# Create a custom target 'translations' that depends on all .qm files
# This ensures translation files are built automatically
add_custom_target(translations ALL DEPENDS ${QM_FILES})

# Make sure 'client' target is built after translations are generated
add_dependencies(client translations)

# After building the client executable, copy the compiled .qm files next to it
add_custom_command(TARGET client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_BINARY_DIR}/i18n
    $<TARGET_FILE_DIR:client>/i18n
    COMMENT "Copying translation (.qm) files to executable directory"
)
