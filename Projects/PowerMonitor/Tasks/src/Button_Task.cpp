/*
//
// File Name:  Led_task.cpp
//
// Author: Amit Chaudhary
//
// Date: 25 June 2018
//
*/
  

#include "Button_task.hpp"
#include "HwButtonIntr.hpp"


void ClickEvent(void);
void LongPressEvent(void);
void LongLongPressEvent(void);

static Bsp::HwButtonIntr Touch_Button(GPIOA, GPIO_Pin_11,Bsp::GpioInput::EXTI_Trigger_Rising, Bsp::GpioInput::EXTI_Mode_Interrupt);

Button_task::Button_task()
{
	
}

Button_task::Button_task(TaskBase::FREQ_t Frequency, TaskBase::ALLOWEDLOOPTIME_t AllowedLoopTime): TaskBase(Frequency, AllowedLoopTime)
{
	
}
void Button_task::Init()
{	
	Touch_Button.HwInit();
	Touch_Button.RegisterEventHandler(Bsp::HwButtonIntr::Click,static_cast<Bsp::HwButtonIntr::BtnHandler>(ClickEvent));
	Touch_Button.RegisterEventHandler(Bsp::HwButtonIntr::LongPress,static_cast<Bsp::HwButtonIntr::BtnHandler>(LongPressEvent));
	Touch_Button.RegisterEventHandler(Bsp::HwButtonIntr::LongLongPress,static_cast<Bsp::HwButtonIntr::BtnHandler>(LongLongPressEvent));
}

void Button_task::Run()
{
	Touch_Button.RunStateMachine();
}



void ClickEvent(void)
{

}
void LongPressEvent(void)
{
    //Led_PA8.MultiBlink(2);
    //NokiaLCD.DrawLine(1, 0, "Long Press");
}
void LongLongPressEvent(void)
{
    //Led_PA8.MultiBlink(3);
   // NokiaLCD.DrawLine(1, 0, "Long Long Pres");
}