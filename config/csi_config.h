#ifndef __CSI_CONFIG_H__
#define __CSI_CONFIG_H__

/**
 * uncomment the following line to disable Kernel
#ifndef CONFIG_KERNEL_NONE
#define CONFIG_KERNEL_NONE 1
#endif
**/

#ifdef CONFIG_KERNEL_NONE
#undef CONFIG_KERNEL_NONE
#endif

// clang-format off
#if CONFIG_KERNEL_NONE
  #ifdef CONFIG_KERNEL_FREERTOS
  #undef CONFIG_KERNEL_FREERTOS
  #endif
#else
  #ifndef CONFIG_KERNEL_FREERTOS
  #define CONFIG_KERNEL_FREERTOS 1
  #endif
#endif
// clang-format on

#ifndef CONFIG_CHIP_SL04
#define CONFIG_CHIP_SL04 1
#endif

#ifndef CONFIG_HAVE_VIC
#define CONFIG_HAVE_VIC 1
#endif

#ifndef CONFIG_SEPARATE_IRQ_SP
#define CONFIG_SEPARATE_IRQ_SP 1
#endif

#ifndef CONFIG_ARCH_INTERRUPTSTACK
#define CONFIG_ARCH_INTERRUPTSTACK 4096
#endif

#ifndef CONFIG_IRQ_VECTOR_SIZE
#define CONFIG_IRQ_VECTOR_SIZE 256
#endif

#ifndef USE_UART0_PRINT
#define USE_UART0_PRINT 1
#endif

#if CONFIG_KERNEL_NONE
#ifndef CONFIG_SYSTEM_SECURE
#define CONFIG_SYSTEM_SECURE 1
#endif
#endif

/**
 * unused, reserved for alternative driver
 *
#ifndef SAVE_HIGH_REGISTERS
#define SAVE_HIGH_REGISTERS 1
#endif

#ifndef SAVE_VR_REGISTERS
#define SAVE_VR_REGISTERS 1
#endif
*/

#endif
