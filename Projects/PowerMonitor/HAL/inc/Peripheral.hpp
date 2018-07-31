/*
 * Peripheral.hpp
 *
 *  Created on: 14-Oct-2017
 *      Author: Amit Chaudhary
 */

#ifndef APP_INC_PERIPHERAL_HPP_
#define APP_INC_PERIPHERAL_HPP_

#include"PeripheralBase.h"
#include"ClockManager.hpp"
#include"PortManager.hpp"
#include"InterruptManager.hpp"

namespace Bsp
{


class Peripheral : protected PeripheralBase
{
public:
  typedef uint32_t FlagStatus_t;
  typedef uint32_t Flag_t;
  typedef uint8_t InterruptPriority_t;
  typedef void(*ISR)(void);

protected:
	Peripheral(){};
	virtual~Peripheral(){};
	virtual bool HwInit(){return true;}
	virtual bool HwConfig(void* pConfigStruct){return true;};
	virtual void HwEnable(){}
	virtual void HwDisable(){};
	virtual FlagStatus_t HwGetFlagStatus(Flag_t aFlag ){return (FlagStatus_t)true;}
  virtual bool HwClockEnable(Peripheral_t Peripheral);
	virtual bool HwClockDisble(Peripheral_t Peripheral);
  virtual bool RegisterInterrupt  (ISR pISR,IRQn eIRQn, InterruptPriority_t aInterruptPriority); 
	virtual bool HwConfigInterrupt  (IRQn eIRQn, InterruptPriority_t aInterruptPriority, InterruptPriority_t aInterruptSubPriority);
	virtual bool HwEnableInterrupt  (IRQn eIRQn);
	virtual bool HwDisableInterrupt (IRQn eIRQn);
	virtual bool HwClearInterrupt   (IRQn eIRQn){return true;};
  inline ClockManager*     GetClockManager()    { return ClockManager::GetInstance();}
  inline InterruptManager* GetInterruptManager(){ return InterruptManager::GetInstance();}

};

}//namespace Bsp


#endif /* APP_INC_PERIPHERAL_HPP_ */
