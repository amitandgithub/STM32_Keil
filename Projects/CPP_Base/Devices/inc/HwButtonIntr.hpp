/*
 * HwButtonIntr.hpp
 *
 *  Created on: 21-Oct-2017
 *      Author: Amit Chaudhary
 */

#ifndef APP_INC_HWBUTTONINTR_HPP_
#define APP_INC_HWBUTTONINTR_HPP_


#include "GpioInput.hpp"
#include "SysTickTimer.hpp"
#include <Queue.hpp>

namespace Bsp
{

class HwButtonIntr : public GpioInput
{
public:
	static const u32 QSIZE = 20;

	typedef void(*BtnHandler)(void);
    typedef void(*HwButtonISR)(void);
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

	HwButtonIntr(PORT Port, u16 Pin,IntOnWhichEdge eIntOnWhichEdge, EXTIMode eEXTIMode);
	~HwButtonIntr(){};

	bool HwInit();
	void RunStateMachine();
	bool RegisterEventHandler(BtnEvent_t eEvent,BtnHandler aBtnHandler);

private:
	void SetEvent(BtnEvent_t aBtnEvent){ m_Events |= 1UL << aBtnEvent;}
	void ClearEvent(BtnEvent_t aBtnEvent){ m_Events &= ~(1UL << aBtnEvent);}
	void EventHandler();
	inline static void ISR();
	const bool DEBOUNCE_REQUIRED = false;
	u32        m_Events;
	u32        m_HighEdgeDetectedTime;
	bool       m_PreviousInputPinState;
	static BtnStates  m_CurrentState;
	bool       m_EdgeDetected;
	Bsp::SysTickTimer* m_SysTickTimer;
	BtnHandler PressedHandler;
	BtnHandler ReleasedHandler;
	BtnHandler ClickHandler;
	BtnHandler DoubleClickHandler;
	BtnHandler LongPressHandler;
	BtnHandler LongLongPressHandler;
	HwButtonISR m_HwButtonISR;
	static u16 m_Pin;

	Queue::QType aQMem[QSIZE];




};

}// namespace Bsp



#endif /* APP_INC_HWBUTTONINTR_HPP_ */
