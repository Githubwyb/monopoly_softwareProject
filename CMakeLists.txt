cmake_minimum_required(VERSION 3.12)
project(version_1_5 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(version_1_5
        cmd.c
        cmd.h
        drawMap.c
        drawMap.h
        events.c
        events.h
        game.c
        game.h)
