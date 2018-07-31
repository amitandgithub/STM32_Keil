/*
 * I2CDriver.hpp
 *
 *  Created on: 16-Dec-2017
 *      Author: Amit Chaudhary
 */

#ifndef APP_INC_I2CDRIVER_HPP_
#define APP_INC_I2CDRIVER_HPP_

#include"Peripheral.hpp"
#include "stm32f10x.h"

#define Timed(x) Timeout = 0xFFFF; while (x) { if (Timeout -- == 0) goto errReturn ;}
namespace Bsp
{
#define I2C_DEBUG 1
class I2CDriver : public Peripheral
{
public:
    typedef enum
    {
        I2C_ERROR_NONE = 0,
        I2C_ERROR_START_CONDITION_TIMEOUT   = (1UL << 0),
        I2C_ERROR_STOP_CONDITION_TIMEOUT    = (1UL << 1),
        I2C_ERROR_DATA_TRANSMISSION_TIMEOUT = (1UL << 2),
        I2C_ERROR_RECIEVE_NACK              = (1UL << 3),
        I2C_ERROR_BUSY_TRANSMITTING         = (1UL << 4),
        I2C_ERROR_BUSY_RECEIVING            = (1UL << 5),
        I2C_ERROR_RECEIVE_WITH_ACK_TIMEOUT  = (1UL << 6),
        I2C_ERROR_RECEIVE_WITH_NACK_TIMEOUT = (1UL << 7)
    }I2C_STATUS;
    typedef enum
    {
        I2C_EVENT_MASTER_BYTE_RECEIVED_TIMEOUT = 0,
        I2C_EVENT_MASTER_MODE_SELECT_TIMEOUT = 1,
        I2C_EVENT_MASTER_BYTE_TRANSMITTED_TIMEOUT = 2,
        I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED_TIMEOUT = 3,
        I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED_TIMEOUT = 4,
        I2C_FLAG_BTF_TIMEOUT = 5,
        I2C_FLAG_STOPF_TIMEOUT = 6,
        I2C_FLAG_BUSY_TIMEOUT = 7,
        I2C_FLAG_ADDR_TIMEOUT = 8,
        I2C_FLAG_RXNE_TIMEOUT = 9,
        I2C_TIMEOUT_MAX = 10
    }I2C_TIMEOUT;
	typedef enum{
		I2C1_B6_B7 = 0,
		I2C1_B8_B9,
		I2C2_B10_B11
	}I2CInstance_t;
	typedef enum{
		BaudRate_100000   = 100000,
		BaudRate_400000   = 400000
	}I2CBaudRate_t;
	typedef enum{
		Master = 0,
		Slave  = 1
	}I2CMode_t;
    	typedef enum{
		Write = 0,
		Read  = 1
	}I2CDir_t;

	virtual ~I2CDriver();
	I2CDriver(I2CInstance_t I2CInstance, Peripheral::ISR aI2CIsr = nullptr, I2CMode_t I2CMode = Master, I2CBaudRate_t I2CBaudRate = BaudRate_100000 );
	bool HwInit();
	I2C_STATUS HwStart();
	I2C_STATUS HwStop();
	I2C_STATUS HwSendAddressWithDirection(uint8_t address, I2CDir_t direction);
	void HwReset();
	uint8_t HwReadDataRegistor();
	void HwWriteDataRegistor(uint8_t Data);
    I2C_STATUS Transmit(uint8_t byte);
    void HwSendByte(u8 SlaveAddress, u8 data);
	I2C_STATUS Send(u8 SlaveAddress, u8* pBuf, u32 Bytes);
    u8 HwReadByte(u8 SlaveAddress);
    u8 HwReceiveWithACK();
    u8 HwReceiveWithNACK();
	I2C_STATUS Receive(u8 SlaveAddress, u8* pBuf, u32 Bytes);
	uint8_t ScanDevice(uint8_t SlaveAddress);
    uint8_t ScanBus(uint8_t* pResult, uint8_t Len);
private :
	I2CInstance_t        m_I2CInstance;
	Peripheral::ISR      m_I2CIsr;
	I2CMode_t            m_I2CMode;
	I2CBaudRate_t        m_I2CBaudRate;
	I2C_TypeDef*         m_I2Cx;
#if I2C_DEBUG
    uint32_t             m_Timeout[I2C_TIMEOUT_MAX];
    uint32_t             m_Status;
#endif

};

} /* namespace Bsp */

#endif /* APP_INC_I2CDRIVER_HPP_ */
