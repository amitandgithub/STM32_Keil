
 
#include "Driver_GPIO.h"

#define ARM_GPIO_DRV_VERSION    ARM_DRIVER_VERSION_MAJOR_MINOR(2, 0) /* driver version */

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = {
    ARM_GPIO_API_VERSION,
    ARM_GPIO_DRV_VERSION
};

/* Driver Capabilities */
static const ARM_GPIO_CAPABILITIES DriverCapabilities = {
    0  /* supports 10-bit addressing */
};

//
//  Functions
//

ARM_DRIVER_VERSION ARM_GPIO_GetVersion(void)
{
	return DriverVersion;
}

ARM_GPIO_CAPABILITIES ARM_GPIO_GetCapabilities(void)
{
	return DriverCapabilities;
}

int32_t ARM_GPIO_Initialize(ARM_GPIO_SignalEvent_t cb_event)
{
	int32_t ret;
	return ret;
}

int32_t ARM_GPIO_Uninitialize(void)
{
	int32_t ret;
	return ret;
}

int32_t ARM_GPIO_PowerControl(ARM_POWER_STATE state)
{
    switch (state)
    {
    case ARM_POWER_OFF:
        break;

    case ARM_POWER_LOW:
        break;

    case ARM_POWER_FULL:
        break;

    default:
        return ARM_DRIVER_ERROR_UNSUPPORTED;
    }
		return ARM_DRIVER_ERROR_UNSUPPORTED;
}

int32_t ARM_GPIO_On(GPIO_PORT Port, uint32_t Pin)
{
	int32_t ret = 0;
	GPIO_WriteBit(Port,Pin, 1);
	return ret;
}

int32_t ARM_GPIO_Off(GPIO_PORT Port, uint32_t Pin)
{
	int32_t ret = 0;
	GPIO_WriteBit(Port,Pin, 0);
	return ret;
}

int32_t ARM_GPIO_Toggle(GPIO_PORT Port, uint32_t Pin)
{
	int32_t ret = 0;
	return ret;
}


int32_t ARM_GPIO_Control(uint32_t control, uint32_t arg)
{
    switch (control)
    {
    case ARM_GPIO_OWN_ADDRESS:
        break;

    case ARM_GPIO_BUS_SPEED:
        switch (arg)
        {
        case ARM_GPIO_BUS_SPEED_STANDARD:
            break;
        case ARM_GPIO_BUS_SPEED_FAST:
            break;
        case ARM_GPIO_BUS_SPEED_FAST_PLUS:
            break;
        default:
            return ARM_DRIVER_ERROR_UNSUPPORTED;
        }
        break;

    case ARM_GPIO_BUS_CLEAR:
        break;

    case ARM_GPIO_ABORT_TRANSFER:
        break;

    default:
        return ARM_DRIVER_ERROR_UNSUPPORTED;
    }
		return ARM_DRIVER_ERROR_UNSUPPORTED;
}

ARM_GPIO_STATUS ARM_GPIO_GetStatus(void)
{
	ARM_GPIO_STATUS ret;
	return ret;
}

void ARM_GPIO_SignalEvent(uint32_t event)
{
    // function body
}

// End GPIO Interface

ARM_DRIVER_GPIO Driver_GPIO = {
    ARM_GPIO_GetVersion,
    ARM_GPIO_GetCapabilities,
    ARM_GPIO_Initialize,
    ARM_GPIO_Uninitialize,
    ARM_GPIO_PowerControl,
    ARM_GPIO_On,
    ARM_GPIO_Off,
    ARM_GPIO_Toggle,
    ARM_GPIO_Control,
    ARM_GPIO_GetStatus
};
