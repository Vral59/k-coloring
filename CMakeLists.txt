cmake_minimum_required(VERSION 3.26)
project(mh_project)

set(CMAKE_CXX_STANDARD 17)

add_executable(mh_project main.cpp
        src/Node.cpp
        include/Node.h
        src/Graph.cpp
        include/Graph.h)
