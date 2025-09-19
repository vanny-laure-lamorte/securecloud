cmake_minimum_required(VERSION 3.10)
project(securecloud LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- OUTPUT DIRECTORY FOR EXE ---
set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/deploy/bin")
file(MAKE_DIRECTORY "${OUTPUT_DIR}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}")

if(CMAKE_CONFIGURATION_TYPES)
  foreach(cfg ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER "${cfg}" cfgU)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${cfgU} "${OUTPUT_DIR}")
  endforeach()
endif()

# --- projects ---
add_subdirectory(api-gateway)
add_subdirectory(services/auth-service)
add_subdirectory(services/messaging-service)
add_subdirectory(client)
