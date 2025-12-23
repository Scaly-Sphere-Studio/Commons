cmake_minimum_required(VERSION 3.12)

# set the project name and version
project(sss-commons VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/DemoMain.cpp)

add_compile_definitions(SSS_COMMONS_EXPORTS)

# add the executable
add_library(sss-commons SHARED ${SRC_FILES})

target_include_directories(sss-commons PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

target_link_libraries(sss-commons winmm)

