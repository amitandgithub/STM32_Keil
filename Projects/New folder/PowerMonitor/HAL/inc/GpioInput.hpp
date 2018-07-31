/*
 * GpioInput.hpp
 *
 *  Created on: 08-Oct-2017
 *      Author: Amit Chaudhary
 */

#ifndef APP_INC_GPIOINPUT_HPP_
#define APP_INC_GPIOINPUT_HPP_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "Gpio.hpp"
#include "InterruptManager.hpp"
#include "ClockManager.hpp"

namespace Bsp
{

class GpioInput: public Gpio
{
public:

	GpioInput(PORT Port, u16 Pin);

	GpioInput(PORT Port, u16 Pin, Peripheral::ISR aISR,IntOnWhichEdge eIntOnWhichEdge, EXTIMode eEXTIMode );

	GpioInput(PORT Port, u16 Pin, Peripheral::ISR aISR,IntOnWhichEdge eIntOnWhichEdge );

	virtual ~GpioInput(){};

	virtual bool HwInit();

	inline bool ReadInput() const { return GPIO_ReadInputDataBit(m_Port,m_Pin); }




private:
    bool ConfigureInterrupt();

	Bsp::PeripheralBase::IRQn MapPin2ExtLine();

	u8 MapPin2PinSource();

	bool ExtLineInterruptConfig();

private:
	u16             m_Pin;
	PORT            m_Port;
	EXTIMode        m_eEXTIMode;
    IntOnWhichEdge  m_eEdge;
	Peripheral::ISR m_pISR;
};

}// namespace Bsp




#endif /* APP_INC_GPIOINPUT_HPP_ */
