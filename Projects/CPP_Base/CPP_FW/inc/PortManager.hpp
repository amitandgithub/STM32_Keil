/*
 * PortManager.hpp
 *
 *  Created on: 15-Oct-2017
 *      Author: Amit Chaudhary
 */

#ifndef APP_INC_PORTMANAGER_HPP_
#define APP_INC_PORTMANAGER_HPP_
#include "stm32f10x.h"

namespace Bsp
{

class PortManager
{
public:
    static const u32 GPIO_Remaped_SPI1             = 0x00000001UL;  /*!< SPI1 Alternate Function mapping */
	static const u32 GPIO_Remaped_I2C1             = 0x00000002UL;  /*!< I2C1 Alternate Function mapping */
	static const u32 GPIO_Remaped_USART1           = 0x00000004UL;  /*!< USART1 Alternate Function mapping */
	static const u32 GPIO_Remaped_USART2           = 0x00000008UL;  /*!< USART2 Alternate Function mapping */
	static const u32 GPIO_PartialRemaped_USART3    = 0x00140010UL;  /*!< USART3 Partial Alternate Function mapping */
	static const u32 GPIO_FullRemaped_USART3       = 0x00140030UL;  /*!< USART3 Full Alternate Function mapping */
	static const u32 GPIO_PartialRemaped_TIM1      = 0x00160040UL;  /*!< TIM1 Partial Alternate Function mapping */
	static const u32 GPIO_FullRemaped_TIM1         = 0x001600C0UL;  /*!< TIM1 Full Alternate Function mapping */
	static const u32 GPIO_PartialRemaped1_TIM2     = 0x00180100UL;  /*!< TIM2 Partial1 Alternate Function mapping */
	static const u32 GPIO_PartialRemaped2_TIM2     = 0x00180200UL;  /*!< TIM2 Partial2 Alternate Function mapping */
	static const u32 GPIO_FullRemaped_TIM2         = 0x00180300UL;  /*!< TIM2 Full Alternate Function mapping */
	static const u32 GPIO_PartialRemaped_TIM3      = 0x001A0800UL;  /*!< TIM3 Partial Alternate Function mapping */
	static const u32 GPIO_FullRemaped_TIM3         = 0x001A0C00UL;  /*!< TIM3 Full Alternate Function mapping */
	static const u32 GPIO_Remaped_TIM4             = 0x00001000UL;  /*!< TIM4 Alternate Function mapping */
	static const u32 GPIO_Remaped1_CAN1            = 0x001D4000UL;  /*!< CAN1 Alternate Function mapping */
	static const u32 GPIO_Remaped2_CAN1            = 0x001D6000UL;  /*!< CAN1 Alternate Function mapping */
	static const u32 GPIO_Remaped_PD01             = 0x00008000UL;  /*!< PD01 Alternate Function mapping */
	static const u32 GPIO_Remaped_TIM5CH4_LSI      = 0x00200001UL;  /*!< LSI connected to TIM5 Channel4 input capture for calibration */
	static const u32 GPIO_Remaped_ADC1_ETRGINJ     = 0x00200002UL;  /*!< ADC1 External Trigger Injected Conversion remapping */
	static const u32 GPIO_Remaped_ADC1_ETRGREG     = 0x00200004UL;  /*!< ADC1 External Trigger Regular Conversion remapping */
	static const u32 GPIO_Remaped_ADC2_ETRGINJ     = 0x00200008UL;  /*!< ADC2 External Trigger Injected Conversion remapping */
	static const u32 GPIO_Remaped_ADC2_ETRGREG     = 0x00200010UL;  /*!< ADC2 External Trigger Regular Conversion remapping */
	static const u32 GPIO_Remaped_ETH              = 0x00200020UL;  /*!< Ethernet remapping (only for Connectivity line devicesUL; */
	static const u32 GPIO_Remaped_CAN2             = 0x00200040UL;  /*!< CAN2 remapping (only for Connectivity line devicesUL; */
	static const u32 GPIO_Remaped_SWJ_NoJTRST      = 0x00300100UL;  /*!< Full SWJ Enabled (JTAG-DP + SW-DPUL; but without JTRST */
	static const u32 GPIO_Remaped_SWJ_JTAGDisable  = 0x00300200UL;  /*!< JTAG-DP Disabled and SW-DP Enabled */
	static const u32 GPIO_Remaped_SWJ_Disable      = 0x00300400UL;  /*!< Full SWJ Disabled (JTAG-DP + SW-DPUL; */
	static const u32 GPIO_Remaped_SPI3             = 0x00201100UL;  /*!< SPI3/I2S3 Alternate Function mapping (only for Connectivity line devicesUL; */
	static const u32 GPIO_Remaped_TIM2ITR1_PTP_SOF = 0x00202000UL;  /*!< Ethernet PTP output or USB OTG SOF
	                                                                (Start of FrameUL; connected to TIM2 Internal Trigger 1 for calibration
	                                                                (only for Connectivity line devicesUL; */
	static const u32 GPIO_Remaped_PTP_PPS          = 0x00204000UL;  /*!< Ethernet MAC PPS_PTS output on PB05 (only for Connectivity line devicesUL; */
	static const u32 GPIO_Remaped_TIM15            = 0x80000001UL;  /*!< TIM15 Alternate Function mapping (only for Value line devicesUL; */
	static const u32 GPIO_Remaped_TIM16            = 0x80000002UL;  /*!< TIM16 Alternate Function mapping (only for Value line devicesUL; */
	static const u32 GPIO_Remaped_TIM17            = 0x80000004UL;  /*!< TIM17 Alternate Function mapping (only for Value line devicesUL; */
	static const u32 GPIO_Remaped_CEC              = 0x80000008UL;  /*!< CEC Alternate Function mapping (only for Value line devicesUL; */
	static const u32 GPIO_Remaped_TIM1_DMA         = 0x80000010UL;  /*!< TIM1 DMA requests mapping (only for Value line devicesUL; */
	static const u32 GPIO_Remaped_TIM9             = 0x80000020UL;  /*!< TIM9 Alternate Function mapping (only for XL-density devicesUL; */
	static const u32 GPIO_Remaped_TIM10            = 0x80000040UL;  /*!< TIM10 Alternate Function mapping (only for XL-density devicesUL; */
	static const u32 GPIO_Remaped_TIM11            = 0x80000080UL;  /*!< TIM11 Alternate Function mapping (only for XL-density devicesUL; */
	static const u32 GPIO_Remaped_TIM13            = 0x80000100UL;  /*!< TIM13 Alternate Function mapping (only for High density Value line and XL-density devicesUL; */
	static const u32 GPIO_Remaped_TIM14            = 0x80000200UL;  /*!< TIM14 Alternate Function mapping (only for High density Value line and XL-density devicesUL; */
	static const u32 GPIO_Remaped_FSMC_NADV        = 0x80000400UL;  /*!< FSMC_NADV Alternate Function mapping (only for High density Value line and XL-density devicesUL; */
	static const u32 GPIO_Remaped_TIM67_DAC_DMA    = 0x80000800UL;  /*!< TIM6/TIM7 and DAC DMA requests remapping (only for High density Value line devicesUL; */
	static const u32 GPIO_Remaped_TIM12            = 0x80001000UL;  /*!< TIM12 Alternate Function mapping (only for High density Value line devicesUL; */
	static const u32 GPIO_Remaped_MISC             = 0x80002000UL;  /*!< Miscellaneous Remap (DMA2 Channel5 Position and DAC Trigger remapping,
	                                                                 only for High density Value line devicesUL; */
	bool PortConfig(u32 PortRemap);
	bool PortDeConfig(u32 PortRemap);
	static PortManager* GetInstance();
private:
	PortManager(){}
	~PortManager(){};

	static PortManager* m_pPortManager;
};

}


#endif /* APP_INC_PORTMANAGER_HPP_ */
