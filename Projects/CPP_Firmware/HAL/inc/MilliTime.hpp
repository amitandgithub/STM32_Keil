//******************
// CLASS: Time
//
// DESCRIPTION:
//  This class implements the Time witn Milliseconds support
//
// CREATED: 19-01-2018, by Amit Chaudhary
//
// FILE: $FILE_FNAME$
//
#ifndef TIME_HPP_
#define TIME_HPP_

#include "SysTickTimer.hpp"

namespace Utility
{

class Time
{
public:
    Time():Sec(0),Min(0),Hrs(0){};
    Time(unsigned char hrs, unsigned char min, unsigned char sec, unsigned int millis);
    ~Time(){};
    char* Get(char* pBuffer);
    unsigned int GetMillis(){return Millis;}
    unsigned char GetSec(){return Sec;}
    unsigned char GetMin(){return Min;}
    unsigned char GetHrs(){return Hrs;}
    void AddMillis(unsigned int millis);
    void SetSec(unsigned char sec){Sec = sec;}
    void SetMin(unsigned char min){Min = min;}
    void SetHrs(unsigned char hrs){Hrs = hrs;}
    void SetMillis(unsigned int millis){Millis = millis;}
    void Set(unsigned char hrs, unsigned char min, unsigned char sec, unsigned int millis);
    bool Run();
private:
    unsigned int GetRawMilliSecTicks(){return Bsp::SysTickTimer::GetTicks();}
    unsigned int GetRawMiliSecTicksSince(unsigned int LastTicks){return Bsp::SysTickTimer::GetTicksSince(LastTicks);}

    unsigned int    m_Previous_Millis;
    unsigned int    Millis;
    unsigned char   Sec;
    unsigned char   Min;
    unsigned char   Hrs;

};

}
#endif // TIME_HPP_