cmake_minimum_required(VERSION 3.10)

project(Threadpool CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
    message(FATAL_ERROR "Please build the binaries in a separate directory")
endif()

set(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR})


include_directories(include)
add_subdirectory(include)
add_subdirectory(tests)