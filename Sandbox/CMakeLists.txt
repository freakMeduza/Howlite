cmake_minimum_required(VERSION 3.16)

project(Sandbox LANGUAGES CXX)

message(STATUS "----- ${PROJECT_NAME} -----")
message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_C_FLAGS=${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/..
)

file(GLOB_RECURSE HDRS "*.h")
file(GLOB_RECURSE SRCS "*.cpp")

add_executable(${PROJECT_NAME} WIN32 ${HDRS} ${SRCS})

target_link_libraries(${PROJECT_NAME} HowliteEngine)



