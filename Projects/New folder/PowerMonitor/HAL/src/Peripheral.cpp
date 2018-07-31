

#include "Peripheral.hpp"

namespace Bsp
{


bool Peripheral::HwClockEnable(Peripheral_t Peripheral)
{

    return (GetClockManager())->PeripheralClockEnable(Peripheral);

}

bool Peripheral::HwClockDisble(Peripheral_t Peripheral)
{

    return (GetClockManager())->PeripheralClockDisble(Peripheral);

}

bool Peripheral::RegisterInterrupt( ISR aISR, IRQn eIRQn, InterruptPriority_t aInterruptPriority)
{
    return (GetInterruptManager())-> RegisterInterrupt(aISR, static_cast<InterruptManager::IRQn>(eIRQn) );
}

bool Peripheral::HwEnableInterrupt(IRQn eIRQn)
{
    return (GetInterruptManager())-> EnableInterrupt(static_cast<InterruptManager::IRQn>(eIRQn));
}

bool Peripheral::HwDisableInterrupt(IRQn eIRQn)
{
    return (GetInterruptManager())-> DisableInterrupt(static_cast<InterruptManager::IRQn>(eIRQn));
}

bool Peripheral::HwConfigInterrupt(IRQn eIRQn, InterruptPriority_t aInterruptPriority, InterruptPriority_t aInterruptSubPriority)
{
    return (GetInterruptManager())-> NVICConfig(static_cast<InterruptManager::IRQn>(eIRQn),aInterruptPriority, aInterruptSubPriority);
}








}