/*
//
// File Name:  Scheduler.cpp
//
// Author: Amit Chaudhary
//
// Date: 16 June 2018
//
*/
  

#include "Scheduler.hpp"
#include "SysTickTimer.hpp"

TaskBase::TCB_t * Scheduler::m_All_Tasks_TCB = nullptr;
volatile uint32_t Scheduler::m_TotalLoopTime_us = 0;

Scheduler::Scheduler(TaskBase::TCB_t* pTCB)// : m_All_Tasks_TCB(pTCB)
{
	m_TotalLoopTime_us = 0;
	m_All_Tasks_TCB = pTCB;
}

void Scheduler::Init()
{
	for( uint8_t task = 0; task < TaskBase::NO_OF_TASKS; task++)
	{
		if(m_All_Tasks_TCB && m_All_Tasks_TCB[task].Handle )
			m_All_Tasks_TCB[task].Handle->Init();
	}
}
volatile int32_t TotalCurrentTicks = 0;
volatile uint32_t CurrentTicks = 0;
volatile uint32_t CurrentMicroTicks = 0;
void Scheduler::Run()
{


static uint8_t task;

	while (1)
	{
		
		
		for( task = 0; task < TaskBase::NO_OF_TASKS; task++)
		{
			if(m_All_Tasks_TCB && m_All_Tasks_TCB[task].Handle )
			{
				//if(m_All_Tasks_TCB[task].State == TaskBase::ACTIVE)
				//{					
					
					
					if( Bsp::SysTickTimer::GetTicksSince(m_All_Tasks_TCB[task].LastExecuted) >= m_All_Tasks_TCB[task].Frequency)
					{
						CurrentMicroTicks = Bsp::SysTickTimer::GetMicroTicks();
						CurrentTicks      = Bsp::SysTickTimer::GetTicks();
						
						
						m_All_Tasks_TCB[task].Handle->Run();
						//TotalCurrentTicks = Bsp::SysTickTimer::GetMicroTicks() - TotalCurrentTicks;
						m_All_Tasks_TCB[task].LoopTime =  Bsp::SysTickTimer::GetMicroTicksSince(CurrentMicroTicks);//TotalCurrentTicks;//Bsp::SysTickTimer::GetTicksSince(CurrentTicks);
						m_All_Tasks_TCB[task].LastExecuted = CurrentTicks + Bsp::SysTickTimer::GetTicksSince(CurrentTicks);					
					
						if(m_All_Tasks_TCB[task].LoopTime > m_All_Tasks_TCB[task].AllowedLoopTime)
						{
							m_All_Tasks_TCB[task].State = TaskBase::TIMED_OUT;
						}
					}
				//}
					
			}
				
		}
		//m_TotalLoopTime_us = (Bsp::SysTickTimer::GetMicroTicks() - TotalCurrentTicks) > 0? Bsp::SysTickTimer::GetMicroTicks() - TotalCurrentTicks :
																									//Bsp::SysTickTimer::GetMicroTicks() - TotalCurrentTicks + 0xFFFFFFFFUL;
	}

}


//9986380292



