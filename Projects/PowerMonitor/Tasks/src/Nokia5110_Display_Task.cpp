/*
//
// File Name:  Nokia5110_Display_task.cpp
//
// Author: Amit Chaudhary
//
// Date: 25 June 2018
//
*/
  

#include "Nokia5110_Display_task.hpp"
#include "SpiDriver.hpp"
#include "Nokia5110LCD.hpp"
#include <string>     // std::string, std::to_string
#include <math.h>
#include "INA219.hpp"

uint8_t ftoa(float n, char *res, int afterpoint);
int intToStr(int x, char str[], int d);
void reverse(char *str, int len);
int power(int x, unsigned int y);
float power(float x, int y);

static SpiDriver SpiDriverLCD           (SpiDriver::SPI_1_A7_A6_A5_A4);
static GpioOutput DataCommandSelectGpio (GPIOB,GPIO_Pin_1);
static GpioOutput ResetPinGpio          (GPIOB,GPIO_Pin_0);
static GpioOutput BackLightGpio         (GPIOB,GPIO_Pin_10);
static App::Nokia5110LCD NokiaLCD       ( &SpiDriverLCD, &ResetPinGpio, &DataCommandSelectGpio, &BackLightGpio );

Nokia5110_Display_task::Nokia5110_Display_task()
{
	
}

Nokia5110_Display_task::Nokia5110_Display_task(TaskBase::FREQ_t Frequency, TaskBase::ALLOWEDLOOPTIME_t AllowedLoopTime): TaskBase(Frequency, AllowedLoopTime)
{
	
}
void Nokia5110_Display_task::Init()
{	
	NokiaLCD.HwInit();
	NokiaLCD.BackLightOn();
	NokiaLCD.DrawLine(0,0,"Amit Chaudhary");
}



using namespace Bsp;
extern INA219::Power_t Power;
void Nokia5110_Display_task::Run()
{
	extern uint32_t samples;
	char Buf[10];
	uint8_t str_size;

	str_size = ftoa(Power.Voltage, (char*)Buf, 4);
	Buf[str_size + 1] = 'V';
	Buf[str_size + 2] = '\0';
	NokiaLCD.DrawLine(1,0,Buf);

	//ftoa(Power.Power, (char*)Buf, 4);
	//NokiaLCD.DrawLine(3,0,Buf);

	if(samples >= 1000)
	{
		samples = 0;
		str_size = ftoa(Power.Current, (char*)Buf, 4);
		Buf[str_size + 1] = 'm';
		Buf[str_size + 2] = 'A';
		Buf[str_size + 3] = '\0';
		NokiaLCD.DrawLine(2,0,Buf);
		
		str_size = ftoa(Power.mAH, (char*)Buf, 4);
		Buf[str_size + 1] = 'm';
		Buf[str_size + 2] = 'A';
		Buf[str_size + 3] = 'H';
		Buf[str_size + 4] = '\0';
		NokiaLCD.DrawLine(3,0,Buf);
		
	}
	
}


#if 1
// reverses a string 'str' of length 'len'

void reverse(char *str, int len)
{
    int i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; j--;
    }
}

 // Converts a given integer x to string str[].  d is the number
 // of digits required in output. If d is more than the number
 // of digits in x, then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }

    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';

    reverse(str, i);
    str[i] = '\0';
    return i;
}
// Converts a floating point number to string.
uint8_t ftoa(float n, char *res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;

    // Extract floating part
    float fpart = n - (float)ipart;

    // convert integer part to string
    int i = intToStr(ipart, res, 0);

    // check for display option after point
    if (afterpoint != 0)
    {
        res[i] = '.';  // add dot

        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter is needed
        // to handle cases like 233.007
        fpart = fpart * power(10.0, afterpoint);

        intToStr((int)fpart, res + i + 1, afterpoint);
    }
	return i + afterpoint;
}

/* Function to calculate x raised to the power y */
int power(int x, unsigned int y)
{
    if (y == 0)
        return 1;
    else if (y%2 == 0)
        return power(x, y/2)*power(x, y/2);
    else
        return x*power(x, y/2)*power(x, y/2);
}

/* Extended version of power function that can work
 for float x and negative y*/
 
float power(float x, int y)
{
    float temp;
    if( y == 0)
       return 1;
    temp = power(x, y/2);       
    if (y%2 == 0)
        return temp*temp;
    else
    {
        if(y > 0)
            return x*temp*temp;
        else
            return (temp*temp)/x;
    }
} 
#endif
