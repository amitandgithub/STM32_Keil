/*
 * InterruptManager.cpp
 *
 *  Created on: 12-Oct-2017
 *      Author: Amit Chaudhary
 */

#include "InterruptManager.hpp"
#include "stm32f10x.h"
#include "PeripheralBase.h"
#include "stm32f10x_exti.h"
#include "misc.h"

namespace Bsp
{
InterruptManager* InterruptManager::m_pInterruptManager = nullptr;

InterruptManager::ISR InterruptManager::m_NMI_Handler = nullptr;
InterruptManager::ISR InterruptManager::m_HardFault_Handler = nullptr;
InterruptManager::ISR InterruptManager::m_MemManage_Handler = nullptr;
InterruptManager::ISR InterruptManager::m_BusFault_Handler = nullptr;
InterruptManager::ISR InterruptManager::m_UsageFault_Handler = nullptr;
InterruptManager::ISR InterruptManager::m_SVC_Handler = nullptr;
InterruptManager::ISR InterruptManager::m_DebugMon_Handler = nullptr;
InterruptManager::ISR InterruptManager::m_PendSV_Handler = nullptr;
InterruptManager::ISR InterruptManager::m_SysTick_Handler = nullptr;
InterruptManager::ISR InterruptManager::m_WWDG_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_PVD_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_TAMPER_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_RTC_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_FLASH_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_RCC_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_EXTI0_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_EXTI1_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_EXTI2_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_EXTI3_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_EXTI4_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_DMA1_Channel1_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_DMA1_Channel2_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_DMA1_Channel3_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_DMA1_Channel4_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_DMA1_Channel5_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_DMA1_Channel6_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_DMA1_Channel7_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_ADC1_2_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_USB_HP_CAN1_TX_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_USB_LP_CAN1_RX0_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_CAN1_RX1_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_CAN1_SCE_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_EXTI9_5_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_TIM1_BRK_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_TIM1_UP_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_TIM1_TRG_COM_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_TIM1_CC_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_TIM2_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_TIM3_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_TIM4_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_I2C1_EV_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_I2C1_ER_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_I2C2_EV_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_I2C2_ER_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_SPI1_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_SPI2_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_USART1_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_USART2_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_USART3_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_EXTI15_10_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_RTCAlarm_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_USBWakeUp_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_EXTI5_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_EXTI6_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_EXTI7_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_EXTI8_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_EXTI9_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_EXTI10_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_EXTI11_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_EXTI12_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_EXTI13_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_EXTI14_IRQHandler = nullptr;
InterruptManager::ISR InterruptManager::m_EXTI15_IRQHandler = nullptr;

bool InterruptManager::RegisterInterrupt(ISR pISR, IRQn eIRQn )
{
	switch(static_cast<short int>(eIRQn))
	{
    case InterruptManager::NMI_Handler               :    InterruptManager::m_NMI_Handler                 = pISR; break;
	case InterruptManager::HardFault_Handler         :    InterruptManager::m_HardFault_Handler           = pISR; break;
	case InterruptManager::MemManage_Handler         :    InterruptManager::m_MemManage_Handler           = pISR; break;
	case InterruptManager::BusFault_Handler          :    InterruptManager::m_BusFault_Handler            = pISR; break;
	case InterruptManager::UsageFault_Handler        :    InterruptManager::m_UsageFault_Handler          = pISR; break;
	case InterruptManager::SVC_Handler               :    InterruptManager::m_SVC_Handler                 = pISR; break;
	case InterruptManager::DebugMon_Handler          :    InterruptManager::m_DebugMon_Handler            = pISR; break;
	case InterruptManager::PendSV_Handler            :    InterruptManager::m_PendSV_Handler              = pISR; break;
	case InterruptManager::SysTick_Handler           :    InterruptManager::m_SysTick_Handler             = pISR; break;
	case InterruptManager::WWDG_IRQHandler           :    InterruptManager::m_WWDG_IRQHandler             = pISR; break;
	case InterruptManager::PVD_IRQHandler            :    InterruptManager::m_PVD_IRQHandler              = pISR; break;
	case InterruptManager::TAMPER_IRQHandler         :    InterruptManager::m_TAMPER_IRQHandler           = pISR; break;
	case InterruptManager::RTC_IRQHandler            :    InterruptManager::m_RTC_IRQHandler              = pISR; break;
	case InterruptManager::FLASH_IRQHandler          :    InterruptManager::m_FLASH_IRQHandler            = pISR; break;
	case InterruptManager::RCC_IRQHandler            :    InterruptManager::m_RCC_IRQHandler              = pISR; break;
	case InterruptManager::EXTI0_IRQHandler          :    InterruptManager::m_EXTI0_IRQHandler            = pISR; break;
	case InterruptManager::EXTI1_IRQHandler          :    InterruptManager::m_EXTI1_IRQHandler            = pISR; break;
	case InterruptManager::EXTI2_IRQHandler          :    InterruptManager::m_EXTI2_IRQHandler            = pISR; break;
	case InterruptManager::EXTI3_IRQHandler          :    InterruptManager::m_EXTI3_IRQHandler            = pISR; break;
	case InterruptManager::EXTI4_IRQHandler          :    InterruptManager::m_EXTI4_IRQHandler            = pISR; break;
	case InterruptManager::DMA1_Channel1_IRQHandler  :    InterruptManager::m_DMA1_Channel1_IRQHandler    = pISR; break;
	case InterruptManager::DMA1_Channel2_IRQHandler  :    InterruptManager::m_DMA1_Channel2_IRQHandler    = pISR; break;
	case InterruptManager::DMA1_Channel3_IRQHandler  :    InterruptManager::m_DMA1_Channel4_IRQHandler    = pISR; break;
	case InterruptManager::DMA1_Channel4_IRQHandler  :    InterruptManager::m_DMA1_Channel5_IRQHandler    = pISR; break;
	case InterruptManager::DMA1_Channel5_IRQHandler  :    InterruptManager::m_DMA1_Channel6_IRQHandler    = pISR; break;
	case InterruptManager::DMA1_Channel6_IRQHandler  :    InterruptManager::m_DMA1_Channel7_IRQHandler    = pISR; break;
	case InterruptManager::DMA1_Channel7_IRQHandler  :    InterruptManager::m_ADC1_2_IRQHandler           = pISR; break;
	case InterruptManager::ADC1_2_IRQHandler         :    InterruptManager::m_USB_HP_CAN1_TX_IRQHandler   = pISR; break;
	case InterruptManager::USB_HP_CAN1_TX_IRQHandler :    InterruptManager::m_USB_LP_CAN1_RX0_IRQHandler  = pISR; break;
	case InterruptManager::USB_LP_CAN1_RX0_IRQHandler:    InterruptManager::m_CAN1_RX1_IRQHandler         = pISR; break;
	case InterruptManager::CAN1_RX1_IRQHandler       :    InterruptManager::m_CAN1_SCE_IRQHandler         = pISR; break;
	case InterruptManager::CAN1_SCE_IRQHandler       :    InterruptManager::m_TIM1_BRK_IRQHandler         = pISR; break;
	case InterruptManager::EXTI9_5_IRQHandler        :    InterruptManager::m_EXTI9_5_IRQHandler          = pISR; break;
	case InterruptManager::TIM1_BRK_IRQHandler       :    InterruptManager::m_TIM1_UP_IRQHandler          = pISR; break;
	case InterruptManager::TIM1_UP_IRQHandler        :    InterruptManager::m_TIM1_TRG_COM_IRQHandler     = pISR; break;
	case InterruptManager::TIM1_TRG_COM_IRQHandler   :    InterruptManager::m_TIM1_CC_IRQHandler          = pISR; break;
	case InterruptManager::TIM1_CC_IRQHandler        :    InterruptManager::m_TIM2_IRQHandler             = pISR; break;
	case InterruptManager::TIM2_IRQHandler           :    InterruptManager::m_TIM3_IRQHandler             = pISR; break;
	case InterruptManager::TIM3_IRQHandler           :    InterruptManager::m_TIM4_IRQHandler             = pISR; break;
	case InterruptManager::TIM4_IRQHandler           :    InterruptManager::m_I2C1_ER_IRQHandler          = pISR; break;
	case InterruptManager::I2C1_EV_IRQHandler        :    InterruptManager::m_I2C2_EV_IRQHandler          = pISR; break;
	case InterruptManager::I2C1_ER_IRQHandler        :    InterruptManager::m_I2C1_ER_IRQHandler          = pISR; break;
	case InterruptManager::I2C2_EV_IRQHandler        :    InterruptManager::m_I2C2_EV_IRQHandler          = pISR; break;
	case InterruptManager::I2C2_ER_IRQHandler        :    InterruptManager::m_I2C2_ER_IRQHandler          = pISR; break;
	case InterruptManager::SPI1_IRQHandler           :    InterruptManager::m_SPI1_IRQHandler             = pISR; break;
	case InterruptManager::SPI2_IRQHandler           :    InterruptManager::m_SPI2_IRQHandler             = pISR; break;
	case InterruptManager::USART1_IRQHandler         :    InterruptManager::m_USART1_IRQHandler           = pISR; break;
	case InterruptManager::USART2_IRQHandler         :    InterruptManager::m_USART2_IRQHandler           = pISR; break;
	case InterruptManager::USART3_IRQHandler         :    InterruptManager::m_USART3_IRQHandler           = pISR; break;
	case InterruptManager::EXTI15_10_IRQHandler      :    InterruptManager::m_EXTI15_10_IRQHandler        = pISR; break;
	case InterruptManager::RTCAlarm_IRQHandler       :    InterruptManager::m_RTCAlarm_IRQHandler         = pISR; break;
	case InterruptManager::USBWakeUp_IRQHandler      :    InterruptManager::m_USBWakeUp_IRQHandler        = pISR; break;
	case InterruptManager::EXTI5_IRQHandler          :    InterruptManager::m_EXTI5_IRQHandler            = pISR; break;
	case InterruptManager::EXTI6_IRQHandler          :    InterruptManager::m_EXTI6_IRQHandler            = pISR; break;
	case InterruptManager::EXTI7_IRQHandler          :    InterruptManager::m_EXTI7_IRQHandler            = pISR; break;
	case InterruptManager::EXTI8_IRQHandler          :    InterruptManager::m_EXTI8_IRQHandler            = pISR; break;
	case InterruptManager::EXTI9_IRQHandler          :    InterruptManager::m_EXTI9_IRQHandler            = pISR; break;
	case InterruptManager::EXTI10_IRQHandler         :    InterruptManager::m_EXTI10_IRQHandler           = pISR; break;
	case InterruptManager::EXTI11_IRQHandler         :    InterruptManager::m_EXTI11_IRQHandler           = pISR; break;
	case InterruptManager::EXTI12_IRQHandler         :    InterruptManager::m_EXTI12_IRQHandler           = pISR; break;
	case InterruptManager::EXTI13_IRQHandler         :    InterruptManager::m_EXTI13_IRQHandler           = pISR; break;
	case InterruptManager::EXTI14_IRQHandler         :    InterruptManager::m_EXTI14_IRQHandler           = pISR; break;
	case InterruptManager::EXTI15_IRQHandler         :    InterruptManager::m_EXTI15_IRQHandler           = pISR; break;

	default : break;
	}

	if(pISR) NVICConfig(eIRQn,15,0);

	return true;

}

InterruptManager* InterruptManager::GetInstance()
{
	if(m_pInterruptManager==nullptr)
	{
		static InterruptManager InterruptManager_Instance;
		m_pInterruptManager = &InterruptManager_Instance;
	}
	return m_pInterruptManager;

}

bool InterruptManager::DisableInterrupt(IRQn eIRQn)
{
    bool Status = false;

	if( (eIRQn >= EXTI5_IRQHandler ) && (eIRQn <= EXTI9_IRQHandler )  )
    {
        eIRQn = InterruptManager::EXTI9_5_IRQHandler;

    }
	else if( (eIRQn >= EXTI10_IRQHandler ) && (eIRQn <= EXTI15_IRQHandler )  )
    {
        eIRQn = InterruptManager::EXTI15_10_IRQHandler;

    }
	else
	{
		// nothing to do
	}

    /* Disable the Selected IRQ Channels -------------------------------------*/
    NVIC->ICER[static_cast<u8>(eIRQn) >> 0x05] = (uint32_t)0x01 << (static_cast<u8>(eIRQn) & (u8)0x1F);
    Status = true;

    return  Status;
}

bool InterruptManager::EnableInterrupt(IRQn eIRQn)
{
    bool Status = false;

	if( (eIRQn >= EXTI5_IRQHandler ) && (eIRQn <= EXTI9_IRQHandler )  )
    {
        eIRQn = InterruptManager::EXTI9_5_IRQHandler;

    }
	else if( (eIRQn >= EXTI10_IRQHandler ) && (eIRQn <= EXTI15_IRQHandler )  )
    {
        eIRQn = InterruptManager::EXTI15_10_IRQHandler;

    }
	else
	{
		// nothing to do
	}



    /* Enable the Selected IRQ Channels --------------------------------------*/
    NVIC->ISER[( static_cast<u8>(eIRQn) >> 0x05)] = (u32)0x01 << (static_cast<u8>(eIRQn) & (u8)0x1F);
    Status = true;

    return  Status;

}
bool InterruptManager::NVICConfig(IRQn eIRQn, u8 Priority, u8 SubPriority)
{
    bool Status = false;
	NVIC_InitTypeDef NVIC_InitStructure;
	InterruptState aFunctionalState = DISABLE;

	if( (eIRQn >= EXTI5_IRQHandler ) && (eIRQn <= EXTI9_IRQHandler )  )
    {
        eIRQn = InterruptManager::EXTI9_5_IRQHandler;
    }

	else if( (eIRQn >= EXTI10_IRQHandler ) && (eIRQn <= EXTI15_IRQHandler )  )
    {
        eIRQn = InterruptManager::EXTI15_10_IRQHandler;
    }
	else
	{
		// nothing to do
	}

    /* Configure NVIC for External Interrupts */
    NVIC_InitStructure.NVIC_IRQChannel = eIRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = static_cast<FunctionalState>(aFunctionalState);
    NVIC_Init( &NVIC_InitStructure );
    Status = true;

    return Status;
}


} // namespace Bsp

