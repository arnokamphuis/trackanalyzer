cmake_minimum_required(VERSION 2.8.4)
project(trackanalyzer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Analyzer.cpp Location.cpp)
add_executable(trackanalyzer ${SOURCE_FILES})
target_link_libraries(trackanalyzer xerces-c)