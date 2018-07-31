/*
 * Gpio.hpp
 *
 *  Created on: 12-Aug-2017
 *      Author: Amit Chaudhary
 */

#ifndef APP_INC_GPIO_HPP_
#define APP_INC_GPIO_HPP_


#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "Peripheral.hpp"

namespace Bsp
{

class Gpio : public Peripheral
{
public:
	typedef GPIO_TypeDef*     PORT;
	typedef GPIOMode_TypeDef  MODE;
	typedef GPIOSpeed_TypeDef SPEED;
    typedef enum
	{
	    EXTI_Trigger_Rising         = 0x08,
	    EXTI_Trigger_Falling        = 0x0C,
	    EXTI_Trigger_Rising_Falling = 0x10
	}IntOnWhichEdge;

	typedef enum
	{
        EXTI_Mode_Interrupt = 0x00,
        EXTI_Mode_Event     = 0x04
	}EXTIMode;

protected:
	Gpio(void):Peripheral(){};
	~Gpio(void){};
	//virtual bool HwInit() = 0;
private:

};

}// namespace Bsp



#endif /* APP_INC_GPIO_HPP_ */
