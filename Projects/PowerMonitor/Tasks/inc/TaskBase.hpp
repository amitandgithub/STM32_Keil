
#ifndef TASK_BASE_HPP
#define TASK_BASE_HPP

#include "stdint.h"

// Forward declaration
class TaskBase;


class TaskBase
{
	
public:
    typedef uint32_t FREQ_t;
	typedef uint32_t ALLOWEDLOOPTIME_t;
	typedef enum 
	{
		ACTIVE    = 0,
		TIMED_OUT = 1,
		SUSPENDED = 2	
	}TaskState_t;
	
	typedef struct 
	{
		TaskBase *            Handle;
		uint32_t              LastExecuted;
		FREQ_t                Frequency;
		uint32_t              LoopTime;
		ALLOWEDLOOPTIME_t     AllowedLoopTime;
		TaskState_t  State;
	}TCB_t;

	static const FREQ_t DEFAULT_FREQ = 10*1000; // in Micro seconds
	static const ALLOWEDLOOPTIME_t DEFAULT_ALLOWED_LOOP_TIME = 10*1000; // in Micro seconds
	static const uint8_t NO_OF_TASKS = 10;
	
	TaskBase();
	
	TaskBase(FREQ_t Frequency, ALLOWEDLOOPTIME_t AllowedLoopTime);
	
	virtual ~TaskBase(){};
	
	static inline TCB_t * GetTaskTCB(){return Tasks;}
	
	static void TaskDelete(TaskBase * aTask);
	
	static void TaskDelete(uint8_t TaskID);
	
	static uint8_t Create(TaskBase* ThisPtr, FREQ_t Frequency, ALLOWEDLOOPTIME_t AllowedLoopTime );
	
	static uint8_t Create(TaskBase* ThisPtr);
	
	virtual void Init() = 0;
	
	virtual void Run() = 0;
	
	
private:

	static TCB_t Tasks[NO_OF_TASKS];
	
	
};

#endif

// Program Size: Code=32984 RO-data=4892 RW-data=12 ZI-data=3028  
// Program Size: Code=32976 RO-data=4900 RW-data=12 ZI-data=3068  