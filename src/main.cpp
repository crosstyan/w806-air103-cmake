#include <cstdint>
#include <wm_hal.h>
#include <FreeRTOSConfig.h>
#include <FreeRTOS.h>
#include <task.h>
#include "core.h"
#include "wm_regs.h"
#include "wm_cpu.hpp"

constexpr auto O1 = GPIO_PIN_24;
constexpr auto O2 = GPIO_PIN_25;
constexpr auto O3 = GPIO_PIN_26;
const auto GRP    = GPIOB;

static const auto GPIO_init = [] {
	auto init = GPIO_InitTypeDef{O1 | O2 | O3, GPIO_MODE_OUTPUT, GPIO_NOPULL};
	HAL_GPIO_Init(GRP, &init);
};

static const auto set_all = [](GPIO_PinState st = GPIO_PIN_SET) {
	HAL_GPIO_WritePin(GRP, O1, st);
	HAL_GPIO_WritePin(GRP, O2, st);
	HAL_GPIO_WritePin(GRP, O3, st);
};

/**
 * @brief naive implementation of delay
 */
constexpr auto naive_delay = [](uint32_t cycles) {
	for (uint32_t i = 0; i < cycles; i++) {
		__NOP();
	}
};

/**
 * @brief delay in microsecond in 240Mhz
 * @warning based on empirical data
 */
constexpr auto naive_240M_delay_us = [](uint32_t us) {
	// 100'000 iteration taks 1565 us
	const auto iteration_per_us = 100'000 / 1565;
	for (auto i = 0; i < iteration_per_us * us; i++) {
		__NOP();
	}
};

static constexpr auto get_PSR = [] {
	auto psr_ = __get_PSR();
	return *reinterpret_cast<PSR_Type *>(&psr_);
};

static constexpr auto blink = [](void *pvParameters) -> void {
	for (;;) {
		set_all(GPIO_PIN_RESET);
		vTaskDelay(pdMS_TO_TICKS(500));
		set_all(GPIO_PIN_SET);
		vTaskDelay(pdMS_TO_TICKS(500));
		const auto psr = get_PSR();
		printf("t=%lldus m=%ldms c=%ld\n",
			   hal::cpu::tick_us(),
			   pdTICKS_TO_MS(xTaskGetTickCount()),
			   xTaskGetTickCount());
	}
};

extern "C" __attribute__((constructor)) void premain() {
	SystemClock_Config(CPU_CLK_240M);
	HAL_InitTick(0b10);
	/**
	 * @note portSET_INTERRUPT_MASK_FROM_ISR
	 */
	core::rtos_init(0b00);
	core::serial_init();
	GPIO_init();
}

extern "C" {
[[noreturn]] int main() {
	StaticTask_t xTaskBuffer;
	StackType_t xStack[512];
#if CONFIG_KERNEL_FREERTOS
	xTaskCreateStatic(blink, "blink", std::size(xStack), nullptr, configMAX_PRIORITIES - 4, xStack, &xTaskBuffer);
	vTaskStartScheduler();
#else
	for (;;) {
		printf("t=%lld\n", hal::cpu::tick_us());
	}
#endif
}
}
