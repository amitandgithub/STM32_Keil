
/*
**
** File Name:  SD_Card_Task.h
**
** Author: Amit Chaudhary
**
** Date: 12 July 2018
**
*/


#ifndef SD_Card_task_HPP
#define SD_Card_task_HPP

#include "stdint.h"
#include "TaskBase.hpp"


class SD_Card_Task : public TaskBase
{
public:

	SD_Card_Task();
	
	SD_Card_Task(TaskBase::FREQ_t Frequency, TaskBase::ALLOWEDLOOPTIME_t AllowedLoopTime);
	
	~SD_Card_Task(){TaskDelete(this);}
	
	void Init();
	
	void Run();
	
private:


};

#endif // SD_Card_task_HPP
