cmake_minimum_required(VERSION 3.10.0)
project("aztheria_and_elendor")

add_subdirectory("3rdparty/bearlibterminal")
include_directories("3rdparty/bearlibterminal/Terminal/Include/C")

file(GLOB SOURCES "src/main.cpp")

add_executable(aztheria ${SOURCES})
