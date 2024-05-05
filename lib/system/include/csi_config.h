#ifndef __CSI_CONFIG_H__
#define __CSI_CONFIG_H__

#ifndef CONFIG_CHIP_SL04
#define CONFIG_CHIP_SL04           1
#endif

#ifndef CONFIG_KERNEL_NONE
#define CONFIG_KERNEL_NONE         1
#endif

#ifndef CONFIG_HAVE_VIC
#define CONFIG_HAVE_VIC            1
#endif

#ifndef CONFIG_SEPARATE_IRQ_SP
#define CONFIG_SEPARATE_IRQ_SP     1
#endif

#ifndef CONFIG_ARCH_INTERRUPTSTACK
#define CONFIG_ARCH_INTERRUPTSTACK 4096
#endif

#ifndef CONFIG_IRQ_VECTOR_SIZE
#define CONFIG_IRQ_VECTOR_SIZE     256
#endif

#define USE_UART0_PRINT            1 // UART0 printf, 0:OFF, 1:ON

#if USE_UART0_PRINT
#define USE_UART0_AUTO_DL          0 // Auto download, 0:OFF, 1:ON
#endif

#ifdef CONFIG_KERNEL_NONE
#define CONFIG_SYSTEM_SECURE       1
#endif


#endif
