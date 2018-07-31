/*
 * SysTickTimer.hpp
 *
 *  Created on: 19-Oct-2017
 *      Author: Amit Chaudhary
 */

#ifndef APP_INC_SYSTICKTIMER_HPP_
#define APP_INC_SYSTICKTIMER_HPP_

#include "stm32f10x.h"

namespace Bsp
{

class SysTickTimer
{
public:
	const unsigned int SYS_TICK_FREQ = 1000UL;
	static bool HwInit(unsigned int SysTickFreq);
	static inline unsigned int GetTicks(){ return m_ticks;}
	static unsigned int GetMicroTicks();
	//static inline unsigned int GetMicroTicks(){ return (m_ticks*1000UL +1000 - (SysTick->VAL/72) );}
	//static inline unsigned int GetMicroTicks(){ return (m_ticks*1000UL +  (SysTick->LOAD - SysTick->VAL)/72 );}
    //*1000 + 1000 � SysTick->VAL/72;
    //static inline unsigned int GetMicroTicks(){ return (m_ticks*1000 + 1000UL � SysTick->VAL/72);}
	static void DelayTicks (unsigned int Ticks);

	static inline unsigned int GetTicksSince(unsigned int LastTicks)
	{
		unsigned int CurrentTicks = GetTicks();

		return ( (CurrentTicks >= LastTicks)? (CurrentTicks - LastTicks ) :   (CurrentTicks - LastTicks + 0xFFFFFFFFUL) );
	}
	static inline unsigned int GetMicroTicksSince(unsigned int LastTicks)
	{
		static volatile unsigned int Rollover;
		volatile unsigned int CurrentTicks = GetMicroTicks();
		if(CurrentTicks >= LastTicks)
		return CurrentTicks - LastTicks;
		else
		{
			Rollover++;
			return CurrentTicks - LastTicks + 0xFFFFFFFFUL;
		}
	
		//return ( (CurrentTicks >= LastTicks)? (CurrentTicks - LastTicks ) :   (CurrentTicks - LastTicks + 0xFFFFFFFFUL) );
	}
	static SysTickTimer* GetInstance();
	static inline void IncrementTicks(){m_ticks++;}
	~SysTickTimer(){};

private:
	//SysTickTimer(){m_pSysTickTimer = nullptr;}
	//static SysTickTimer* m_pSysTickTimer;
	static volatile unsigned int m_ticks;

};













}



#endif /* APP_INC_SYSTICKTIMER_HPP_ */
