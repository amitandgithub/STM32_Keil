/*
//
// File Name:  Led_task.cpp
//
// Author: Amit Chaudhary
//
// Date: 16 June 2018
//
*/
  

#include "Led_task.hpp"
#include "Led.hpp"
#include "SysTickTimer.hpp"

static Led Led_PC13(GPIOC,GPIO_Pin_13);


Led_task::Led_task()
{
	
}

Led_task::Led_task(uint32_t Frequency, uint32_t AllowedLoopTime): TaskBase(Frequency, AllowedLoopTime)
{
	
}
void Led_task::Init()
{	
	Led_PC13.HwInit();
}

void Led_task::Run()
{
	Led_PC13.Toggle();
	//Bsp::SysTickTimer::DelayTicks(1);
	//for(volatile uint32_t i = 0; i<200;i++);
}