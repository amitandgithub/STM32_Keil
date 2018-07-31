
/* Includes */
#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include "stm32f10x.h"
#include "SysTickTimer.hpp"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "Test.h"
#include "GpioOutput.hpp"

#include "Scheduler.hpp"
#include "TaskBase.hpp"
#include "Led_task.hpp"
#include "Button_task.hpp"
#include "Nokia5110_Display_task.hpp"
#include "PowerMonitor_task.hpp"
#include "SD_Card_Task.hpp"

#define MS(a) (a*1000)




using namespace Bsp;
static void prvSetupHardware( void );
/**********************/
// Cretate all the tasks here



Scheduler MyScheduler(TaskBase::GetTaskTCB());
Led_task LED_task(250,MS(1));
Button_task TouchButton_Task(1,MS(10));
Nokia5110_Display_task NokiaDisplay_Task(10,MS(10));
PowerMonitor_task PowerMonitor(1,MS(50));
SD_Card_Task SD_Card_Task(5000,MS(500));

int main(void)
{		
    // Initialize the PLL, Clocks, Memories and other Microcontroller HW
    prvSetupHardware();

	MyScheduler.Init();
		
	MyScheduler.Run();

    while(1)
    {    

    }
}

static void prvSetupHardware( void )
{
	/* Start with the clocks in their expected state. */
	RCC_DeInit();

	/* Enable HSE (high speed external clock). */
	RCC_HSEConfig( RCC_HSE_ON );

	/* Wait till HSE is ready. */
	while( RCC_GetFlagStatus( RCC_FLAG_HSERDY ) == RESET )
	{
	}

	/* 2 wait states required on the flash. */
	*( ( unsigned long * ) 0x40022000 ) = 0x02;

	/* HCLK = SYSCLK */
	RCC_HCLKConfig( RCC_SYSCLK_Div1 );

	/* PCLK2 = HCLK */
	RCC_PCLK2Config( RCC_HCLK_Div1 );

	/* PCLK1 = HCLK/2 */
	RCC_PCLK1Config( RCC_HCLK_Div2 );

	/* PLLCLK = 8MHz * 9 = 72 MHz. */
	RCC_PLLConfig( RCC_PLLSource_HSE_Div1, RCC_PLLMul_9 );

	/* Enable PLL. */
	RCC_PLLCmd( ENABLE );

	/* Wait till PLL is ready. */
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
	{
	}

	/* Select PLL as system clock source. */
	RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK );

	/* Wait till PLL is used as system clock source. */
	while( RCC_GetSYSCLKSource() != 0x08 )
	{
	}

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_AFIO, ENABLE );

	/* Set the Vector Table base address at 0x08000000 */
	NVIC_SetVectorTable( NVIC_VectTab_FLASH, 0x0 );

	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

	/* Configure HCLK clock as SysTick clock source. */
	SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK );
	
	// Start SysTick Timer for 1ms (1000 us) tick resolution
    SysTickTimer::HwInit(1000UL);

}