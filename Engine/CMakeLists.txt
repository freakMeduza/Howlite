cmake_minimum_required(VERSION 3.16)

project(HowliteEngine LANGUAGES CXX)

# CXX settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_definitions(-DHL_EXPORT)

find_package(DirectX11 REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${DirectX11_INCLUDE_DIR}
)

message(STATUS "----- ${PROJECT_NAME} -----")
message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_C_FLAGS=${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")

file(GLOB_RECURSE HDRS "*.h")
file(GLOB_RECURSE SRCS "*.cpp")

add_library(${PROJECT_NAME} SHARED ${HDRS} ${SRCS})

set_target_properties(${PROJECT_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

target_link_libraries(${PROJECT_NAME} PUBLIC ${DirectX11_LIBRARIES})

