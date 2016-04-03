project(qTuner)
set(PROJECTVERSION 0.3b)
cmake_minimum_required(VERSION 2.8)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Multimedia REQUIRED)

add_subdirectory(src)
