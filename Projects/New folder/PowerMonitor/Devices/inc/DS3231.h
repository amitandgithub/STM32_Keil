/*
  DS3231.cpp - Arduino/chipKit library support for the DS3231 I2C Real-Time Clock
  Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved

  This library has been made to easily interface and use the DS3231 RTC with
  an Arduino or chipKit.

  You can find the latest version of the library at
  http://www.RinkyDinkElectronics.com/

  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.

  Commercial use of this library requires you to buy a license that
  will allow commercial use. This includes using the library,
  modified or not, as a tool to sell products.

  The license applies to all part of the library including the
  examples and tools supplied with the library.
*/
#ifndef DS3231_h
#define DS3231_h

#include "stm32f10x.h"

#include <I2CDriver.hpp>




class DateNTime
{
public:

	uint8_t		hour;
	uint8_t		min;
	uint8_t		sec;
	uint8_t		date;
	uint8_t		mon;
	uint16_t	year;
	uint8_t		dow;

	DateNTime();
};

class DS3231
{
	public:
        typedef uint8_t byte;
        typedef bool boolean;
		DS3231(Bsp::I2CDriver* pI2CDrv, uint8_t Address);
		void	begin();
		DateNTime	getTime();
        unsigned char getSec();
        char*	getTimeStr(char* pBuffer);
		void	setTime(int8_t hour, int8_t min, int8_t sec);
		void	setDate(uint8_t date, uint8_t mon, uint16_t year);
		void	setDOW();
		void	setDOW(uint8_t dow);
        void    setLocalTime();

		char	*getTimeStr(uint8_t format=FORMAT_LONG);
		char	*getDateStr(uint8_t slformat=FORMAT_LONG, uint8_t eformat=FORMAT_LITTLEENDIAN, char divider='.');
		const char	*getDOWStr(uint8_t format=FORMAT_LONG);
		const char	*getMonthStr(uint8_t format=FORMAT_LONG);
		long	getUnixTime(DateNTime t);

		void	enable32KHz(bool enable);
		void	setOutput(byte enable);
		void	setSQWRate(int rate);
		float	getTemp();

	private:
		uint8_t _scl_pin;
		uint8_t _sda_pin;
		uint8_t _burstArray[7];
		//boolean	_use_hw;

		//void	_sendStart(byte addr);
		//void	_sendStop();
		//void	_sendAck();
		//void	_sendNack();
		//void	_waitForAck();
		uint8_t	_readByte();
		void	_writeByte(uint8_t value);
		void	_burstRead();
		uint8_t	_readRegister(uint8_t reg);
		void 	_writeRegister(uint8_t reg, uint8_t value);
		uint8_t	_decode(uint8_t value);
		uint8_t	_decodeH(uint8_t value);
		uint8_t	_decodeY(uint8_t value);
		uint8_t	_encode(uint8_t vaule);
        uint8_t m_DS3231_Address;
        Bsp::I2CDriver* m_pI2CDrv;
public:
        static const uint8_t FORMAT_SHORT	= 1;
        static const uint8_t FORMAT_LONG	= 2;

        static const uint8_t FORMAT_LITTLEENDIAN	= 1;
        static const uint8_t FORMAT_BIGENDIAN	    = 2;
        static const uint8_t FORMAT_MIDDLEENDIAN	= 3;

        static const uint8_t MONDAY		= 1;
        static const uint8_t TUESDAY	= 2;
        static const uint8_t WEDNESDAY	= 3;
        static const uint8_t THURSDAY	= 4;
        static const uint8_t FRIDAY		= 5;
        static const uint8_t SATURDAY	= 6;
        static const uint8_t SUNDAY		= 7;

        static const uint8_t SQW_RATE_1		= 0;
        static const uint8_t SQW_RATE_1K	= 1;
        static const uint8_t SQW_RATE_4K	= 2;
        static const uint8_t SQW_RATE_8K	= 3;

        static const uint8_t OUTPUT_SQW		= 0;
        static const uint8_t OUTPUT_INT		= 1;
        static const uint8_t REG_SEC	 = 0x00;
        static const uint8_t REG_MIN	 = 0x01;
        static const uint8_t REG_HOUR	 = 0x02;
        static const uint8_t REG_DOW	 = 0x03;
        static const uint8_t REG_DATE	 = 0x04;
        static const uint8_t REG_MON	 = 0x05;
        static const uint8_t REG_YEAR	 = 0x06;
        static const uint8_t REG_CON	 = 0x0e;
        static const uint8_t REG_STATUS	 = 0x0f;
        static const uint8_t REG_AGING	 = 0x10;
        static const uint8_t REG_TEMPM	 = 0x11;
        static const uint8_t REG_TEMPL	 = 0x12;

        static const uint32_t SEC_1970_TO_2000  = 946684800;
};
#endif
