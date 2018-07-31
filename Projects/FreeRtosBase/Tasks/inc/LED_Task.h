

#include "FreeRTOS.h"
#include "task.h"

#define mainCHECK_TASK_PRIORITY				( tskIDLE_PRIORITY )
#define mainCHECK_TASK_STACK_SIZE			( configMINIMAL_STACK_SIZE )

extern void vLEDTask( void *pvParameters );

