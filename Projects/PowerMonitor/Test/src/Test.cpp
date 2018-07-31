


#include"Test.h"

static GpioInput TestGpio_WithoutInterrupt(GPIOA,GPIO_Pin_2);
static Led Led_PC13(GPIOC,GPIO_Pin_13);
static Bsp::HwButtonIntr HwButton_A2(GPIOA, GPIO_Pin_3,Bsp::GpioInput::EXTI_Trigger_Rising, Bsp::GpioInput::EXTI_Mode_Interrupt);
static I2CDriver INA219_I2C(I2CDriver::I2C1_B6_B7, nullptr, I2CDriver::Master, I2CDriver::BaudRate_400000 ); // I2C1_B6_B7
static INA219 INA219_Obj(&INA219_I2C,0x80);
static INA219::Power_t Power;

static SpiDriver SpiDriverLCD(SpiDriver::SPI_1_A7_A6_A5_A4);
static GpioOutput DataCommandSelectGpio(GPIOB,GPIO_Pin_1);
static GpioOutput ResetPinGpio(GPIOB,GPIO_Pin_0);
static GpioOutput BackLightGpio(GPIOB,GPIO_Pin_10);
Nokia5110LCD NokiaLCD( &SpiDriverLCD, &ResetPinGpio, &DataCommandSelectGpio, &BackLightGpio );

UI MyUI(&NokiaLCD);
Screen::Event_t gEvent = Screen::MaxEvents;
static ControlScreen myControlScreen;
static Screen PowerMonitorScreen;

static UI::ScreenHandle_t myControlScreenHandle;

uint8_t aCurrent[20];
uint8_t aCurrent1[20];
uint8_t aVoltage[9];
//Clock nClock;
//Time aTime(0,0,0,1000);

static DS3231 rtc(&INA219_I2C,0xD0);
//static HwStopWatch nHwStopWatch(&rtc);


void Init_Tests()
{

  GpioInput TestGpio(GPIOA,GPIO_Pin_1, GpioInput_ISR,Bsp::GpioInput::EXTI_Trigger_Rising);

  TestGpio.HwInit();
  TestGpio_WithoutInterrupt.HwInit();
  HwButton_A2.HwInit();
  HwButton_A2.RegisterEventHandler(Bsp::HwButtonIntr::Click,static_cast<Bsp::HwButtonIntr::BtnHandler>(ClickEvent));
  HwButton_A2.RegisterEventHandler(Bsp::HwButtonIntr::LongPress,static_cast<Bsp::HwButtonIntr::BtnHandler>(LongPressEvent));
  HwButton_A2.RegisterEventHandler(Bsp::HwButtonIntr::LongLongPress,static_cast<Bsp::HwButtonIntr::BtnHandler>(LongLongPressEvent));
  INA219_I2C.HwInit();
  INA219_Obj.SetCalibration_32V_2A();
  MyUI.Init();
  CreateUI();
  Create_Alarm_Clock();
  rtc.begin();
  //rtc.setLocalTime();
  //rtc.setDOW(DS3231::SATURDAY);
  //rtc.setDate(20,1,2018);

}

void GpioInput_ISR()
{
  static int i = 0;
  i++;
}

void RunTests()
{

   // ButtonTest();
   // I2CDriver_Test();
   // INA219_Obj.Run(&Power);
   // Nokia_Display_Test();
    HwButton_A2.RunStateMachine();
    MyUI.EventHamdler(gEvent);
    MyUI.Run();
    INA219_Obj.Run(&Power);
/*
    PowerMonitorScreen.SetText(0, 0, rtc.getDOWStr(DS3231::FORMAT_SHORT), 3);
    PowerMonitorScreen.SetText(0, 3, ",", 1);
    PowerMonitorScreen.SetText(0, 4, rtc.getDateStr(DS3231::FORMAT_LONG, DS3231::FORMAT_LITTLEENDIAN,'-'), 2);
    PowerMonitorScreen.SetText(0, 6, "-", 1);
    PowerMonitorScreen.SetText(0, 7, rtc.getMonthStr(DS3231::FORMAT_SHORT), 3);
    PowerMonitorScreen.SetText(0, 10, "-", 1);
    PowerMonitorScreen.SetText(0, 11, &(rtc.getDateStr(DS3231::FORMAT_LONG, DS3231::FORMAT_LITTLEENDIAN,'-'))[8], 2);
    ftoa(rtc.getTemp(),(char*)aCurrent, 5);

    PowerMonitorScreen.SetText(1, 4, (char*)aCurrent , 5);
    PowerMonitorScreen.SetText(2, 4, rtc.getTimeStr(DS3231::FORMAT_LONG), 8);
*/
    ftoa(Power.Voltage, (char*)aVoltage, 6);
    ftoa(Power.Current, (char*)aCurrent, 6);

    PowerMonitorScreen.SetText(3, 0, "V = ", 4);
    PowerMonitorScreen.SetText(3, 4,  (char*)aVoltage, 7);

    PowerMonitorScreen.SetText(4, 0, "I = ", 4);
    PowerMonitorScreen.SetText(4, 4,  (char*)aCurrent,7);

    //ftoa(Power.Power, (char*)aCurrent, 6);
    //PowerMonitorScreen.SetText(5, 0, "P = ", 4);
    //PowerMonitorScreen.SetText(5, 4,  (char*)aCurrent, 7);

    //nClock.Run();
    //nClock.RunAlarmHandler();
    //PowerMonitorScreen.SetText(0, 4, nClock.GetCurrentTimeString((char*)aCurrent), 8);
    //MilliSecTimer.Run();
    //PowerMonitorScreen.SetText(5, 0, MilliSecTimer.Get((char*)aCurrent), 12);
    //t = rtc.getTime();
    //aTime.Run();
    //PowerMonitorScreen.SetText(5, 0,  aTime.Get((char*)aCurrent), 8);

    //PowerMonitorScreen.SetText(5, 0,  &(ctime(&mytime))[11], 14);
    //PowerMonitorScreen.SetText(5, 0,  loc_time. , 14);

   // nHwStopWatch.Run();
   // PowerMonitorScreen.SetText(5, 0,  nHwStopWatch.GetTimeStr((char*)aCurrent1), 8);
    //nClock.Run();
    //PowerMonitorScreen.SetText(4, 0, nClock.GetCurrentTimeString((char*)aCurrent), 8);

}

