/*
 * HwButtonIntr.cpp
 *
 *  Created on: 21-Oct-2017
 *      Author: Amit Chaudhary
 */

#include "stm32f10x.h"
#include "HwButtonIntr.hpp"
#include "SysTickTimer.hpp"

namespace Bsp
{
u16 HwButtonIntr::m_Pin = 0;
HwButtonIntr::BtnStates  HwButtonIntr::m_CurrentState = HwButtonIntr::IdleState;

HwButtonIntr::HwButtonIntr(PORT Port, u16 Pin,IntOnWhichEdge eIntOnWhichEdge, EXTIMode eEXTIMode) :
		           GpioInput(Port, Pin, ISR, eIntOnWhichEdge, eEXTIMode )
				   //Queue(aQMem,  QSIZE)
{
	m_Pin = Pin;
	m_Events                = 0UL;
	m_HighEdgeDetectedTime  = 0UL;
	m_PreviousInputPinState = false;
	m_EdgeDetected          = false;
	m_SysTickTimer          = nullptr;

	PressedHandler          = nullptr;
	ReleasedHandler         = nullptr;
	DoubleClickHandler      = nullptr;
	ClickHandler            = nullptr;
	LongPressHandler        = nullptr;
	LongLongPressHandler    = nullptr;
}

bool HwButtonIntr::HwInit()
{
	GpioInput::HwInit();
	return true;
}

void HwButtonIntr::RunStateMachine()
{
	static u32 ElapsedTime = 0UL;


	switch(m_CurrentState)
	{
		case HwButtonIntr::IdleState:
		{
			 break;
		}
		case HwButtonIntr::PressedState:
		{
			m_HighEdgeDetectedTime = SysTickTimer::GetTicks();
			m_CurrentState = HwButtonIntr::IdleState;
			break;
		}
		case HwButtonIntr::ReleasedState:
		{
			 ElapsedTime = SysTickTimer::GetTicksSince(m_HighEdgeDetectedTime);

			 if( (ElapsedTime >= ClickTimeLow) && (ElapsedTime <= ClickTimeHigh) )
			 {
				 m_CurrentState = HwButtonIntr::ClickIdentifyState;
			 }
			 else if ( (ElapsedTime >= LongPressTimeLow) && (ElapsedTime <= LongPressTimeHigh) )
			 {
				 m_CurrentState = HwButtonIntr::LongPressState;
			 }
			 else if ( (ElapsedTime >= LongLongPressTimeLow) && (ElapsedTime <= LongLongPressTimeHigh) )
			 {
				 m_CurrentState = HwButtonIntr::LongLongPressState;
			 }
			 else if (ElapsedTime >= UnSupportedTime)
			 {
				 m_CurrentState = HwButtonIntr::IdleState;
			 }
			 else
			 {
				 //m_CurrentState = HwButtonIntr::IdleState;
			 }
			 break;
		}
		case HwButtonIntr::ClickIdentifyState:
		{
			static u32 LastClickStateEntered = 0;//m_SysTickTimer->GetTicks();
			static u32 temp  = 0;//m_SysTickTimer->GetTicksSince(LastClickStateEntered);


			temp  = SysTickTimer::GetTicksSince(LastClickStateEntered);

			if ( (temp) > 300 )
			{
				LastClickStateEntered =  SysTickTimer::GetTicks();
				m_CurrentState = HwButtonIntr::ClickState;
			}
			else
			{
				m_CurrentState = HwButtonIntr::DoubleClickState;
			}

			break;
		}
		case HwButtonIntr::ClickState:
		{
			SetEvent(HwButtonIntr::Click);
			m_CurrentState = HwButtonIntr::IdleState;
			break;
		}
		case HwButtonIntr::DoubleClickState:
		{
			SetEvent(HwButtonIntr::DoubleClick);
			m_CurrentState = HwButtonIntr::IdleState;
			break;
		}
		case HwButtonIntr::LongPressState:
		{
			SetEvent(HwButtonIntr::LongPress);
			m_CurrentState = HwButtonIntr::IdleState;
			break;
		}
		case HwButtonIntr::LongLongPressState:
		{
			SetEvent(HwButtonIntr::LongLongPress);
			m_CurrentState = HwButtonIntr::IdleState;
			break;
		}
		default: m_CurrentState = HwButtonIntr::IdleState;
	}

	EventHandler();
}

void HwButtonIntr::EventHandler()
{
	// If there is some event present, Handle it
	if(m_Events)
	{
		if( (m_Events & (1<<Pressed)) && (PressedHandler))
		{
			PressedHandler();
			ClearEvent(Pressed);
		}
		else if( (m_Events & (1<<Released)) && (ReleasedHandler))
		{
			ReleasedHandler();
			ClearEvent(Released);
		}
		else if( (m_Events & (1<<Click)) && (ClickHandler))
		{
			ClickHandler();
			ClearEvent(Click);
		}
		else if( (m_Events & (1<<DoubleClick)) && (DoubleClickHandler))
		{
			DoubleClickHandler();
			ClearEvent(DoubleClick);
		}
		else if( (m_Events & (1<<LongPress)) && (LongPressHandler))
		{
			LongPressHandler();
			ClearEvent(LongPress);
		}
		else if( (m_Events & (1<<(LongLongPress)) && (LongLongPressHandler)) )
		{
			LongLongPressHandler();
			ClearEvent(LongLongPress);
		}

		else
		{
			// Nothing to do
		}
	}

}
bool HwButtonIntr::RegisterEventHandler(BtnEvent_t eEvent,BtnHandler aBtnHandler)
{
	switch(eEvent)
	{
	case HwButtonIntr::Click         : ClickHandler = aBtnHandler;break;
	case HwButtonIntr::DoubleClick   : DoubleClickHandler = aBtnHandler;break;
	case HwButtonIntr::LongPress     : LongPressHandler = aBtnHandler;break;
	case HwButtonIntr::LongLongPress : LongLongPressHandler = aBtnHandler;break;
	default:break;
	}
	return true;
}

void HwButtonIntr::ISR()
{

	// Masks the further interrupt requests until this is serviced
	 EXTI->IMR &= ~m_Pin;

	 if( (EXTI->RTSR) & m_Pin ) // Interrupt triggered because of Rising edge
	 {
		 m_CurrentState = HwButtonIntr::PressedState;

		 // Configure Interrupt for the next falling edge
		 EXTI->FTSR |= m_Pin;
		 EXTI->RTSR &= ~m_Pin; // Clear Rising edge interrupt configuration

	 }
	 else					 // Interrupt triggered because of Falling edge
	 {
		 m_CurrentState = HwButtonIntr::ReleasedState;
		 // Configure Interrupt for the next Rising edge
		 EXTI->RTSR |= m_Pin;
		 EXTI->FTSR &= ~m_Pin; // Clear Rising edge interrupt configuration
	 }

	// Enable the further interrupt requests
	 EXTI->IMR |= m_Pin;
}

}



