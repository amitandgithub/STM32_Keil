/*
 * Gpio.cpp
 *
 *  Created on: 12-Aug-2017
 *      Author: Amit Chaudhary
 */

#if 0
#include "stm32f10x.h"
#include "Gpio.hpp"



Gpio::Gpio(PORT Port, u16 Pin, MODE Mode)
{
	m_Port = Port;
	m_Pin = Pin;
	m_Mode = Mode;
	m_Speed = GPIO_Speed_50MHz;
}

void Gpio::Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = Gpio::m_Pin;
	GPIO_InitStructure.GPIO_Speed = Gpio::m_Speed;
	GPIO_InitStructure.GPIO_Mode = Gpio::m_Mode;
	GPIO_Init( m_Port, &GPIO_InitStructure );
}

void Gpio::Mode(MODE mode)
{
	m_Mode = mode;
	Gpio::Init();
}
/*
u8 Gpio::Read(void)
{
	return GPIO_ReadInputDataBit(m_Port, m_Pin);
}

void Gpio::Write(BitAction Val)
{
	GPIO_WriteBit(m_Port, m_Pin, Val);
}
*/
void Gpio::AttachIntr(void* pfISR)
{
	m_ISR = pfISR;
}

void Gpio::DeInit(void)
{

}
#endif











