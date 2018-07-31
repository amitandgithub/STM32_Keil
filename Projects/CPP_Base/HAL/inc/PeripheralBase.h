#ifndef APP_INC_PERIPHERAL_BASE_HPP_
#define APP_INC_PERIPHERAL_BASE_HPP_

//******************
// CLASS: PeripheralBase
//
// DESCRIPTION:
//  This is the base class for the peripheral without interrupt functionality
//
// CREATED: 10-01-2018, by Amit Chaudhary
//
// FILE: $FILE_FNAME$
//

//#include"ClockManager.hpp"


#include "PortManager.hpp"
#include "InterruptManager.hpp"
#include "stm32f10x_rcc.h"

namespace Bsp
{

    class PeripheralBase
    {
    public:
    	typedef enum
        {
            APB2Periph_AFIO,
            DUMMY1,
            APB2Periph_GPIOA,
            APB2Periph_GPIOB,
            APB2Periph_GPIOC,
            APB2Periph_GPIOD,
            APB2Periph_GPIOE,
            APB2Periph_GPIOF,
            APB2Periph_GPIOG,
            APB2Periph_ADC1,
            APB2Periph_ADC2,
            APB2Periph_TIM1,
            APB2Periph_SPI1,
            APB2Periph_TIM8 ,
            APB2Periph_USART1,
            APB2Periph_ADC3,
            APB2Periph_TIM15,
            APB2Periph_TIM16,
            APB2Periph_TIM17,
            APB2Periph_TIM9,
            APB2Periph_TIM10,
            APB2Periph_TIM11,
            APB1Periph_TIM2,
            APB1Periph_TIM3,
            APB1Periph_TIM4,
            APB1Periph_TIM5,
            APB1Periph_TIM6,
            APB1Periph_TIM7,
            APB1Periph_TIM12,
            APB1Periph_TIM13,
            APB1Periph_TIM14,
            DUMMY2,
            DUMMY3,
            APB1Periph_WWDG,
            DUMMY4,
            DUMMY5,
            APB1Periph_SPI2,
            APB1Periph_SPI3,
            DUMMY6,
            APB1Periph_USART2,
            APB1Periph_USART3,
            APB1Periph_UART4,
            APB1Periph_UART5,
            APB1Periph_I2C1,
            APB1Periph_I2C2,
            APB1Periph_USB,
            DUMMY7,
            APB1Periph_CAN1,
            APB1Periph_CAN2,
            APB1Periph_BKP,
            APB1Periph_PWR,
            APB1Periph_DAC,
            APB1Periph_CEC,
            AHBPeriph_DMA1,
            AHBPeriph_DMA2,
            AHBPeriph_SRAM,
            DUMMY8,
            AHBPeriph_FLITF,
            DUMMY9,
            AHBPeriph_CRC
        }Peripheral_t;

        typedef enum
        {
            NMI_Handler = -9,
            HardFault_Handler,
            MemManage_Handler,
            BusFault_Handler,
            UsageFault_Handler,
            SVC_Handler,
            DebugMon_Handler,
            PendSV_Handler,
            SysTick_Handler,
            WWDG_IRQHandler,
            PVD_IRQHandler,
            TAMPER_IRQHandler,
            RTC_IRQHandler,
            FLASH_IRQHandler,
            RCC_IRQHandler,
            EXTI0_IRQHandler,
            EXTI1_IRQHandler,
            EXTI2_IRQHandler,
            EXTI3_IRQHandler,
            EXTI4_IRQHandler,
            DMA1_Channel1_IRQHandler,
            DMA1_Channel2_IRQHandler,
            DMA1_Channel3_IRQHandler,
            DMA1_Channel4_IRQHandler,
            DMA1_Channel5_IRQHandler,
            DMA1_Channel6_IRQHandler,
            DMA1_Channel7_IRQHandler,
            ADC1_2_IRQHandler,
            USB_HP_CAN1_TX_IRQHandler,
            USB_LP_CAN1_RX0_IRQHandler,
            CAN1_RX1_IRQHandler,
            CAN1_SCE_IRQHandler,
            EXTI9_5_IRQHandler,
            TIM1_BRK_IRQHandler,
            TIM1_UP_IRQHandler,
            TIM1_TRG_COM_IRQHandler,
            TIM1_CC_IRQHandler,
            TIM2_IRQHandler,
            TIM3_IRQHandler,
            TIM4_IRQHandler,
            I2C1_EV_IRQHandler,
            I2C1_ER_IRQHandler,
            I2C2_EV_IRQHandler,
            I2C2_ER_IRQHandler,
            SPI1_IRQHandler,
            SPI2_IRQHandler,
            USART1_IRQHandler,
            USART2_IRQHandler,
            USART3_IRQHandler,
            EXTI15_10_IRQHandler,
            RTCAlarm_IRQHandler,
            USBWakeUp_IRQHandler,
            EXTI5_IRQHandler,
            EXTI6_IRQHandler,
            EXTI7_IRQHandler,
            EXTI8_IRQHandler,
            EXTI9_IRQHandler,
            EXTI10_IRQHandler,
            EXTI11_IRQHandler,
            EXTI12_IRQHandler,
            EXTI13_IRQHandler,
            EXTI14_IRQHandler,
            EXTI15_IRQHandler
        }IRQn;

    protected:
        PeripheralBase(){};
        virtual ~PeripheralBase(){};

        virtual bool HwInit(){return true;};
        virtual bool HwConfig(void* pConfigStruct){return true;};
        virtual void HwEnable(){};
         virtual void HwDisable(){};
        virtual uint32_t HwGetFlagStatus(uint32_t aFlag ){return 1;};
        inline PortManager*      GetPortManager ()    { return PortManager::GetInstance();}
};

} //namespace Bsp

#endif