cmake_minimum_required(VERSION 3.16)
project(client LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Flags pour MSVC
if (MSVC)
    add_compile_options(/Zc:__cplusplus /permissive-)
endif()

# Qt + outils de traduction
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets LinguistTools)

# Chemin vers les fichiers TS
set(TS_FILES
    ${CMAKE_SOURCE_DIR}/common/i18n/fr_FR.ts
    ${CMAKE_SOURCE_DIR}/common/i18n/es_ES.ts
    ${CMAKE_SOURCE_DIR}/common/i18n/en_US.ts
)

set(QM_FILES)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/i18n)

foreach(ts_file IN LISTS TS_FILES)
    get_filename_component(lang ${ts_file} NAME_WE)
    set(qm_file "${CMAKE_CURRENT_BINARY_DIR}/i18n/${lang}.qm")

    add_custom_command(
        OUTPUT ${qm_file}
        COMMAND ${Qt6_LRELEASE_EXECUTABLE} ARGS ${ts_file} -qm ${qm_file}
        DEPENDS ${ts_file}
        COMMENT "Compiling translation ${ts_file}"
        VERBATIM
    )

    list(APPEND QM_FILES ${qm_file})
endforeach()

# Cible de traduction
add_custom_target(translations ALL DEPENDS ${QM_FILES})

# Exécutable principal
add_executable(client src/main.cpp)
target_link_libraries(client PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui)
add_dependencies(client translations)

# Copie des fichiers .qm après compilation
add_custom_command(TARGET client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:client>/i18n
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_BINARY_DIR}/i18n
            $<TARGET_FILE_DIR:client>/i18n
    COMMENT "Copying .qm files"
)
