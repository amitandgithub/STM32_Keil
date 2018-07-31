/*
 * I2CDriver.cpp
 *
 *  Created on: 16-Dec-2017
 *      Author: Amit Chaudhary
 */

#include <I2CDriver.hpp>
#include "stm32f10x_i2c.h"
#include "stm32f10x_gpio.h"
#include<stdio.h>
#include <cstring>
namespace Bsp
{

I2CDriver::I2CDriver(I2CInstance_t I2CInstance, Peripheral::ISR aI2CIsr, I2CMode_t aI2CMode, I2CBaudRate_t aI2CBaudRate )
{
	m_I2CInstance = I2CInstance;
	m_I2CIsr      = aI2CIsr;
	m_I2CMode     = aI2CMode;
	m_I2CBaudRate = aI2CBaudRate;
	m_I2Cx        = nullptr;
    std::memset(static_cast<void*>(&m_Timeout[0]),0,I2C_TIMEOUT_MAX);
    m_Status = 0;
}

inline I2CDriver::I2C_STATUS I2CDriver::HwStart()
{
    vu32 Timeout = 0;
    // Wait until I2Cx is not busy anymore
    Timed(I2C_GetFlagStatus(m_I2Cx , I2C_FLAG_BUSY));
#if I2C_DEBUG
    m_Timeout[I2C_FLAG_BUSY_TIMEOUT] = Timeout;
#endif
    // Generate start condition
    I2C_GenerateSTART(m_I2Cx , ENABLE);

    // Wait for I2C EV5.
    // It means that the start condition has been correctly released
    // on the I2C bus (the bus is free, no other devices is communicating))
    Timed (! I2C_CheckEvent(m_I2Cx , I2C_EVENT_MASTER_MODE_SELECT));

    // Log how much time it took for this operation
#if I2C_DEBUG
    m_Timeout[I2C_EVENT_MASTER_MODE_SELECT_TIMEOUT] = Timeout;
#endif

    return I2C_ERROR_NONE;
errReturn:
#if I2C_DEBUG
    m_Status |= I2C_ERROR_START_CONDITION_TIMEOUT;
#endif
	return I2C_ERROR_START_CONDITION_TIMEOUT;
}

inline I2CDriver::I2C_STATUS I2CDriver::HwStop()
{
    vu32 Timeout = 0;
    // Generate I2C stop condition
    I2C_GenerateSTOP(m_I2Cx, ENABLE);

    // Wait until I2C stop condition is finished
    Timed(I2C_GetFlagStatus(m_I2Cx , I2C_FLAG_STOPF));
#if I2C_DEBUG
    m_Timeout[I2C_FLAG_STOPF_TIMEOUT] = Timeout;
#endif
    return I2C_ERROR_NONE;
errReturn:
#if I2C_DEBUG
    m_Status |= I2C_ERROR_STOP_CONDITION_TIMEOUT;
#endif
	return I2C_ERROR_STOP_CONDITION_TIMEOUT;
}
uint8_t I2CDriver::HwReceiveWithACK()
{
    vu32 Timeout = 0;
    // Enable ACK of received data
    I2C_AcknowledgeConfig(m_I2Cx, ENABLE);

    // Wait for I2C EV7
    // It means that the data has been received in I2C data register
    Timed(!I2C_CheckEvent(m_I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));

#if I2C_DEBUG
    m_Timeout[I2C_EVENT_MASTER_BYTE_RECEIVED_TIMEOUT] = Timeout;
#endif
    // Read and return data byte from I2C data register
    return I2C_ReceiveData(m_I2Cx);
errReturn:
#if I2C_DEBUG
    m_Status |= I2C_ERROR_RECEIVE_WITH_ACK_TIMEOUT;
#endif
	return 0xFF;
}

uint8_t I2CDriver::HwReceiveWithNACK()
{
    vu32 Timeout = 0;
    // Disable ACK of received data
    I2C_AcknowledgeConfig(m_I2Cx, DISABLE);

    // Wait for I2C EV7
    // It means that the data has been received in I2C data register
    Timed (!I2C_CheckEvent(m_I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));

#if I2C_DEBUG
    m_Timeout[I2C_EVENT_MASTER_BYTE_RECEIVED_TIMEOUT] = Timeout;
#endif
    // Read and return data byte from I2C data register
    return I2C_ReceiveData(m_I2Cx);
errReturn:
#if I2C_DEBUG
    m_Status |= I2C_ERROR_RECEIVE_WITH_NACK_TIMEOUT;
#endif
	return 0xFF;
}
inline void I2CDriver::HwReset()
{
    if(m_I2Cx == I2C1)
    {
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1 , ENABLE);
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1 , DISABLE);
    }
    else if (m_I2Cx == I2C2)
    {
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2 , ENABLE);
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2 , DISABLE);
    }
    else
    {

    }
}
inline I2CDriver::I2C_STATUS I2CDriver::Transmit(uint8_t Data)
{
    vu32 Timeout = 0;
    /*
    // Wait until I2Cx is not busy anymore
    Timed(I2C_GetFlagStatus(m_I2Cx , I2C_FLAG_BUSY));
#if I2C_DEBUG
    m_Timeout[I2C_FLAG_BUSY_TIMEOUT] = Timeout;
#endif */
    // Send data byte
    HwWriteDataRegistor(Data);

    // Wait for I2C EV8_2.
    // It means that the data has been physically shifted out and
    // output on the bus)
    Timed(!I2C_CheckEvent(m_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    // Log how much time it took for this operation
#if I2C_DEBUG
    m_Timeout[I2C_EVENT_MASTER_BYTE_TRANSMITTED_TIMEOUT] = Timeout;
#endif

    return I2C_ERROR_NONE;
errReturn:
#if I2C_DEBUG
    m_Status |= I2C_ERROR_DATA_TRANSMISSION_TIMEOUT;
#endif
	return I2C_ERROR_DATA_TRANSMISSION_TIMEOUT;

}
void I2CDriver::HwSendByte(u8 SlaveAddress, u8 data)
{
    m_Status |=HwStart();
    m_Status |=HwSendAddressWithDirection(SlaveAddress, Write);
    m_Status |=Transmit(data);
    m_Status |=HwStop();
}

uint8_t I2CDriver::HwReadByte(u8 SlaveAddress)
{
    uint8_t Data;
    m_Status |=HwStart();
    m_Status |=HwSendAddressWithDirection(SlaveAddress, Read);
    Data = HwReceiveWithNACK();
    m_Status |= HwStop();
    return Data;
}

I2CDriver::I2C_STATUS I2CDriver::HwSendAddressWithDirection(uint8_t address, I2CDir_t direction)
{
    vu32 Timeout = 0;
    // Send slave address
    I2C_Send7bitAddress(m_I2Cx, address, direction);

    // Wait for I2C EV6
    // It means that a slave acknowledges his address

    if (direction == Write) // Write Direction
    {
        Timed(!I2C_CheckEvent(m_I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
#if I2C_DEBUG
        m_Timeout[I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED_TIMEOUT] = Timeout;
#endif
    }
    else if (direction == Read) // Read Direction
    {
        Timed(!I2C_CheckEvent(m_I2Cx,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
#if I2C_DEBUG
        m_Timeout[I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED_TIMEOUT] = Timeout;
#endif
    }
    return I2C_ERROR_NONE;
errReturn:
#if I2C_DEBUG
    m_Status |= I2C_ERROR_STOP_CONDITION_TIMEOUT;
#endif
	return I2C_ERROR_STOP_CONDITION_TIMEOUT;
}

inline uint8_t I2CDriver::HwReadDataRegistor()
{
    return I2C_ReceiveData(m_I2Cx);
}
inline void I2CDriver::HwWriteDataRegistor(uint8_t Data)
{
    I2C_SendData(m_I2Cx,Data);
}

uint8_t I2CDriver::ScanBus(uint8_t* pResult, uint8_t Len)
{
    uint8_t index=0;
    for(volatile uint8_t i = 0; i<0xff;i++)
    {
        if( ScanDevice(i) == 0 )
        {
            pResult[(index++)%Len] = i;
        }
        HwInit();
    }
    return index;
}

I2CDriver::~I2CDriver()
{

}
uint8_t I2CDriver::ScanDevice(uint8_t SlaveAddress)
{
    static vu32 Timeout = 0;

    Timed(I2C_GetFlagStatus(m_I2Cx , I2C_FLAG_BUSY));
#if I2C_DEBUG
        m_Timeout[I2C_FLAG_BUSY_TIMEOUT] = Timeout;
#endif
    // Intiate Start Sequence
    I2C_GenerateSTART(m_I2Cx , ENABLE);
    Timed (! I2C_CheckEvent(m_I2Cx , I2C_EVENT_MASTER_MODE_SELECT));
#if I2C_DEBUG
        m_Timeout[I2C_EVENT_MASTER_MODE_SELECT_TIMEOUT] = Timeout;
#endif
    // Send Address EV5
    I2C_Send7bitAddress(m_I2Cx , SlaveAddress ,	I2C_Direction_Transmitter);
    Timed (! I2C_CheckEvent(m_I2Cx , I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
#if I2C_DEBUG
        m_Timeout[I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED_TIMEOUT] = Timeout;
#endif
    return 0;
errReturn:
#if I2C_DEBUG
    m_Status |= I2C_ERROR_STOP_CONDITION_TIMEOUT;
#endif
    return 1;
}

bool I2CDriver::HwInit()
{
	I2C_InitTypeDef I2cConfig;
	GPIO_InitTypeDef GPIO_InitStructure;

	if( m_I2CInstance == I2C1_B6_B7)
	{
        m_I2Cx = I2C1;
        HwClockEnable(Bsp::Peripheral::APB1Periph_I2C1);
        HwClockEnable(Bsp::Peripheral::APB2Periph_GPIOB);

        /* Configure I2C SCL and SDA as Alternate Output Push Pull configuration */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;

        /* I2C1 Reset */
        HwReset();
	}
	else if( m_I2CInstance == I2C1_B8_B9)
	{
        m_I2Cx = I2C1;
        HwClockEnable(Bsp::Peripheral::APB1Periph_I2C1);
        HwClockEnable(Bsp::Peripheral::APB2Periph_GPIOB);

        /* Configure I2C SCL and SDA as Alternate Output Push Pull configuration */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;

        /* I2C1 Reset */
        HwReset();
	}
    else if( m_I2CInstance == I2C2_B10_B11)
	{
        m_I2Cx = I2C2;
        HwClockEnable(Bsp::Peripheral::APB1Periph_I2C2);
        HwClockEnable(Bsp::Peripheral::APB2Periph_GPIOB);

        /* Configure I2C SCL and SDA as Alternate Output Push Pull configuration */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;

        /* I2C2 Reset */
        HwReset();
	}

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init( GPIOB, &GPIO_InitStructure );

    I2cConfig.I2C_Ack = I2C_Ack_Enable;
    I2cConfig.I2C_Mode = I2C_Mode_I2C;
    I2cConfig.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2cConfig.I2C_DutyCycle = I2C_DutyCycle_2;
    I2cConfig.I2C_ClockSpeed = m_I2CBaudRate;
    I2cConfig.I2C_OwnAddress1 = 0x50;

	I2C_Init(m_I2Cx,&I2cConfig);
	I2C_Cmd(m_I2Cx,ENABLE);
    return 1;
}
I2CDriver::I2C_STATUS I2CDriver::Send(u8 SlaveAddress, u8* pBuf, u32 Bytes)
{
	volatile uint32_t Timeout = 0;
	if (Bytes)
	{
        m_Status |= HwStart();
        HwSendAddressWithDirection(SlaveAddress, Write);

		// EV6 Write first byte EV8_1
        m_Status |= Transmit(*pBuf++);

		while (--Bytes)
		{
            m_Status |= Transmit(*pBuf++);
		}
        m_Status |= HwStop();
    }
	return I2C_ERROR_NONE;
}

I2CDriver::I2C_STATUS I2CDriver::Receive(u8 SlaveAddress, u8* pBuf, u32 Bytes)
{
    static vu32 Timeout = 0;
    if (!Bytes)
	    return I2C_ERROR_NONE;

    // Wait for idle I2C interface

    Timed(I2C_GetFlagStatus(m_I2Cx, I2C_FLAG_BUSY));

    // Enable Acknowledgement, clear POS flag

    I2C_AcknowledgeConfig(m_I2Cx, ENABLE);
    I2C_PECPositionConfig(m_I2Cx, I2C_PECPosition_Current);

    // Intiate Start Sequence (wait for EV5

    I2C_GenerateSTART(m_I2Cx, ENABLE);
    Timed(!I2C_CheckEvent(m_I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

    // Send Address

    I2C_Send7bitAddress(m_I2Cx, SlaveAddress, I2C_Direction_Receiver);

    // EV6

    Timed(!I2C_GetFlagStatus(m_I2Cx, I2C_FLAG_ADDR));

    if (Bytes == 1)
    {

        // Clear Ack bit

        I2C_AcknowledgeConfig(m_I2Cx, DISABLE);

        // EV6_1 -- must be atomic -- Clear ADDR, generate STOP

        __disable_irq();
        (void) m_I2Cx->SR2;
        I2C_GenerateSTOP(m_I2Cx,ENABLE);
        __enable_irq();

        // Receive data   EV7

        Timed(!I2C_GetFlagStatus(m_I2Cx, I2C_FLAG_RXNE));
        *pBuf++ = I2C_ReceiveData(m_I2Cx);

    }
    else if (Bytes == 2)
    {
        // Set POS flag

        I2C_PECPositionConfig(m_I2Cx, I2C_PECPosition_Next);

        // EV6_1 -- must be atomic and in this order

        __disable_irq();
        (void) m_I2Cx->SR2;                           // Clear ADDR flag
        I2C_AcknowledgeConfig(m_I2Cx, DISABLE);       // Clear Ack bit
        __enable_irq();

        // EV7_3  -- Wait for BTF, program stop, read data twice

        Timed(!I2C_GetFlagStatus(m_I2Cx, I2C_FLAG_BTF));

        __disable_irq();
        I2C_GenerateSTOP(m_I2Cx,ENABLE);
        *pBuf++ = m_I2Cx->DR;
        __enable_irq();

        *pBuf++ = m_I2Cx->DR;

    }
    else
    {
        (void) m_I2Cx->SR2;                           // Clear ADDR flag
        while (Bytes-- != 3)
		{
            // EV7 -- cannot guarantee 1 transfer completion time, wait for BTF
            //        instead of RXNE

            Timed(!I2C_GetFlagStatus(m_I2Cx, I2C_FLAG_BTF));
            *pBuf++ = I2C_ReceiveData(m_I2Cx);
		}

        Timed(!I2C_GetFlagStatus(m_I2Cx, I2C_FLAG_BTF));

        // EV7_2 -- Figure 1 has an error, doesn't read N-2 !

        I2C_AcknowledgeConfig(m_I2Cx, DISABLE);           // clear ack bit

        __disable_irq();
        *pBuf++ = I2C_ReceiveData(m_I2Cx);             // receive byte N-2
        I2C_GenerateSTOP(m_I2Cx,ENABLE);                  // program stop
        __enable_irq();

        *pBuf++ = I2C_ReceiveData(m_I2Cx);             // receive byte N-1

        // wait for byte N

        Timed(!I2C_CheckEvent(m_I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));
        *pBuf++ = I2C_ReceiveData(m_I2Cx);

        Bytes = 0;

    }
    // Wait for stop

    Timed(I2C_GetFlagStatus(m_I2Cx, I2C_FLAG_STOPF));
    return I2C_ERROR_BUSY_TRANSMITTING;

errReturn:
    HwInit();
#if I2C_DEBUG
    m_Status |= I2C_ERROR_STOP_CONDITION_TIMEOUT;
#endif
    return I2C_ERROR_NONE;
}



} /* namespace Bsp */
