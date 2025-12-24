cmake_minimum_required(VERSION 3.12)

# Project name
project(sss-commons VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Sources
file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/DemoMain.cpp)

# DLL
add_compile_definitions(SSS_COMMONS_EXPORTS)
add_library(sss-commons SHARED ${SRC_FILES})

# Headers
target_include_directories(sss-commons PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

# glm
find_package(glm CONFIG REQUIRED)
target_link_libraries(sss-commons PRIVATE glm::glm)

# Windows only
target_link_libraries(sss-commons PUBLIC winmm)
