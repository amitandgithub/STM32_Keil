/*
 * HwButton.hpp
 *
 *  Created on: 17-Oct-2017
 *      Author: Amit Chaudhary
 */

#ifndef APP_INC_HWBUTTON_HPP_
#define APP_INC_HWBUTTON_HPP_

#include "GpioInput.hpp"
#include "SysTickTimer.hpp"

namespace Bsp
{
/* New Button Class Improvements : 10 Jan 2018 
  1. Touch generates 50 milli sec events
  2. Touch events stored in event Queue 
     for the application to read.
*/ 
class HwButton : public GpioInput
{
public:
	typedef void(*BtnHandler)(void);
	typedef enum
	{
		Idle = 0,
		Pressed,
		Released,
		Click,
		DoubleClick,
		LongPress,
		LongLongPress
	}BtnEvent_t;

	 enum
	{
		ClickTimeLow          = 70,
		ClickTimeHigh         = 400,
		DoubleClickTimeLow    = 400,
		DoubleClickTimeHigh   = 1000,
		LongPressTimeLow      = 700,
		LongPressTimeHigh     = 1500,// about 1-2 sec
		LongLongPressTimeLow  = 2000, // more than 5 sec
		LongLongPressTimeHigh = 6000,
		UnSupportedTime = 6100,
	};

	typedef enum
	{
		IdleState = 0,
		PressedState,
		ReleasedState,
		LongPressState,     // about 1-2 sec
		LongLongPressState, // more than 5 sec
		ClickState,
		ClickIdentifyState,
		DoubleClickState
	}BtnStates;

	HwButton(PORT Port, u16 Pin);
	HwButton(PORT Port, u16 Pin, InterruptManager::ISR aISR,IntOnWhichEdge eIntOnWhichEdge, EXTIMode eEXTIMode);
	~HwButton(){};

	bool HwInit();
	void RunStateMachine();
	bool ReadButton();
	bool RegisterEventHandler(BtnEvent_t eEvent,BtnHandler aBtnHandler);

private:
	void SetEvent(BtnEvent_t aBtnEvent){ m_Events |= 1UL << aBtnEvent;}
	void ClearEvent(BtnEvent_t aBtnEvent){ m_Events &= ~(1UL << aBtnEvent);}
	void EventHandler();
	bool SampleButton();
	bool SampleButtonUsingTimer();
	const bool DEBOUNCE_REQUIRED = false;


	u32        m_Events;
	u32        m_HighEdgeDetectedTime;
	bool       m_PreviousInputPinState;
	BtnStates  m_CurrentState;
	bool       m_EdgeDetected;
	Bsp::SysTickTimer* m_SysTickTimer;


	BtnHandler PressedHandler;
	BtnHandler ReleasedHandler;
	BtnHandler ClickHandler;
	BtnHandler DoubleClickHandler;
	BtnHandler LongPressHandler;
	BtnHandler LongLongPressHandler;


};

}// namespace Bsp
#endif /* APP_INC_HWBUTTON_HPP_ */
