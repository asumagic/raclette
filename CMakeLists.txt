# SPDX-License-Identifier: BSD-2-Clause

cmake_minimum_required(VERSION 3.10)

project(raclette)

add_executable(raclette
    src/main.cpp
    src/raclette/board.cpp
    src/raclette/debug.cpp
)

target_include_directories(raclette PRIVATE include/)

target_link_libraries(raclette fmt)

target_compile_options(raclette PRIVATE
    -fsanitize=address
)

target_link_options(raclette PRIVATE
    -fsanitize=address
)

set_target_properties(raclette PROPERTIES
    CXX_STANDARD 23
)