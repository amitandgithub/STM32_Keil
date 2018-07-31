/*
**
** File Name:  Led_task.h
**
** Author: Amit Chaudhary
**
** Date: 16 June 2018
**
*/


#ifndef Led_task_HPP
#define Led_task_HPP

#include "stdint.h"
#include "TaskBase.hpp"


class Led_task : public TaskBase
{
public:

	Led_task();
	
	Led_task(TaskBase::FREQ_t Frequency, TaskBase::ALLOWEDLOOPTIME_t AllowedLoopTime);
	
	~Led_task(){TaskDelete(this);}
	
	void Init();
	
	void Run();
	
private:

	uint8_t m_MyTaskID;

};

#endif // Led_task_HPP
