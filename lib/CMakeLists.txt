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

# DSP library
add_library(dsp INTERFACE)
target_include_directories(dsp INTERFACE dsp/include)
target_link_directories(dsp INTERFACE dsp)

add_library(freeRTOS STATIC)
file(GLOB_RECURSE FREERTOS_PORT_SRC FreeRTOS-Port/*.c FreeRTOS-Port/*.S)
target_include_directories(freeRTOS PUBLIC FreeRTOS-Port/portable/xt804)
file(GLOB FREERTOS_SRC FreeRTOS-Kernel/*.c)
target_include_directories(freeRTOS PUBLIC FreeRTOS-Kernel/include)
target_sources(freeRTOS PRIVATE ${FREERTOS_SRC} ${FREERTOS_PORT_SRC})
# memory management
target_sources(freeRTOS PRIVATE FreeRTOS-Kernel/portable/MemMang/heap_5.c)
set_property(SOURCE FreeRTOS-Port/portable/xt804/cpu_task_sw.S PROPERTY LANGUAGE ASM)
target_link_libraries(freeRTOS wmdrivers)

add_library(freeRTOS-vendor STATIC)
file(GLOB_RECURSE FREERTOS_VENDOR_SRC FreeRTOS-Vendor/*.c FreeRTOS-Vendor/*.S)
target_include_directories(freeRTOS-vendor PUBLIC FreeRTOS-Vendor/include)
target_include_directories(freeRTOS-vendor PUBLIC FreeRTOS-Vendor/portable/xt804)
target_sources(freeRTOS-vendor PRIVATE ${FREERTOS_VENDOR_SRC})
set_property(SOURCE FreeRTOS-Vendor/portable/xt804/cpu_task_sw.S PROPERTY LANGUAGE ASM)
target_link_libraries(freeRTOS-vendor wmdrivers)
