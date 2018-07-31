#include "utils.h"

static __IO uint32_t _delay;

void setup_delay_timer(TIM_TypeDef *timer) {
	TIM_DeInit(timer);
	// Enable Timer clock
	if (timer == TIM2) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	} else if (timer == TIM3) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	} else if (timer == TIM4) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	} else {
		// TODO: not implemented
		while (1) {
		}
	}

	// Configure timer
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStructure.TIM_Prescaler = SystemCoreClock / 1000000 - 1;
	TIM_InitStructure.TIM_Period = 10000 - 1; // Update event every 10000 us (10 ms)
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(timer, &TIM_InitStructure);

	TIM_Cmd(timer, ENABLE);
}

void delay(__IO uint32_t nTime)
{
	_delay = nTime;

	while(_delay != 0);
}

void delay_decrement(void) {
	if (_delay--) {
	}
}

void delay_us(TIM_TypeDef *timer, unsigned int time) {
	timer->CNT = 0;
	time -= 3;
	while (timer->CNT <= time) {
	}
}

void delay_ms(TIM_TypeDef *timer, unsigned int time) {
	while (time--) {
		delay_us(timer, 1000);
	}
}

// Saturated add functions for 8 / 16 / 32 unsigned integers
inline uint8_t sadd8(uint8_t a, uint8_t b) {
	return (a > 0xFF - b) ? 0xFF : a + b;
}

inline uint16_t sadd16(uint16_t a, uint16_t b) {
	return (a > 0xFFFF - b) ? 0xFFFF : a + b;
}

inline uint32_t sadd32(uint32_t a, uint32_t b) {
	return (a > 0xFFFFFFFF - b) ? 0xFFFFFFFF : a + b;
}
