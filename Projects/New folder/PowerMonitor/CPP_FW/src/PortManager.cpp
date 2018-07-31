/*
 * PortManager.cpp
 *
 *  Created on: 15-Oct-2017
 *      Author: Amit Chaudhary
 */

#include"PortManager.hpp"
#include "stm32f10x_gpio.h"
namespace Bsp
{
PortManager* PortManager::m_pPortManager = nullptr;


bool PortManager::PortConfig(u32 PortRemap)
{
	GPIO_PinRemapConfig(PortRemap, ENABLE);
	return true;
}
bool PortManager::PortDeConfig(u32 PortRemap)
{
	GPIO_PinRemapConfig(PortRemap, DISABLE);
	return true;
}

PortManager* PortManager::GetInstance()
{
	if(m_pPortManager==nullptr)
	{
		static PortManager PortManager_Instance;
		m_pPortManager = &PortManager_Instance;
	}
	return m_pPortManager;
}

}//namespace Bsp
