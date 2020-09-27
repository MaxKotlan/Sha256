cmake_minimum_required (VERSION 2.8.11)
project (sha256)
set(CMAKE_CXX_FLAGS "-O3")

file(GLOB ${CMAKE_PROJECT_NAME}_SRC
    "*.h"
    "*.cpp"
)

add_executable(${CMAKE_PROJECT_NAME} ${${CMAKE_PROJECT_NAME}_SRC})