extern "C"
{

void EXTI9_5_IRQHandler(void)
{

	if((bool)EXTI_GetITStatus(EXTI_Line5) == true)
	{
		EXTI_ClearITPendingBit(EXTI_Line5);
		if(Bsp::InterruptManager::m_EXTI5_IRQHandler)
			Bsp::InterruptManager::m_EXTI5_IRQHandler();
	}
	else if ((bool)EXTI_GetITStatus(EXTI_Line6) == true)
	{
		EXTI_ClearITPendingBit(EXTI_Line6);
		if(Bsp::InterruptManager::m_EXTI6_IRQHandler)
			Bsp::InterruptManager::m_EXTI6_IRQHandler();
	}
	else if ((bool)EXTI_GetITStatus(EXTI_Line7) == true)
	{
		EXTI_ClearITPendingBit(EXTI_Line7);
		if(Bsp::InterruptManager::m_EXTI7_IRQHandler)
			Bsp::InterruptManager::m_EXTI7_IRQHandler();
	}
	else if ((bool)EXTI_GetITStatus(EXTI_Line8) == true)
	{
		EXTI_ClearITPendingBit(EXTI_Line8);
		if(Bsp::InterruptManager::m_EXTI8_IRQHandler)
			Bsp::InterruptManager::m_EXTI8_IRQHandler();
	}
	else if ((bool)EXTI_GetITStatus(EXTI_Line9) == true)
	{
		EXTI_ClearITPendingBit(EXTI_Line9);
		if(Bsp::InterruptManager::m_EXTI9_IRQHandler)
			Bsp::InterruptManager::m_EXTI9_IRQHandler();
	}
	else
	{

	}
}

void EXTI15_10_IRQHandler(void)
{

	if((bool)EXTI_GetITStatus(EXTI_Line10) == true)
	{
		EXTI_ClearITPendingBit(EXTI_Line10);
		if(Bsp::InterruptManager::m_EXTI10_IRQHandler)
			Bsp::InterruptManager::m_EXTI10_IRQHandler();
	}
	else if ((bool)EXTI_GetITStatus(EXTI_Line11) == true)
	{
		EXTI_ClearITPendingBit(EXTI_Line11);
		if(Bsp::InterruptManager::m_EXTI11_IRQHandler)
			Bsp::InterruptManager::m_EXTI11_IRQHandler();
	}
	else if ((bool)EXTI_GetITStatus(EXTI_Line12) == true)
	{
		EXTI_ClearITPendingBit(EXTI_Line12);
		if(Bsp::InterruptManager::m_EXTI12_IRQHandler)
			Bsp::InterruptManager::m_EXTI12_IRQHandler();
	}
	else if ((bool)EXTI_GetITStatus(EXTI_Line13) == true)
	{
		EXTI_ClearITPendingBit(EXTI_Line13);
		if(Bsp::InterruptManager::m_EXTI13_IRQHandler)
			Bsp::InterruptManager::m_EXTI13_IRQHandler();
	}
	else if ((bool)EXTI_GetITStatus(EXTI_Line14) == true)
	{
		EXTI_ClearITPendingBit(EXTI_Line14);
		if(Bsp::InterruptManager::m_EXTI14_IRQHandler)
			Bsp::InterruptManager::m_EXTI14_IRQHandler();
	}
	else if ((bool)EXTI_GetITStatus(EXTI_Line15) == true)
	{
		EXTI_ClearITPendingBit(EXTI_Line15);
		if(Bsp::InterruptManager::m_EXTI15_IRQHandler)
			Bsp::InterruptManager::m_EXTI15_IRQHandler();
	}
	else
	{

	}
}

void EXTI0_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line0);
	if(Bsp::InterruptManager::m_EXTI0_IRQHandler)
		Bsp::InterruptManager::m_EXTI0_IRQHandler();
}
void EXTI1_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line1);
	if(Bsp::InterruptManager::m_EXTI1_IRQHandler)
		Bsp::InterruptManager::m_EXTI1_IRQHandler();
}
void EXTI2_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line2);
	if(Bsp::InterruptManager::m_EXTI2_IRQHandler)
		Bsp::InterruptManager::m_EXTI2_IRQHandler();
}
void EXTI3_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line3);
	if(Bsp::InterruptManager::m_EXTI3_IRQHandler)
		Bsp::InterruptManager::m_EXTI3_IRQHandler();
}
void EXTI4_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line4);
	if(Bsp::InterruptManager::m_EXTI4_IRQHandler)
		Bsp::InterruptManager::m_EXTI4_IRQHandler();
}
void EXTI5_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line5);
	if(Bsp::InterruptManager::m_EXTI5_IRQHandler)
		Bsp::InterruptManager::m_EXTI5_IRQHandler();
}



}//  extern "C"


