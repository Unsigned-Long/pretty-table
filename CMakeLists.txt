cmake_minimum_required(VERSION 3.10)

project(prettytable VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_BUILD_TYPE "Build")

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(${CMAKE_PROJECT_NAME} ${CMAKE_SOURCE_DIR}/main.cpp)

set(CMAKE_INSTALL_PREFIX /usr/local)

install(FILES ${CMAKE_SOURCE_DIR}/include/prettytable.hpp DESTINATION ${CMAKE_INSTALL_PREFIX}/include/artwork/pretty-table)