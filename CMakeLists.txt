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

add_custom_command(
        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/demo.bin"
        COMMAND ${TOOLCHAIN_PREFIX}/bin/${TOOLCHAIN}-objcopy -Obinary
        "${CMAKE_CURRENT_BINARY_DIR}/demo.elf"
        "${CMAKE_CURRENT_BINARY_DIR}/demo.bin"
        DEPENDS demo.elf)

find_program(WM_TOOL wm_tool)
if (NOT WM_TOOL)
    message(WARNING "wm_tool not found. Image generation will not be available.")
else ()
    add_custom_command(
            OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/demo.img"
            COMMAND ${WM_TOOL} -b ${CMAKE_CURRENT_BINARY_DIR}/demo.bin -o ${CMAKE_CURRENT_BINARY_DIR}/demo -it 1 -fc 0 -ra 0x080D0400 -ih 0x080D0000 -ua 0x08010000 -nh 0 -un 0
            DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/demo.bin")
    add_custom_target(
            img ALL
            DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/demo.img"
            COMMENT "Building image file")
endif ()

add_custom_target(
        bin ALL
        DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/demo.bin"
        COMMENT "Building binary file")

# Echo the size Information
add_custom_target(
        size
        ${TOOLCHAIN_PREFIX}/bin/${TOOLCHAIN}-size "${CMAKE_CURRENT_BINARY_DIR}/demo.elf"
        DEPENDS demo.elf)
