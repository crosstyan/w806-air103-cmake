/**
 * @file    wm_cpu.c
 *
 * @brief   cpu driver module
 *
 * @author  kevin
 *
 * Copyright (c) 2014 Winner Microelectronics Co., Ltd.
 */
#include "priv/core_804.h"
#include "priv/wm_regs.h"
#include "pub/wm_cpu.h"


/**
 * @brief          	This function is used to get cpu clock
 *
 * @param[out]     *sysclk	point to the addr for system clk output
 *
 * @return         	None
 *
 * @note           	None
 */
void tls_sys_clk_get(tls_sys_clk *sysclk)
{
#ifndef TLS_CONFIG_FPGA
	clk_div_reg clk_div;

	clk_div.w = tls_reg_read32(HR_CLK_DIV_CTL);
	sysclk->cpuclk = W800_PLL_CLK_MHZ/(clk_div.b.CPU);
	sysclk->wlanclk = W800_PLL_CLK_MHZ/(clk_div.b.WLAN);
	sysclk->apbclk = sysclk->cpuclk / clk_div.b.BUS2;
#else
	sysclk->apbclk =
	sysclk->cpuclk =
	sysclk->wlanclk = 40;
#endif
}



