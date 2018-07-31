/*
 * SysTickTimer.cpp
 *
 *  Created on: 19-Oct-2017
 *      Author: Amit Chaudhary
 */

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "SysTickTimer.hpp"

namespace Bsp
{
volatile uint32_t SysTickTimer::m_ticks = 0;


extern "C" void SysTick_Handler (void)
{
	SysTickTimer::IncrementTicks();
}


bool SysTickTimer::HwInit(u32 SysTickFreq)
{
	if(SysTickFreq != 0U )
	{
		RCC_ClocksTypeDef RCC_Clocks;
		RCC_GetClocksFreq (&RCC_Clocks);
		(void) SysTick_Config (RCC_Clocks.HCLK_Frequency / SysTickFreq);
		return true;
	}
	return false;

}

void SysTickTimer::DelayTicks(u32 Ticks)
{
  u32 start, end;
  start = GetTicks();
  end = start + Ticks;
  if (start < end)
  {
    while ((GetTicks() >= start) && (GetTicks() < end))
    {
      // do nothing
    }
  }
  else
  {
    while ((GetTicks() >= start) || (GetTicks() < end))
    {
      // do nothing
    };
  }
}






}
