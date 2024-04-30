add_library(system STATIC)
file(GLOB SYSTEM_SRC system/*.c system/*.S)
target_include_directories(system PUBLIC system/include)
target_sources(system PRIVATE ${SYSTEM_SRC})

add_library(wm_drivers STATIC)
file(GLOB WM_DRIVERS_SRC drivers/*.c)
target_include_directories(wm_drivers PUBLIC drivers/include)
target_sources(wm_drivers PRIVATE ${WM_DRIVERS_SRC})
target_link_libraries(wm_drivers system)

add_library(wm_port STATIC)
target_sources(wm_port PRIVATE port/libc_port.c)
target_link_libraries(wm_port wm_drivers)
