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

set(WM_SECBOOT_NAME w800_secboot)
set(SEC_IMG_HEADER 0x08002000)
set(SEC_IMG_POS    0x08002400)
set(RUN_IMG_HEADER 0x080D0000) # image header
set(RUN_IMG_POS    0x080D0400) # image app address
set(UPD_IMG_POS    0x08010000) # image update address/OTA
find_program(WM_TOOL wm_tool)
if (NOT WM_TOOL)
    message(WARNING "wm_tool not found. Image generation will not be available.")
else ()
    add_custom_command(
            OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/demo.img
            COMMAND ${WM_TOOL} -b ${CMAKE_CURRENT_BINARY_DIR}/demo.bin -o ${CMAKE_CURRENT_BINARY_DIR}/demo -it 1 -fc 0 -ra ${RUN_IMG_POS} -ih ${RUN_IMG_HEADER} -ua ${UPD_IMG_POS} -nh 0 -un 0
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/demo.bin)
    file(COPY misc/${WM_SECBOOT_NAME}.bin DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${WM_SECBOOT_NAME}.img
            COMMAND 
            ${WM_TOOL} -b ${CMAKE_CURRENT_BINARY_DIR}/${WM_SECBOOT_NAME}.bin -o ${CMAKE_CURRENT_BINARY_DIR}/${WM_SECBOOT_NAME} -it 0 -fc 0 -ra ${SEC_IMG_POS} -ih ${SEC_IMG_HEADER} -ua ${UPD_IMG_POS} -nh ${RUN_IMG_HEADER} -un 0
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${WM_SECBOOT_NAME}.bin)
    # https://github.com/github0null/w800_cmake_sdk/blob/e8d91ca604f0b623d9097273bdb69ebaf47785ee/tools/make_firmware.sh.in#L103C18-L103C30
    # https://github.com/github0null/w800_cmake_sdk/blob/e8d91ca604f0b623d9097273bdb69ebaf47785ee/tools/w800/wm_tool.c#L3483-L3569
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/demo.fls
        COMMAND cat ${CMAKE_CURRENT_BINARY_DIR}/${WM_SECBOOT_NAME}.img ${CMAKE_CURRENT_BINARY_DIR}/demo.img > ${CMAKE_CURRENT_BINARY_DIR}/demo.fls
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${WM_SECBOOT_NAME}.img ${CMAKE_CURRENT_BINARY_DIR}/demo.img
    )
    add_custom_target(
            img
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/demo.img
            COMMENT "Building image file")
    add_custom_target(
        fls ALL
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/demo.fls
        COMMENT "Building fls file")
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
