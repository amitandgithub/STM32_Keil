

#include "HwStopWatch.hpp"
#include "Test.h"

namespace Utility
{

unsigned char HwStopWatch::_GetSeconds()
{
   return m_pDS3231->getSec();
}

char* HwStopWatch::GetTimeStr(char* pBuffer)
{
    intToStr(Hrs, &pBuffer[0], 2);
    pBuffer[2] = ':';
    intToStr(Min, &pBuffer[3], 2);
    pBuffer[5] = ':';
    intToStr(Sec, &pBuffer[6], 2);
    return pBuffer;
}

void HwStopWatch::AddSec(unsigned char sec)
{
    Sec = Sec + sec;

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

bool HwStopWatch::Run()
{
#if 0
    static unsigned char ElapsedSeconds, CurrentSeconds;

    CurrentSeconds = _GetSeconds();

    ElapsedSeconds = (CurrentSeconds - m_Previous_Seconds) >= 0 ? CurrentSeconds - m_Previous_Seconds : CurrentSeconds - m_Previous_Seconds + 60;

    if( ElapsedSeconds >= 1)
    {
        AddSec(ElapsedSeconds);
        m_Previous_Seconds = CurrentSeconds;
        return true;
    }
#else
    unsigned char CurrentSeconds;
    static unsigned char Previous_Seconds = _GetSeconds();

    CurrentSeconds = _GetSeconds();
    if(Previous_Seconds != CurrentSeconds )
    {
        AddSec(1);
        Previous_Seconds = CurrentSeconds;
        return true;
    }

#endif
    return false;
}


















}