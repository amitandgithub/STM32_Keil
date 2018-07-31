/*
 * GpioInput.cpp
 *
 *  Created on: 08-Oct-2017
 *      Author: Amit Chaudhary
 */


#include "GpioInput.hpp"
#include "stm32f10x_exti.h"

namespace Bsp
{

GpioInput::GpioInput(PORT Port, u16 Pin)
{
	m_Port = Port;
	m_Pin = Pin;
	m_pISR = nullptr;
	m_eEdge = EXTI_Trigger_Falling;
	m_eEXTIMode = EXTI_Mode_Interrupt;
}

GpioInput::GpioInput(PORT Port, u16 Pin, InterruptManager::ISR aISR,
		             IntOnWhichEdge eIntOnWhichEdge )
{
	m_Port = Port;
	m_Pin = Pin;
	m_pISR = aISR;
	m_eEdge = eIntOnWhichEdge;
	m_eEXTIMode = EXTI_Mode_Interrupt;
}

GpioInput::GpioInput(PORT Port, u16 Pin, InterruptManager::ISR aISR,
		     IntOnWhichEdge eIntOnWhichEdge, EXTIMode eEXTIMode )
{
	m_Port = Port;
	m_Pin = Pin;
	m_pISR = aISR;
	m_eEdge = eIntOnWhichEdge;
	m_eEXTIMode = eEXTIMode;
}

Bsp::PeripheralBase::IRQn GpioInput::MapPin2ExtLine()
{
	const Bsp::PeripheralBase::IRQn ExtLine =
        (GpioInput::m_Pin == GPIO_Pin_0) ? Bsp::PeripheralBase::EXTI0_IRQHandler :
        (GpioInput::m_Pin == GPIO_Pin_1) ? Bsp::PeripheralBase::EXTI1_IRQHandler :
        (GpioInput::m_Pin == GPIO_Pin_2) ? Bsp::PeripheralBase::EXTI2_IRQHandler :
        (GpioInput::m_Pin == GPIO_Pin_3) ? Bsp::PeripheralBase::EXTI3_IRQHandler :
        (GpioInput::m_Pin == GPIO_Pin_4) ? Bsp::PeripheralBase::EXTI4_IRQHandler :
        (GpioInput::m_Pin == GPIO_Pin_5) ? Bsp::PeripheralBase::EXTI5_IRQHandler :
        (GpioInput::m_Pin == GPIO_Pin_6) ? Bsp::PeripheralBase::EXTI6_IRQHandler :
        (GpioInput::m_Pin == GPIO_Pin_7) ? Bsp::PeripheralBase::EXTI7_IRQHandler :
        (GpioInput::m_Pin == GPIO_Pin_8) ? Bsp::PeripheralBase::EXTI8_IRQHandler :
        (GpioInput::m_Pin == GPIO_Pin_9) ? Bsp::PeripheralBase::EXTI9_IRQHandler :
        (GpioInput::m_Pin == GPIO_Pin_10)? Bsp::PeripheralBase::EXTI10_IRQHandler :
        (GpioInput::m_Pin == GPIO_Pin_11)? Bsp::PeripheralBase::EXTI11_IRQHandler :
        (GpioInput::m_Pin == GPIO_Pin_12)? Bsp::PeripheralBase::EXTI12_IRQHandler :
        (GpioInput::m_Pin == GPIO_Pin_13)? Bsp::PeripheralBase::EXTI13_IRQHandler :
        (GpioInput::m_Pin == GPIO_Pin_14)? Bsp::PeripheralBase::EXTI14_IRQHandler :
        (GpioInput::m_Pin == GPIO_Pin_15)? Bsp::PeripheralBase::EXTI15_IRQHandler : Bsp::PeripheralBase::EXTI0_IRQHandler;

	return ExtLine;
}

u8 GpioInput::MapPin2PinSource()
{
	const u8 PinSource =
         (GpioInput::m_Pin == GPIO_Pin_0) ? 0  :
         (GpioInput::m_Pin == GPIO_Pin_1) ? 1  :
         (GpioInput::m_Pin == GPIO_Pin_2) ? 2  :
         (GpioInput::m_Pin == GPIO_Pin_3) ? 3  :
         (GpioInput::m_Pin == GPIO_Pin_4) ? 4  :
         (GpioInput::m_Pin == GPIO_Pin_5) ? 5  :
         (GpioInput::m_Pin == GPIO_Pin_6) ? 6  :
         (GpioInput::m_Pin == GPIO_Pin_7) ? 7  :
         (GpioInput::m_Pin == GPIO_Pin_8) ? 8  :
         (GpioInput::m_Pin == GPIO_Pin_9) ? 9  :
         (GpioInput::m_Pin == GPIO_Pin_10)? 10 :
         (GpioInput::m_Pin == GPIO_Pin_11)? 11 :
         (GpioInput::m_Pin == GPIO_Pin_12)? 12 :
         (GpioInput::m_Pin == GPIO_Pin_13)? 13 :
         (GpioInput::m_Pin == GPIO_Pin_14)? 14 :
         (GpioInput::m_Pin == GPIO_Pin_15)? 15 : 0;

	return PinSource;
}

bool GpioInput::HwInit()
{
        GPIO_InitTypeDef GPIO_InitStructure;
	const Peripheral::Peripheral_t  PeripheralName =
       (m_Port==GPIOA)? Peripheral::APB2Periph_GPIOA :
       (m_Port==GPIOB)? Peripheral::APB2Periph_GPIOB :
       (m_Port==GPIOC)? Peripheral::APB2Periph_GPIOC :
       (m_Port==GPIOD)? Peripheral::APB2Periph_GPIOD :
       (m_Port==GPIOE)? Peripheral::APB2Periph_GPIOE :
       (m_Port==GPIOF)? Peripheral::APB2Periph_GPIOF : Peripheral::APB2Periph_GPIOG;

       // Enable the GPIO hardware Clocks
       HwClockEnable(PeripheralName);
       HwClockEnable(Bsp::Peripheral::APB2Periph_AFIO);

       GPIO_InitStructure.GPIO_Pin    = m_Pin;
       GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;
       GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_IPU;
       GPIO_Init( m_Port, &GPIO_InitStructure );

       if(m_pISR != nullptr)
       {
           // Configure the Interrupt if ISR is supplied with the constructor
           ConfigureInterrupt();
       }
       return true;
}

bool GpioInput::ConfigureInterrupt()
{
    bool Status = false;

    Bsp::PeripheralBase::IRQn L_IRQn;

    if(m_pISR != nullptr)
    {
        L_IRQn = MapPin2ExtLine();
        //Register Interrupt
        RegisterInterrupt(m_pISR, L_IRQn, 0);

        // Configure External Gpio Line for Interrupt
        ExtLineInterruptConfig();

        //Enable_Interrupt
        HwEnableInterrupt(L_IRQn);

        Status = true;
    }

    return Status;
}

bool GpioInput::ExtLineInterruptConfig()
{
	EXTI_InitTypeDef EXTI_InitStruct;

	const u8  GpioPort = (m_Port==GPIOA)? 0 :
       (m_Port==GPIOB)? 1 :
       (m_Port==GPIOC)? 2 :
       (m_Port==GPIOD)? 3 :
       (m_Port==GPIOE)? 4 :
       (m_Port==GPIOF)? 5 : 6;

       GPIO_EXTILineConfig( GpioPort,MapPin2PinSource() );

       EXTI_InitStruct.EXTI_Line = m_Pin;
       EXTI_InitStruct.EXTI_Mode = static_cast<EXTIMode_TypeDef>(m_eEXTIMode);
       EXTI_InitStruct.EXTI_Trigger = static_cast<EXTITrigger_TypeDef>(m_eEdge);
       EXTI_InitStruct.EXTI_LineCmd = ENABLE;
       EXTI_Init(&EXTI_InitStruct);

       return true;
}
/*
InterruptManager::IRQn GpioInput::MapPin2IRQn()
{
	const InterruptManager::IRQn ExtLine =
			(GpioInput::m_Pin == GPIO_Pin_0) ? InterruptManager::EXTI0_IRQHandler :
            (GpioInput::m_Pin == GPIO_Pin_1) ? InterruptManager::EXTI1_IRQHandler :
            (GpioInput::m_Pin == GPIO_Pin_2) ? InterruptManager::EXTI2_IRQHandler :
            (GpioInput::m_Pin == GPIO_Pin_3) ? InterruptManager::EXTI3_IRQHandler :
            (GpioInput::m_Pin == GPIO_Pin_4) ? InterruptManager::EXTI4_IRQHandler :
            (GpioInput::m_Pin == GPIO_Pin_5) ? InterruptManager::EXTI9_5_IRQHandler :
			(GpioInput::m_Pin == GPIO_Pin_6) ? InterruptManager::EXTI9_5_IRQHandler :
            (GpioInput::m_Pin == GPIO_Pin_7) ? InterruptManager::EXTI9_5_IRQHandler :
            (GpioInput::m_Pin == GPIO_Pin_8) ? InterruptManager::EXTI9_5_IRQHandler :
            (GpioInput::m_Pin == GPIO_Pin_9) ? InterruptManager::EXTI9_5_IRQHandler :
            (GpioInput::m_Pin == GPIO_Pin_10)? InterruptManager::EXTI15_10_IRQHandler :
            (GpioInput::m_Pin == GPIO_Pin_11)? InterruptManager::EXTI15_10_IRQHandler :
			(GpioInput::m_Pin == GPIO_Pin_12)? InterruptManager::EXTI15_10_IRQHandler :
            (GpioInput::m_Pin == GPIO_Pin_13)? InterruptManager::EXTI15_10_IRQHandler :
            (GpioInput::m_Pin == GPIO_Pin_14)? InterruptManager::EXTI15_10_IRQHandler :
            (GpioInput::m_Pin == GPIO_Pin_15)? InterruptManager::EXTI15_10_IRQHandler : InterruptManager::EXTI0_IRQHandler;

	return ExtLine;
}
*/

}



