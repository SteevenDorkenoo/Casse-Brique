cmake_minimum_required(VERSION 3.16)
project(bombermandem C)

set(CMAKE_C_STANDARD 99)

include_directories(src/includes)

FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/maps/ DESTINATION ./maps/)

add_executable(bombermandem src/main.c src/game_run/game_run.c src/game_init/game_init.c src/helper_funcs/helper_funcs.c src/includes/helper_funcs.h)
