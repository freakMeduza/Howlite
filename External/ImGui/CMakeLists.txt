cmake_minimum_required(VERSION 3.16)

project(ImGui)

file(GLOB HDRS "*.h")
file(GLOB SRCS "*.cpp")

add_library(${PROJECT_NAME} STATIC ${HDRS} ${SRCS})
