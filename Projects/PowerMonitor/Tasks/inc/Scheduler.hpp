

/*
//
// File Name:  Led_task.hpp
//
// Author: Amit Chaudhary
//
// Date: 16 June 2018
//
*/

#ifndef FILE_HPP
#define FILE_HPP

#include "stdint.h"
#include "TaskBase.hpp"

class Scheduler
{

public:

	Scheduler(TaskBase::TCB_t* pTCB);
	
	~Scheduler(){};
	
	void Init();
	
	void Run();
	
private:
	static TaskBase::TCB_t * m_All_Tasks_TCB;
	static volatile uint32_t m_TotalLoopTime_us;

};

#endif // FILE_HPP

