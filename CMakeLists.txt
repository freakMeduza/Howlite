cmake_minimum_required(VERSION 3.16)

project(Howlite LANGUAGES CXX)

set(VERSION "0.0.1")

# Build options
option(BUILD_MULTICORE "Build with multiple processes" ON)

message(STATUS "----- Build Config ----")
message(STATUS "Compiler=${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "BUILD_MULTICORE=${BUILD_MULTICORE}")

# Setup MSVC compiler flags
if(MSVC)
    if(BUILD_MULTICORE)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    endif()
  # Force to always compile with W4
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
endif()

# CXX settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set cmake module path
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

message(STATUS "CMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}")

# Extrenal includes
include_directories(
   ${PROJECT_SOURCE_DIR}
)

# External linkage
link_directories(
    
)

add_subdirectory(Engine)
add_subdirectory(Sandbox)



