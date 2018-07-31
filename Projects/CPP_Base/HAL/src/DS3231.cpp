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
#include <time.h>
#include <stdio.h>
#include "DS3231.h"
#include "Test.h"

static const uint8_t dim[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

/* Public */

DateNTime::DateNTime()
{
	this->year = 2018;
	this->mon  = 1;
	this->date = 20;
	this->hour = 0;
	this->min  = 0;
	this->sec  = 0;
	this->dow  = 5;
}

unsigned char DS3231::getSec()
{
    DateNTime t = getTime();
    return t.sec;
}
DS3231::DS3231(Bsp::I2CDriver* pI2CDrv, uint8_t Address)
{
    m_pI2CDrv        = pI2CDrv;
    m_DS3231_Address = Address;
}

DateNTime DS3231::getTime()
{
	DateNTime t;
	_burstRead();
	t.sec	= _decode(_burstArray[0]);
	t.min	= _decode(_burstArray[1]);
	t.hour	= _decodeH(_burstArray[2]);
	t.dow	= _burstArray[3];
	t.date	= _decode(_burstArray[4]);
	t.mon	= _decode(_burstArray[5]);
	t.year	= _decodeY(_burstArray[6])+2000;
	return t;
}

void DS3231::setTime(int8_t hour, int8_t min, int8_t sec)
{
	if (((hour>=0) && (hour<24)) && ((min>=0) && (min<60)) && ((sec>=0) && (sec<60)))
	{
		_writeRegister(REG_HOUR, _encode(hour));
		_writeRegister(REG_MIN, _encode(min));
		_writeRegister(REG_SEC, _encode(sec));
	}
}

void DS3231::setLocalTime()
{
    //uint8_t date, uint8_t mon, uint16_t year;
    time_t mytime;
    mytime = time(NULL);
    struct tm *loc_time;
    loc_time = localtime (&mytime);
    uint8_t WOD = ( loc_time->tm_wday ) == 0 ? SUNDAY :
                  ( loc_time->tm_wday ) == 1 ? MONDAY :
                  ( loc_time->tm_wday ) == 2 ? TUESDAY :
                  ( loc_time->tm_wday ) == 3 ? WEDNESDAY :
                  ( loc_time->tm_wday ) == 4 ? THURSDAY :
                  ( loc_time->tm_wday ) == 5 ? FRIDAY :
                  ( loc_time->tm_wday ) == 6 ? SATURDAY : 0;

    setDate(loc_time->tm_mday,loc_time->tm_mon + 1,loc_time->tm_year + 1900);
    setTime((loc_time->tm_hour+6)%24,(loc_time->tm_min - 30 > 0? loc_time->tm_min - 30 : loc_time->tm_min - 30 + 60),loc_time->tm_sec);
    setDOW(WOD);
}

void DS3231::setDate(uint8_t date, uint8_t mon, uint16_t year)
{
	if (((date>0) && (date<=31)) && ((mon>0) && (mon<=12)) && ((year>=2000) && (year<3000)))
	{
		year -= 2000;
		_writeRegister(REG_YEAR, _encode(year));
		_writeRegister(REG_MON, _encode(mon));
		_writeRegister(REG_DATE, _encode(date));
	}
}

void DS3231::setDOW()
{
	int dow;
	byte mArr[12] = {6,2,2,5,0,3,5,1,4,6,2,4};
	DateNTime _t = getTime();

	dow = (_t.year % 100);
	dow = dow*1.25;
	dow += _t.date;
	dow += mArr[_t.mon-1];
	if (((_t.year % 4)==0) && (_t.mon<3))
		dow -= 1;
	while (dow>7)
		dow -= 7;
	_writeRegister(REG_DOW, dow);
}

void DS3231::setDOW(uint8_t dow)
{
	if ((dow>0) && (dow<8))
		_writeRegister(REG_DOW, dow);
}

char *DS3231::getTimeStr(uint8_t format)
{
	static char output[] = "xxxxxxxx";
	DateNTime t;
	t=getTime();
	if (t.hour<10)
		output[0]=48;
	else
		output[0]=char((t.hour / 10)+48);
	output[1]=char((t.hour % 10)+48);
	output[2]=58;
	if (t.min<10)
		output[3]=48;
	else
		output[3]=char((t.min / 10)+48);
	output[4]=char((t.min % 10)+48);
	output[5]=58;
	if (format==FORMAT_SHORT)
		output[5]=0;
	else
	{
	if (t.sec<10)
		output[6]=48;
	else
		output[6]=char((t.sec / 10)+48);
	output[7]=char((t.sec % 10)+48);
	output[8]=0;
	}
	return (char*)&output;
}

char *DS3231::getDateStr(uint8_t slformat, uint8_t eformat, char divider)
{
	static char output[] = "xxxxxxxxxx";
	int yr, offset;
	DateNTime t;
	t=getTime();
	switch (eformat)
	{
		case FORMAT_LITTLEENDIAN:
			if (t.date<10)
				output[0]=48;
			else
				output[0]=char((t.date / 10)+48);
			output[1]=char((t.date % 10)+48);
			output[2]=divider;
			if (t.mon<10)
				output[3]=48;
			else
				output[3]=char((t.mon / 10)+48);
			output[4]=char((t.mon % 10)+48);
			output[5]=divider;
			if (slformat==FORMAT_SHORT)
			{
				yr=t.year-2000;
				if (yr<10)
					output[6]=48;
				else
					output[6]=char((yr / 10)+48);
				output[7]=char((yr % 10)+48);
				output[8]=0;
			}
			else
			{
				yr=t.year;
				output[6]=char((yr / 1000)+48);
				output[7]=char(((yr % 1000) / 100)+48);
				output[8]=char(((yr % 100) / 10)+48);
				output[9]=char((yr % 10)+48);
				output[10]=0;
			}
			break;
		case FORMAT_BIGENDIAN:
			if (slformat==FORMAT_SHORT)
				offset=0;
			else
				offset=2;
			if (slformat==FORMAT_SHORT)
			{
				yr=t.year-2000;
				if (yr<10)
					output[0]=48;
				else
					output[0]=char((yr / 10)+48);
				output[1]=char((yr % 10)+48);
				output[2]=divider;
			}
			else
			{
				yr=t.year;
				output[0]=char((yr / 1000)+48);
				output[1]=char(((yr % 1000) / 100)+48);
				output[2]=char(((yr % 100) / 10)+48);
				output[3]=char((yr % 10)+48);
				output[4]=divider;
			}
			if (t.mon<10)
				output[3+offset]=48;
			else
				output[3+offset]=char((t.mon / 10)+48);
			output[4+offset]=char((t.mon % 10)+48);
			output[5+offset]=divider;
			if (t.date<10)
				output[6+offset]=48;
			else
				output[6+offset]=char((t.date / 10)+48);
			output[7+offset]=char((t.date % 10)+48);
			output[8+offset]=0;
			break;
		case FORMAT_MIDDLEENDIAN:
			if (t.mon<10)
				output[0]=48;
			else
				output[0]=char((t.mon / 10)+48);
			output[1]=char((t.mon % 10)+48);
			output[2]=divider;
			if (t.date<10)
				output[3]=48;
			else
				output[3]=char((t.date / 10)+48);
			output[4]=char((t.date % 10)+48);
			output[5]=divider;
			if (slformat==FORMAT_SHORT)
			{
				yr=t.year-2000;
				if (yr<10)
					output[6]=48;
				else
					output[6]=char((yr / 10)+48);
				output[7]=char((yr % 10)+48);
				output[8]=0;
			}
			else
			{
				yr=t.year;
				output[6]=char((yr / 1000)+48);
				output[7]=char(((yr % 1000) / 100)+48);
				output[8]=char(((yr % 100) / 10)+48);
				output[9]=char((yr % 10)+48);
				output[10]=0;
			}
			break;
	}
	return (char*)&output;
}

const char *DS3231::getDOWStr(uint8_t format)
{
	const char *output = "xxxxxxxxxx";
	const char *daysLong[]  = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
	const char *daysShort[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
	DateNTime t;
	t=getTime();
	if (format == FORMAT_SHORT)
		output = daysShort[t.dow-1];
	else
		output = daysLong[t.dow-1];
	return output;
}

const char *DS3231::getMonthStr(uint8_t format)
{
	const char *output= "xxxxxxxxx";
	const char *monthLong[]  = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
	const char *monthShort[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	DateNTime t;
	t=getTime();
	if (format == FORMAT_SHORT)
		output = monthShort[t.mon-1];
	else
		output = monthLong[t.mon-1];
	return output;
}

long DS3231::getUnixTime(DateNTime t)
{
	uint16_t	dc;

	dc = t.date;
	for (uint8_t i = 0; i<(t.mon-1); i++)
		dc += dim[i];
	if ((t.mon > 2) && (((t.year-2000) % 4) == 0))
		++dc;
	dc = dc + (365 * (t.year-2000)) + (((t.year-2000) + 3) / 4) - 1;

	return ((((((dc * 24L) + t.hour) * 60) + t.min) * 60) + t.sec) + SEC_1970_TO_2000;

}

void DS3231::enable32KHz(bool enable)
{
  uint8_t _reg = _readRegister(REG_STATUS);
  _reg &= ~(1 << 3);
  _reg |= (enable << 3);
  _writeRegister(REG_STATUS, _reg);
}

void DS3231::setOutput(byte enable)
{
  uint8_t _reg = _readRegister(REG_CON);
  _reg &= ~(1 << 2);
  _reg |= (enable << 2);
  _writeRegister(REG_CON, _reg);
}

void DS3231::setSQWRate(int rate)
{
  uint8_t _reg = _readRegister(REG_CON);
  _reg &= ~(3 << 3);
  _reg |= (rate << 3);
  _writeRegister(REG_CON, _reg);
}

float DS3231::getTemp()
{
	uint8_t _msb = _readRegister(REG_TEMPM);
	uint8_t _lsb = _readRegister(REG_TEMPL);
	return (float)_msb + ((_lsb >> 6) * 0.25f);
}

/* Private */
#if 0
void	DS3231::_sendStart(byte addr)
{
	pinMode(_sda_pin, OUTPUT);
	digitalWrite(_sda_pin, HIGH);
	digitalWrite(_scl_pin, HIGH);
	digitalWrite(_sda_pin, LOW);
	digitalWrite(_scl_pin, LOW);
	shiftOut(_sda_pin, _scl_pin, MSBFIRST, addr);
}

void	DS3231::_sendStop()
{
	pinMode(_sda_pin, OUTPUT);
	digitalWrite(_sda_pin, LOW);
	digitalWrite(_scl_pin, HIGH);
	digitalWrite(_sda_pin, HIGH);
	pinMode(_sda_pin, INPUT);
}

void	DS3231::_sendNack()
{
	pinMode(_sda_pin, OUTPUT);
	digitalWrite(_scl_pin, LOW);
	digitalWrite(_sda_pin, HIGH);
	digitalWrite(_scl_pin, HIGH);
	digitalWrite(_scl_pin, LOW);
	pinMode(_sda_pin, INPUT);
}

void	DS3231::_sendAck()
{
	pinMode(_sda_pin, OUTPUT);
	digitalWrite(_scl_pin, LOW);
	digitalWrite(_sda_pin, LOW);
	digitalWrite(_scl_pin, HIGH);
	digitalWrite(_scl_pin, LOW);
	pinMode(_sda_pin, INPUT);
}

void	DS3231::_waitForAck()
{
	pinMode(_sda_pin, INPUT);
	digitalWrite(_scl_pin, HIGH);
	while (digitalRead(_sda_pin)==HIGH) {}
	digitalWrite(_scl_pin, LOW);
}

uint8_t DS3231::_readByte()
{
	pinMode(_sda_pin, INPUT);

	uint8_t value = 0;
	uint8_t currentBit = 0;

	for (int i = 0; i < 8; ++i)
	{
		digitalWrite(_scl_pin, HIGH);
		currentBit = digitalRead(_sda_pin);
		value |= (currentBit << 7-i);
		delayMicroseconds(1);
		digitalWrite(_scl_pin, LOW);
	}
	return value;
}

void DS3231::_writeByte(uint8_t value)
{
	pinMode(_sda_pin, OUTPUT);
	shiftOut(_sda_pin, _scl_pin, MSBFIRST, value);
}
#endif
uint8_t	DS3231::_decode(uint8_t value)
{
	uint8_t decoded = value & 127;
	decoded = (decoded & 15) + 10 * ((decoded & (15 << 4)) >> 4);
	return decoded;
}

uint8_t DS3231::_decodeH(uint8_t value)
{
  if (value & 128)
    value = (value & 15) + (12 * ((value & 32) >> 5));
  else
    value = (value & 15) + (10 * ((value & 48) >> 4));
  return value;
}

uint8_t	DS3231::_decodeY(uint8_t value)
{
	uint8_t decoded = (value & 15) + 10 * ((value & (15 << 4)) >> 4);
	return decoded;
}

uint8_t DS3231::_encode(uint8_t value)
{
	uint8_t encoded = ((value / 10) << 4) + (value % 10);
	return encoded;
}

void DS3231::_writeRegister(uint8_t reg, uint8_t value)
{
    uint8_t buf[3];
	buf[0] = reg;
    buf[1] = value;
    m_pI2CDrv->Send( m_DS3231_Address, buf, 2) ;

}

uint8_t DS3231::_readRegister(uint8_t reg)
{
    uint8_t buf[3];
	buf[0] = reg;
    m_pI2CDrv->Send( m_DS3231_Address, buf, 1) ;
    m_pI2CDrv->Receive( m_DS3231_Address, buf, 1);
    return buf[0];
}

void DS3231::begin()
{
    m_pI2CDrv->HwInit();
}

char* DS3231::getTimeStr(char* pBuffer)
{
    DateNTime t;
    t = getTime();
    // Date
    intToStr(t.date, &pBuffer[0], 2);
    pBuffer[2] = '-';
    intToStr(t.mon, &pBuffer[3], 2);
    pBuffer[5] = '-';
    intToStr(t.year, &pBuffer[6], 4);
    // Time (startsw with 10)
    intToStr(t.hour, &pBuffer[10], 2);
    pBuffer[12] = ':';
    intToStr(t.min, &pBuffer[13], 2);
    pBuffer[15] = ':';
    intToStr(t.sec, &pBuffer[16], 2);
    return pBuffer;

}

void DS3231::_burstRead()
{
    uint8_t buf[3];
    buf[0] = 0;
    // Send 0, and read seven bytes
    m_pI2CDrv->Send   ( m_DS3231_Address, buf, 1) ;
    m_pI2CDrv->Receive( m_DS3231_Address, _burstArray, 7);
}

