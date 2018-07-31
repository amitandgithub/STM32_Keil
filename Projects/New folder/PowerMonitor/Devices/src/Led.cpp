

#include "stm32f10x.h"
#include "Led.hpp"
#include "SysTickTimer.hpp"


Led::Led(PORT Port, u16 Pin) : Bsp::GpioOutput(Port, Pin)
{

	HwInit();
}

void Led::On(void)
{
	SetOutput();
	Led_State = ON;
}

void Led::Off(void)
{
	ClearOutput();
	Led_State = OFF;
}

void Led::Blink(unsigned int delay)
{
	volatile unsigned int i;
	for(i=0;i<delay;i++);
	ToggleOutput();
}
void Led::Blink(unsigned int on,unsigned int off)
{
	volatile unsigned int i;
	for(i=0;i<off;i++);
	ToggleOutput();
	for(i=0;i<on;i++);
	ToggleOutput();
}

void Led::MultiBlink(unsigned int Blinks)
{
    for(volatile uint32_t i = 0; i< Blinks; i++ )
    {
        On();
        Bsp::SysTickTimer::DelayTicks(200);
        Off();
        Bsp::SysTickTimer::DelayTicks(200);
        On();

    }
}


