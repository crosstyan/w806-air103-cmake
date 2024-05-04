add_library(xt804 INTERFACE)
target_include_directories(xt804 INTERFACE system/include)

# registers, interruption table and startup code
add_library(wmarch STATIC)
file(GLOB SYSTEM_SRC system/*.c system/*.S)
target_include_directories(wmarch PUBLIC system/include)
target_sources(wmarch PRIVATE ${SYSTEM_SRC})
# somehow *.S won't be recognized as ASM, so we need to set it explicitly
set_property(SOURCE system/startup.S PROPERTY LANGUAGE ASM)
set_property(SOURCE system/vectors.S PROPERTY LANGUAGE ASM)
target_link_libraries(wmarch xt804)
set_target_properties(wmarch PROPERTIES
    LINKER_FLAGS "-Wl,--whole-archive")

# basic HAL drivers
add_library(wmdrivers STATIC)
file(GLOB WM_DRIVERS_SRC drivers/*.c drivers/*.cpp)
target_include_directories(wmdrivers PUBLIC drivers/include)
target_sources(wmdrivers PRIVATE ${WM_DRIVERS_SRC})
target_link_libraries(wmdrivers wmarch)
set_target_properties(wmdrivers PROPERTIES
    LINKER_FLAGS "-Wl,--whole-archive")

add_library(wmlibc STATIC)
target_sources(wmlibc PRIVATE port/libc_port.c)
target_link_libraries(wmlibc xt804 wmdrivers)
set_target_properties(wmlibc PROPERTIES
    LINKER_FLAGS "-Wl,--whole-archive wmlibc -Wl,--no-whole-archive")

# DSP library
add_library(dsp INTERFACE)
target_include_directories(dsp INTERFACE dsp/include)
target_link_directories(dsp INTERFACE dsp)

add_library(freeRTOS STATIC)
file(GLOB_RECURSE FREERTOS_SRC FreeRTOS/*.c FreeRTOS/*.S)
target_include_directories(freeRTOS PUBLIC FreeRTOS/include)
target_include_directories(freeRTOS PUBLIC FreeRTOS/portable/xt804)
set_property(SOURCE FreeRTOS/portable/xt804/cpu_task_sw.S PROPERTY LANGUAGE ASM)
target_sources(freeRTOS PRIVATE ${FREERTOS_SRC})
target_link_libraries(freeRTOS wmarch)
set_target_properties(freeRTOS PROPERTIES 
    LINKER_FLAGS "-Wl,--whole-archive")
