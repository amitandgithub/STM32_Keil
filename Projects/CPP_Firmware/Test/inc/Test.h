//******************
// CLASS: GpioInputTest
//
// DESCRIPTION:
//  This class is to test the GPIO Input Functionality
//
// CREATED: 13-01-2018, by Amit Chaudhary
//
// FILE: $FILE_FNAME$
//

#ifndef APP_INC_TEST_HPP_
#define APP_INC_TEST_HPP_

#include<stddef.h>
#include<stdio.h>
#include<math.h>
#include <time.h>
#include <stdio.h>
#include "GpioInput.hpp"
#include "Led.hpp"
#include "SysTickTimer.hpp"
#include "HwButton.hpp"
#include "HwButtonIntr.hpp"
#include "I2CDriver.hpp"
#include "INA219.hpp"
#include "SpiDriver.hpp"
#include "Nokia5110LCD.hpp"
#include "Screen.hpp"
#include "ControlScreen.hpp"
#include "UI.hpp"
#include "Clock.hpp"
#include "MilliTime.hpp"
#include <DS3231.h>
#include "HwStopWatch.hpp"

using namespace Bsp;
using namespace App;
using namespace Utility;

void Init_Tests();
void RunTests();
void GpioInput_ISR();
void LEDTest();
void GpioTest();
void ButtonTest();
void ClickEvent(void);
void LongPressEvent(void);
void LongLongPressEvent(void);
void ClickEvent1(void);
void LongPressEvent1(void);
void LongLongPressEvent1(void);
void I2CDriver_Test();
void Nokia_Display_Test();
void CreateUI();
void Line0Menu0LongLongTouchHandler();
void Line0Menu0LongTouchHandler();
void Line5Menu0LongTouchHandler();
void Line5MenuScreenLongTouchHandler();
void Line2TanyaLongTouchHandler();
void Line1AaravLongTouchHandler();
void LongTouchBackLightHandler();
void Create_Alarm_Clock();
void OneSecondHandler();





void reverse(char *str, int len);
extern int intToStr(int x, char str[], int d);
extern void ftoa(float n, char *res, int afterpoint);

#endif