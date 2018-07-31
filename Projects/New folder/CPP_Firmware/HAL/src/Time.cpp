

#include "Time.hpp"
#include "Test.h"

namespace Utility
{

void Time::Set(unsigned char hrs, unsigned char min, unsigned char sec, unsigned int millis)
{
    if(millis > 999)
    {
        millis = millis - 1000;
        Sec++;
    }
    if(sec > 59)
    {
        sec = sec - 60;
        min++;
    }

    if(min > 59)
    {
        min = min - 60;
        hrs++;
    }
    Millis = millis;
    Sec    = sec;
    Min    = min;
    Hrs    = hrs;
}

Time::Time(unsigned char hrs, unsigned char min, unsigned char sec, unsigned int millis)
{
    if(millis > 999)
    {
        millis = millis - 1000;
        Sec++;
    }
    if(sec > 59)
    {
        sec = sec - 60;
        min++;
    }

    if(min > 59)
    {
        min = min - 60;
        hrs++;
    }
    Millis = millis;
    Sec    = sec;
    Min    = min;
    Hrs    = hrs;
    m_Previous_Millis = GetRawMilliSecTicks();
}

// Overload + for Time.
Time Time::operator+(Time t)
{
    Time temp;
    temp.Sec = t.Sec + Sec;
    temp.Min = t.Min + Min;
    temp.Hrs = t.Hrs + Hrs;

    if(temp.Sec > 59)
    {
        temp.Sec = temp.Sec - 59;
        temp.Min++;
    }

    if(temp.Min > 59)
    {
        temp.Min = temp.Min - 59;
        Hrs++;
    }
    return temp;
}

void Time::AddMillis(unsigned int millis)
{
    Millis = Millis + millis;

    if(Millis > 999)
    {
        Millis = Millis - 1000;
        Sec++;
    }
    if(Sec > 59)
    {
        Sec = Sec - 60;
        Min++;
    }

    if(Min > 59)
    {
        Min = Min - 60;
        Hrs++;
    }

}


char* Time::Get(char* pBuffer)
{
    intToStr(Hrs, &pBuffer[0], 2);
    pBuffer[2] = ':';
    intToStr(Min, &pBuffer[3], 2);
    pBuffer[5] = ':';
    intToStr(Sec, &pBuffer[6], 2);
    pBuffer[8] = ':';
    intToStr(Millis, &pBuffer[9], 3);
    return pBuffer;
}


bool Time::Run()
{
    static unsigned int Previous_Millis = GetRawMilliSecTicks();
    unsigned int ElapsedTicks;

    ElapsedTicks = GetRawMiliSecTicksSince(Previous_Millis);

    if( ElapsedTicks >= 1)
    {
        Previous_Millis = GetRawMilliSecTicks();
        AddMillis(ElapsedTicks);
        return true;
    }
    return false;
}


















}