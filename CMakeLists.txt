cmake_minimum_required(VERSION 3.20)
set(CMAKE_TOOLCHAIN_FILE
        ${CMAKE_CURRENT_LIST_DIR}/toolchain-csky-unknown-elf.cmake)
project(demo)
enable_language(C CXX ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(lib)
add_executable(demo.elf
        src/core.cpp
        src/main.cpp
        src/interruption.cpp)
target_link_libraries(demo.elf wm_drivers wm_port dsp)
target_include_directories(demo.elf PRIVATE src/include)

set(LD_SCRIPT_PATH gcc_csky.ld)
get_filename_component(LD_SCRIPT_PATH_ABSOLUTE ${LD_SCRIPT_PATH} ABSOLUTE)
set_target_properties(demo.elf PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS ON
        LINK_DEPENDS ${LD_SCRIPT_PATH_ABSOLUTE}
        LINK_FLAGS "-T \"${LD_SCRIPT_PATH_ABSOLUTE}\"")