void GpioTest()
{
    static uint32_t Input_State = 0;

    if(TestGpio_WithoutInterrupt.ReadInput() == true)
    {
        Input_State = 1;
    }
    else
    {
        Input_State = 0;
    }
    Input_State = Input_State;
}


void LEDTest()
{
    Led_PC13.On();
    SysTickTimer::DelayTicks(1000);
    Led_PC13.Off();
    SysTickTimer::DelayTicks(1000);
}

void ButtonTest()
{
    HwButton_A2.RunStateMachine();
}

void ClickEvent1(void)
{
    Led_PC13.MultiBlink(1);
    NokiaLCD.DrawLine(1, 0, "Click");
}
void LongPressEvent1(void)
{
    Led_PC13.MultiBlink(2);
    NokiaLCD.DrawLine(1, 0, "Long Press");
}
void LongLongPressEvent1(void)
{
    Led_PC13.MultiBlink(3);
    NokiaLCD.DrawLine(1, 0, "Long Long Pres");
}
void I2CDriver_Test()
{
    static uint8_t I2C_Devices[10];

    INA219_I2C.ScanBus(I2C_Devices,10);
}

void Nokia_Display_Test()
{
    static const char Array[] = {'-','\\','|','/','-','\\','|','/'};
    static char index;
    HwButton_A2.RunStateMachine();
    NokiaLCD.DrawChar(0, 0, Array[index % sizeof(Array)]);
    NokiaLCD.DrawChar(0, 2, Array[index % sizeof(Array)]);
    NokiaLCD.DrawChar(0, 4, Array[index % sizeof(Array)]);
    NokiaLCD.DrawChar(0, 6, Array[index % sizeof(Array)]);
    NokiaLCD.DrawChar(0, 8, Array[index % sizeof(Array)]);
    NokiaLCD.DrawChar(0, 10,Array[index % sizeof(Array)]);
    NokiaLCD.DrawChar(0, 12,Array[index++ % sizeof(Array)]);
    SysTickTimer::DelayTicks(150);
}
///static Clock::AlarmHandle_t nClockHandle;
void OneSecondHandler()
{
    Led_PC13.Toggle();
}
void Create_Alarm_Clock()
{
    //static Time time(0,0,5);
    //nClockHandle = nClock.CreateAlarm( Clock::ALARM_TYPE_REPEAT, time, OneSecondHandler );
    //nClock.StartAlarm(nClockHandle);
}



void CreateUI()
{

    PowerMonitorScreen.AddText( (char *)
                       "D =           "
                       "T =      'C   "
                       "T =           "
                       "              "
                       "              "
                       "              "
                       );

    myControlScreen.AddText( (char *)
                       ">Live Power   "
                       " Back Lite    "
                       " Reset Data   "
                       "              "
                       "              "
                       "              "
                       );
   myControlScreen.AddHandler(0,Line0Menu0LongTouchHandler,Line0Menu0LongLongTouchHandler);
   myControlScreen.AddHandler(1,LongTouchBackLightHandler,nullptr);

   MyUI.AddScreen(&PowerMonitorScreen);
   myControlScreenHandle = MyUI.AddScreen(&myControlScreen);
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
void LongTouchBackLightHandler()
{
    BackLightGpio.ToggleOutput();

    if(BackLightGpio.ReadOutputValue())
    {
        myControlScreen.SetText(myControlScreen.GetActiveLine(),11,"ON ", 3);
    }
    else
    {
        myControlScreen.SetText(myControlScreen.GetActiveLine(),11,"OFF", 3);
    }
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