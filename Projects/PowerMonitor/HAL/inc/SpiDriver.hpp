/*
 * SpiDriver.hpp
 *
 *  Created on: 28-Oct-2017
 *      Author: Amit Chaudhary
 */

#ifndef APP_INC_SPIDRIVER_HPP_
#define APP_INC_SPIDRIVER_HPP_

#include"Peripheral.hpp"
#include "GpioOutput.hpp"
#include "stm32f10x.h"
#include "stm32f10x_spi.h"

namespace Bsp
{

class SpiDriver : public Peripheral
{
public:
	typedef void(*SpiIsr_t)();
	typedef enum{
		Low = 0,
		High
	}ChipSelect_t;
	enum
	{
		FLAG_RXNE                      = 0x0001,
		FLAG_TXE                       = 0x0002,
		FLAG_CRCERR                    = 0x0010,
		FLAG_MODF                      = 0x0020,
		FLAG_OVR                       = 0x0040,
		FLAG_BSY                       = 0x0080,
	};
	typedef enum{
		BaudRate_2   = 0x0000,
		BaudRate_4   = 0x0008,
		BaudRate_8   = 0x0010,
		BaudRate_16  = 0x0018,
		BaudRate_32  = 0x0020,
		BaudRate_64  = 0x0028,
		BaudRate_128 = 0x0030,
		BaudRate_256 = 0x0038
	}SpiBaudRate_t;
	typedef enum{
		Master = 0,
		Slave  = 1
	}SpiMode_t;
	typedef enum{
		SPI_1_A7_A6_A5_A4 = 0,
		SPI_1_B5_B4_B3_A15,
		SPI_2_B12_B13_B14_B15,
		SPI_3
	}SpiInstance_t;

	SpiDriver(SpiInstance_t aSpi, SpiIsr_t aSpiIsr = nullptr,SpiMode_t aSpiMode = Master, SpiBaudRate_t aSpiBaudRate = BaudRate_16 );
	~SpiDriver(){};
	bool HwInit();
	u8 SendByte(u8 byte);
	u8 ReadByte();
	void ChipSelect(ChipSelect_t eChipSelect);
	void DrawBitmap(const s8 my_array[]);


private:

	SpiInstance_t m_SpiInstance;
	SpiIsr_t      m_SpiIsr;
	SpiMode_t     m_SpiMode;
	SpiBaudRate_t m_SpiBaudRate;
	SPI_TypeDef*  m_SPIx;

	GpioOutput::PORT  m_ChicpSelectPort;
	u16               m_ChicpSelectPin;
    GpioOutput*       m_ChipSelect;




};

























}




#endif /* APP_INC_SPIDRIVER_HPP_ */
