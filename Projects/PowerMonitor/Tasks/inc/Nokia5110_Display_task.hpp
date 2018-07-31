
/*
**
** File Name:  Nokia5110_Display_task.hpp
**
** Author: Amit Chaudhary
**
** Date: 25 June 2018
**
*/

#ifndef Nokia5110_Display_task_HPP
#define Nokia5110_Display_task_HPP

#include "stdint.h"
#include "TaskBase.hpp"

class Nokia5110_Display_task : public TaskBase
{
public:

	Nokia5110_Display_task();
	
	Nokia5110_Display_task(TaskBase::FREQ_t Frequency, TaskBase::ALLOWEDLOOPTIME_t AllowedLoopTime);
	
	~Nokia5110_Display_task(){TaskDelete(this);}
	
	void Init();
	
	void Run();

};




#endif // Nokia5110_Display_task_HPP

