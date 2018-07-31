/*
 * Nokia5110LCD.hpp
 *
 *  Created on: 28-Oct-2017
 *      Author: Amit Chaudhary
 */

#ifndef APP_INC_NOKIA5110LCD_HPP_
#define APP_INC_NOKIA5110LCD_HPP_



#include"Gpio.hpp"
#include"SpiDriver.hpp"
#include"GpioInput.hpp"
#include"GpioOutput.hpp"

using namespace Bsp; // Not to be placed before Header includes

namespace App
{



class Nokia5110LCD
{
public:
	typedef enum
	{
		COMMAND = 0,
		DATA
	}DC_t;

	static const u8 NORMAL  = 0x0C;
	static const u8 INVERSE = 0x0D;

	static const u8 LCD_X  = 84;
	static const u8 LCD_Y  = 48;

	static const u8 SIZE_OF_1_CHAR = 6;
    static const u8 NO_OF_CHAR_IN_LINE = 14;

	Nokia5110LCD(SpiDriver* pSpiDriver,
				 GpioOutput* pResetGpio,
				 GpioOutput* pDataCommandGpio,
				 GpioOutput* pBacklightGpio);
	~Nokia5110LCD(){};
	bool HwInit();
    void BackLightOn() { if(m_pBackLight) m_pBackLight->SetOutput();  }
    void BackLightOff(){ if(m_pBackLight) m_pBackLight->ClearOutput();}
	bool DisplayInit();
	void SetMode(u8 LCDMode);
	void Write(DC_t DC, unsigned char data);
	void GoToXY(int x, int y);
	void Clear();
	void DrawLine(unsigned char Row, unsigned char Col, const char* Str);
	void DrawString(const char *characters);
	void DrawBitmap(const char my_array[]);
	void LCDCharacter(const char character);
	void DrawChar(unsigned char Row, unsigned char Col, const char aChar);
	void DrawBuffer(char* pBuffer);

private:
	SpiDriver*  m_pDisplaySPI;
	GpioOutput* m_pDataCommandSelect;
	GpioOutput* m_pReset;
	GpioOutput* m_pBackLight;
};

}
#endif /* APP_INC_NOKIA5110LCD_HPP_ */
