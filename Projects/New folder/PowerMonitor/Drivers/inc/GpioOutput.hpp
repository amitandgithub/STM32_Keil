/*
 * GpioOutput.hpp
 *
 *  Created on: 07-Oct-2017
 *      Author: Amit Chaudhary
 */

#ifndef APP_INC_GPIOOUTPUT_HPP_
#define APP_INC_GPIOOUTPUT_HPP_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "Gpio.hpp"

namespace Bsp
{

class GpioOutput: public Gpio
{
public:
	enum OutputValue
	{
		LOW    = 0U,
		HIGH   = 1U
	};

	GpioOutput(PORT Port, u16 Pin);
	GpioOutput(PORT Port, u16 Pin, MODE aMODE);
	virtual ~GpioOutput(void){};
	virtual bool HwInit();
	inline void SetOutput(void){GPIO_WriteBit(m_Port, m_Pin,(BitAction)1);}
	inline void ClearOutput(void){GPIO_WriteBit(m_Port, m_Pin, (BitAction)0);}
    inline void Write(bool Value){ Value ? GPIO_WriteBit(m_Port, m_Pin,(BitAction)1) : GPIO_WriteBit(m_Port, m_Pin,(BitAction)0);}
	void ToggleOutput(void);
	inline uint8_t ReadOutputValue(void){ return GPIO_ReadOutputDataBit(m_Port, m_Pin); }
private:
	u16   m_PinMask;
	PORT  m_Port;
	u16   m_Pin;
	MODE  m_Mode;
	OutputValue   m_eGpioOutputState;
	ClockManager* m_pClockManager;

};






}



#endif /* APP_INC_GPIOOUTPUT_HPP_ */
