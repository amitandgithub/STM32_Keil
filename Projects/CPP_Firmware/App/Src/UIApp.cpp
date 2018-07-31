/*
 * UIApp.cpp
 *
 *  Created on: 25-Dec-2017
 *      Author: Amit Chaudhary
 */

#include "UIApp.hpp"

namespace App 
{


Bsp::HwButtonIntr HwButton_A2(GPIOA, GPIO_Pin_3,Bsp::GpioInput::EXTI_Trigger_Rising, Bsp::GpioInput::EXTI_Mode_Interrupt);

Nokia5110LCD NokiaLCD( SpiDriver::SPI_1_A7_A6_A5_A4,
		       GPIOB,GPIO_Pin_0,        // Reset Pin on B0
		       GPIOB,GPIO_Pin_1,	// Data/Command Pin on B1
		       GPIOB,GPIO_Pin_10	// Backlight Pin on B10
		     );
I2CDriver INA219_I2C(I2CDriver::I2C1_B6_B7, nullptr, I2CDriver::Master, I2CDriver::BaudRate_100000 ); // I2C1_B6_B7

INA219 INA219_Obj(&INA219_I2C,0x80);



UI MyUI(&NokiaLCD);
Screen::Event_t gEvent = Screen::MaxEvents;
char aPower[6][12];

uint8_t aCurrent[9];
uint8_t aVoltage[9];
float BusVoltage = 0.00;
static INA219::Power_t Power;
static Led Led_PC13(GPIOC,GPIO_Pin_13);
static ControlScreen MenuScreen;
static ControlScreen myControlScreen;
static Screen PowerMonitorScreen;
static Screen Aarav;
static Screen Tanya;
static UI::ScreenHandle_t MenuScreenHandle; 
static UI::ScreenHandle_t myControlScreenHandle; 
static UI::ScreenHandle_t AaravScreenHandle;
static UI::ScreenHandle_t TanyaScreenHandle;
 void CreateUI()
 {
    
    Aarav.AddText( (char *)
                       "Aarav       "
                       "Aarav       "
                       "Aarav       "
                       "Aarav       "
                       "Aarav       "
                       "Aarav       "
                       );
    
    MenuScreen.AddText( (char *)
                       " Menu1      "
                       " Menu2      "
                       " Menu3      "
                       " Menu4      "
                       " Menu5      "
                       " Next Menu  "
                       );    
    
    Tanya.AddText( (char *)
                       "Tanya       "
                       "Tanya       "
                       "Tanya       "
                       "Tanya       "
                       "Tanya       "
                       "Tanya       "
                       );
    
    PowerMonitorScreen.AddText( (char *)
                       "<Live Power>"
                       "            "
                       "            "
                       "            "
                       "            "
                       "            "
                       );
    
    myControlScreen.AddText( (char *)
                       ">Live Power "
                       " Aarav,Avni "
                       " Tanya,Dog  "
                       " Rudransh,  "
                       " Amit,sumit "
                       " Next Menu  "
                       );
   myControlScreen.AddHandler(0,Line0Menu0LongTouchHandler,Line0Menu0LongLongTouchHandler);
   myControlScreen.AddHandler(1,Line1AaravLongTouchHandler,Line0Menu0LongLongTouchHandler);
   myControlScreen.AddHandler(2,Line2TanyaLongTouchHandler,Line0Menu0LongLongTouchHandler);
   myControlScreen.AddHandler(5,Line5Menu0LongTouchHandler,nullptr);
   MenuScreen.AddHandler(5,Line5MenuScreenLongTouchHandler,nullptr);


  HwButton_A2.HwInit();
  INA219_I2C.HwInit();
  INA219_I2C.HwInit();
  
  INA219_Obj.SetCalibration_32V_2A();
  
  HwButton_A2.RegisterEventHandler(Bsp::HwButtonIntr::Click,static_cast<Bsp::HwButtonIntr::BtnHandler>(ClickEvent));
  HwButton_A2.RegisterEventHandler(Bsp::HwButtonIntr::LongPress,static_cast<Bsp::HwButtonIntr::BtnHandler>(LongPressEvent));
  HwButton_A2.RegisterEventHandler(Bsp::HwButtonIntr::LongLongPress,static_cast<Bsp::HwButtonIntr::BtnHandler>(LongLongPressEvent));
  
  MyUI.Init();
  MyUI.AddScreen(&PowerMonitorScreen);
  AaravScreenHandle = MyUI.AddScreen(&Aarav);
  TanyaScreenHandle = MyUI.AddScreen(&Tanya);
  myControlScreenHandle = MyUI.AddScreen(&myControlScreen); 
  MenuScreenHandle = MyUI.AddScreen(&MenuScreen);
  
 }
 void RunUI()
 {
    HwButton_A2.RunStateMachine();
    MyUI.EventHamdler(gEvent);
    MyUI.Run();
    INA219_Obj.Run(&Power);
    
    ftoa(Power.Voltage, (char*)aVoltage, 6);
    ftoa(Power.Current, (char*)aCurrent, 6);
    
    PowerMonitorScreen.SetText(1, 0, "V = ", 4);
    PowerMonitorScreen.SetText(1, 4,  (char*)aVoltage, 7);
    
    PowerMonitorScreen.SetText(2, 0, "I = ", 4);
    PowerMonitorScreen.SetText(2, 4,  (char*)aCurrent,7);
    
    ftoa(Power.Power, (char*)aCurrent, 6);
    PowerMonitorScreen.SetText(3, 0, "P = ", 4);
    PowerMonitorScreen.SetText(3, 4,  (char*)aCurrent, 7);
 }
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
void ftoa(float n, char *res, int afterpoint)
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
        fpart = fpart * pow(10.0, afterpoint);
 
        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}
void Led_PC13Toggle(void)
{
	//Led_PC13.Toggle();
}
void Led_PC13BlinkOnce(void)
{
	//Led_PC13.Toggle();
	//SysTickTimer::DelayTicks(100);
	//Led_PC13.Toggle();
}
void Led_PC13BlinkTwice(void)
{
	
  /*    Led_PC13.Toggle();
	SysTickTimer::DelayTicks(100);
	Led_PC13.Toggle();
	SysTickTimer::DelayTicks(100);
	Led_PC13.Toggle();
	SysTickTimer::DelayTicks(100);
	Led_PC13.Toggle();
*/

}
  
void ClickEvent(void)
{
	gEvent = Screen::Touch;
}
void LongPressEvent(void)
{
	gEvent = Screen::LongTouch;
}
void LongLongPressEvent(void)
{
	gEvent = Screen::LongLongTouch;
}

void Line0Menu0LongTouchHandler() 
{
  UI::SetActiveScreen(0);     
}

void Line1AaravLongTouchHandler() 
{
  UI::SetActiveScreen(AaravScreenHandle);     
}
void Line2TanyaLongTouchHandler() 
{
  UI::SetActiveScreen(TanyaScreenHandle);     
}
void Line5Menu0LongTouchHandler()
{
  UI::SetActiveScreen(MenuScreenHandle);     
}
void Line5MenuScreenLongTouchHandler()
{
  UI::SetActiveScreen(myControlScreenHandle);     
}
void Line0Menu0LongLongTouchHandler()
{
	Led_PC13.Toggle();
	SysTickTimer::DelayTicks(200);
	Led_PC13.Toggle();
}

  
  

} /* namespace App */
