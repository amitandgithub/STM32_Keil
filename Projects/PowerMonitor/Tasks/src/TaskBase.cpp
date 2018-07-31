


#include "TaskBase.hpp"

TaskBase::TCB_t TaskBase::Tasks[TaskBase::NO_OF_TASKS];


TaskBase::TaskBase()
{
	//Tasks[TotalTasks++] = this;	
	
	if ( 0xFF == Create(this) )
	{
		while(1); // Fatal Error
	}
}

TaskBase::TaskBase(FREQ_t Frequency, ALLOWEDLOOPTIME_t AllowedLoopTime)
{
	
	if ( 0xFF == Create(this, Frequency, AllowedLoopTime ) )
	{
		while(1); // Fatal Error
	}
}

/*
uint8_t TaskBase::GetTaskID(TaskBase * aTask)
{

	for(uint8_t task = 0; task < NO_OF_TASKS; task++ )
	{
		if(Tasks[task] == aTask )
			return task;
	}
	return 0xFF;

}
*/

void TaskBase::TaskDelete(TaskBase * aTask)
{
	for(uint8_t task = 0; task < NO_OF_TASKS; task++ )
	{
		if(Tasks[task].Handle == aTask )
		{
			Tasks[task].Handle = nullptr;
			return;
		}
	}
}

void TaskBase::TaskDelete(uint8_t TaskID)
{
	Tasks[TaskID].Handle = nullptr;	
}

uint8_t TaskBase::Create(TaskBase* ThisPtr)
{
	for(uint8_t task = 0; task < NO_OF_TASKS; task++ )
	{
		if(Tasks[task].Handle == nullptr )
		{
			Tasks[task].Handle = ThisPtr;
			Tasks[task].Frequency = DEFAULT_FREQ;
			Tasks[task].AllowedLoopTime = DEFAULT_ALLOWED_LOOP_TIME;
			Tasks[task].State = ACTIVE;
			return task;
		}
	}
	return 0xFF;
}

uint8_t TaskBase::Create(TaskBase* ThisPtr, FREQ_t Frequency = DEFAULT_FREQ, ALLOWEDLOOPTIME_t AllowedLoopTime = DEFAULT_ALLOWED_LOOP_TIME  )
{
	for(uint8_t task = 0; task < NO_OF_TASKS; task++ )
	{
		if(Tasks[task].Handle == nullptr )
		{
			Tasks[task].Handle = ThisPtr;
			Tasks[task].Frequency = Frequency;
			Tasks[task].AllowedLoopTime = AllowedLoopTime;
			Tasks[task].State = ACTIVE;
			return task;
		}
	}
	return 0xFF;
}


