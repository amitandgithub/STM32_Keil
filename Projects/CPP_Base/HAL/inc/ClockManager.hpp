/*
 * ClockManager.hpp
 *
 *  Created on: 14-Oct-2017
 *      Author: Amit Chaudhary
 */

#ifndef APP_INC_CLOCKMANAGER_HPP_
#define APP_INC_CLOCKMANAGER_HPP_

#include "stm32f10x.h"
#include "PeripheralBase.h"


namespace Bsp
{
    
class ClockManager
{
public:

        static bool PeripheralClockEnable(Bsp::PeripheralBase::Peripheral_t RCC_Periph);
	static bool PeripheralClockDisble(Bsp::PeripheralBase::Peripheral_t RCC_Periph);
	static ClockManager* GetInstance();
private:
	static ClockManager* m_pClockManager;
};

}//namespace bsp
#endif /* APP_INC_CLOCKMANAGER_HPP_ */
