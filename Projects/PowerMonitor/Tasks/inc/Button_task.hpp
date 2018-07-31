


/*
**
** File Name:  Button_task.hpp
**
** Author: Amit Chaudhary
**
** Date: 16 June 2018
**
*/

#ifndef Button_task_HPP
#define Button_task_HPP

#include "stdint.h"
#include "TaskBase.hpp"

class Button_task : public TaskBase
{
public:

	Button_task();
	
	Button_task(TaskBase::FREQ_t Frequency, TaskBase::ALLOWEDLOOPTIME_t AllowedLoopTime);
	
	~Button_task(){TaskDelete(this);}
	
	void Init();
	
	void Run();

};




#endif // Button_task_HPP

