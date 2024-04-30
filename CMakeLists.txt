cmake_minimum_required(VERSION 3.20)
set(CMAKE_TOOLCHAIN_FILE
        ${CMAKE_CURRENT_LIST_DIR}/toolchain-csky-unknown-elf.cmake)
project(demo)

add_subdirectory(lib)
add_executable(demo.elf src/main.c)
target_link_libraries(demo.elf wm_drivers wm_port)
