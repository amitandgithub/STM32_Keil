/*
 * SpiDriver.cpp
 *
 *  Created on: 28-Oct-2017
 *      Author: Amit Chaudhary
 */


#include"SpiDriver.hpp"
#include "stm32f10x.h"
#include "stm32f10x_spi.h"

namespace Bsp
{

SpiDriver::SpiDriver(SpiInstance_t aSpi, SpiIsr_t aSpiIsr, SpiMode_t aSpiMode, SpiBaudRate_t aSpiBaudRate )
{
	m_SpiInstance = aSpi;
	m_SpiIsr      = aSpiIsr;
	m_SpiMode     = aSpiMode;
	m_SpiBaudRate = aSpiBaudRate;

	// Default values, to be on safer side
	m_SPIx            = SPI1;
	m_ChicpSelectPort = GPIOA;
	m_ChicpSelectPin  = GPIO_Pin_4;
	m_ChipSelect      = nullptr;

}

bool SpiDriver::HwInit()
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	//PortManager*  pPortManager    = GetPortManager();
	//ClockManager* pClockManager  = GetClockManager();

	if( m_SpiInstance == SPI_1_A7_A6_A5_A4)
	{
		m_SPIx = SPI1;
		//pClockManager->PeripheralClockEnable(ClockManager::APB2Periph_SPI1);
        HwClockEnable(Bsp::Peripheral::APB2Periph_SPI1);

		  /* Configure SPI1 pins: NSS, SCK, MISO and MOSI */
		  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
		  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		  GPIO_Init(GPIOA, &GPIO_InitStructure);

		  /* Configure PA.4 as Output push-pull, used as Chip select */
		  m_ChicpSelectPort = GPIOA;
		  m_ChicpSelectPin  = GPIO_Pin_4;

		  /* SPI1 configuration */
		  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		  SPI_InitStructure.SPI_Mode = (m_SpiMode == Master) ? SPI_Mode_Master : SPI_Mode_Slave;
		  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
		  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
		  SPI_InitStructure.SPI_BaudRatePrescaler = m_SpiBaudRate;
		  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
		  SPI_InitStructure.SPI_CRCPolynomial = 7;

	}
	else if(m_SpiInstance == SPI_1_B5_B4_B3_A15)
	{
		m_SPIx = SPI1;
		//pClockManager->PeripheralClockEnable(ClockManager::APB2Periph_SPI1);
		//pPortManager->PortConfig(PortManager::GPIO_Remaped_SPI1);
        HwClockEnable(Bsp::Peripheral::APB2Periph_SPI1);

		  /* Configure SPI1 pins: NSS, SCK, MISO and MOSI */
		  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_3;
		  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		  GPIO_Init(GPIOB, &GPIO_InitStructure);

		  /* Configure PA.15 as Output push-pull, used as Chip select */
		  m_ChicpSelectPort = GPIOA;
		  m_ChicpSelectPin  = GPIO_Pin_15;

		  /* SPI1 configuration */
		  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		  SPI_InitStructure.SPI_Mode = (m_SpiMode == Master) ? SPI_Mode_Master : SPI_Mode_Slave;
		  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
		  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
		  SPI_InitStructure.SPI_BaudRatePrescaler = m_SpiBaudRate;
		  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
		  SPI_InitStructure.SPI_CRCPolynomial = 7;
	}
	else if(m_SpiInstance == SPI_2_B12_B13_B14_B15)
	{
		m_SPIx = SPI2;
		//pClockManager->PeripheralClockEnable(ClockManager::APB1Periph_SPI2);
        HwClockEnable(Bsp::Peripheral::APB1Periph_SPI2);

		  /* Configure SPI2 pins: NSS, SCK, MISO and MOSI */
		  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
		  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		  GPIO_Init(GPIOB, &GPIO_InitStructure);

		  /* Configure PB.12 as Output push-pull, used as Chip select */
		  m_ChicpSelectPort = GPIOB;
		  m_ChicpSelectPin  = GPIO_Pin_12;

		  /* SPI1 configuration */
		  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		  SPI_InitStructure.SPI_Mode = (m_SpiMode == Master) ? SPI_Mode_Master : SPI_Mode_Slave;
		  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
		  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
		  SPI_InitStructure.SPI_BaudRatePrescaler = m_SpiBaudRate;
		  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
		  SPI_InitStructure.SPI_CRCPolynomial = 7;
	}
	else if(m_SpiInstance == SPI_3)
	{
		m_SPIx = SPI3;
		//pClockManager->PeripheralClockEnable(ClockManager::APB1Periph_SPI3);
        HwClockEnable(Bsp::Peripheral::APB1Periph_SPI3);

	}
	else
	{

	}

	GPIO_InitStructure.GPIO_Pin = m_ChicpSelectPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(m_ChicpSelectPort, &GPIO_InitStructure);

	static GpioOutput ChipSelectObj(m_ChicpSelectPort, m_ChicpSelectPin);
	ChipSelectObj.HwInit();
	m_ChipSelect      = &ChipSelectObj;
	ChipSelect(High);

	SPI_Init(m_SPIx, &SPI_InitStructure);
	/* Enable SPI  */
	SPI_Cmd(m_SPIx, ENABLE);

	return true;
}


void SpiDriver::ChipSelect(ChipSelect_t eChipSelect)
{
	if(eChipSelect == SpiDriver::High)
	{
		m_ChipSelect->SetOutput();
	}
	else
	{
		m_ChipSelect->ClearOutput();
	}
}

u8 SpiDriver::SendByte(u8 byte)
{
  /* Loop while DR register in not empty */
  while(SPI_I2S_GetFlagStatus(m_SPIx, FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(m_SPIx, byte);

  /* Wait to receive a byte */
  while(SPI_I2S_GetFlagStatus(m_SPIx, FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(m_SPIx);
}


u8 SpiDriver::ReadByte()
{
  return SendByte(0xA5);
}
















}

