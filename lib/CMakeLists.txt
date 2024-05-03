# registers, interruption table and startup code
add_library(system STATIC)
file(GLOB SYSTEM_SRC system/*.c system/*.S)
target_include_directories(system PUBLIC system/include)
target_sources(system PRIVATE ${SYSTEM_SRC})
# somehow *.S won't be recognized as ASM, so we need to set it explicitly
set_property(SOURCE system/startup.S PROPERTY LANGUAGE ASM)
set_property(SOURCE system/vectors.S PROPERTY LANGUAGE ASM)

# basic HAL drivers
add_library(wm_drivers STATIC)
file(GLOB WM_DRIVERS_SRC drivers/*.c)
target_include_directories(wm_drivers PUBLIC drivers/include)
target_sources(wm_drivers PRIVATE ${WM_DRIVERS_SRC})
target_sources(wm_drivers PRIVATE port/libc_port.c)
target_link_libraries(wm_drivers system)

# DSP library
add_library(dsp INTERFACE)
target_include_directories(dsp INTERFACE dsp/include)
target_link_directories(dsp INTERFACE dsp)
