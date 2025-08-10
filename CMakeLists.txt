cmake_minimum_required(VERSION 3.31)
project(BugsLife2)

set(CMAKE_CXX_STANDARD 20)

add_executable(BugsLife2
        main.cpp
        Bug.cpp
        Crawler.cpp
        Hopper.cpp
        Board.cpp
)
