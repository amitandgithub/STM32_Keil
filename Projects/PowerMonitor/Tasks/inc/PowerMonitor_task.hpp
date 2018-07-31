/*
**
** File Name:  PowerMonitor_task.hpp
**
** Author: Amit Chaudhary
**
** Date: 26 June 2018
**
*/

#ifndef PowerMonitor_task_HPP
#define PowerMonitor_task_HPP

#include "stdint.h"
#include "TaskBase.hpp"

class PowerMonitor_task : public TaskBase
{
public:

	PowerMonitor_task();
	
	PowerMonitor_task(TaskBase::FREQ_t Frequency, TaskBase::ALLOWEDLOOPTIME_t AllowedLoopTime);
	
	~PowerMonitor_task(){TaskDelete(this);}
	
	void Init();
	
	void Run();

};

#endif // PowerMonitor_task_HPP

