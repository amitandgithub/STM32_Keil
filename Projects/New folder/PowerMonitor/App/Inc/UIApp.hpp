/*
 * UIApp.hpp
 *
 *  Created on: 25-Dec-2017
 *      Author: Amit Chaudhary
 */

#ifndef APP_UIAPP_HPP_
#define APP_UIAPP_HPP_

#include<stddef.h>
#include<stdio.h>
#include<math.h>
#include"stm32f10x.h"
#include"Led.hpp"
#include"GpioOutput.hpp"
#include"GpioInput.hpp"
#include"HwButton.hpp"
#include"HwButtonIntr.hpp"
#include"SysTickTimer.hpp"
#include"SpiDriver.hpp"
#include"Nokia5110LCD.hpp"
#include"UI.hpp"
#include"Line.hpp"
#include"Menu.hpp"
#include"I2CDriver.hpp"
#include"INA219.hpp"
#include"I2c.hpp"
#include"Screen.hpp"
#include"ControlScreen.hpp"
//#include"OLED_I2C.h"

namespace App 
{
  
void CreateUI();
void RunUI();
void reverse(char *str, int len);
int intToStr(int x, char str[], int d);
void ftoa(float n, char *res, int afterpoint);

void ClickEvent(void);
void LongPressEvent(void);
void LongLongPressEvent(void);

void Led_PC13BlinkTwice(void);
void Led_PC13BlinkOnce(void);
void Led_PC13Toggle(void);
void Line0Menu0LongLongTouchHandler();
void Line0Menu0LongTouchHandler();
void Line5Menu0LongTouchHandler();
void Line5MenuScreenLongTouchHandler();
void Line2TanyaLongTouchHandler();
void Line1AaravLongTouchHandler();
} /* namespace App */

#endif /* APP_UIAPP_HPP_ */