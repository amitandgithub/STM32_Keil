//******************
// CLASS: HwStopWatch
//
// DESCRIPTION:
//  This class implements the HwStopWatch witn Milliseconds support
//
// CREATED: 21-01-2018, by Amit Chaudhary
//
// FILE: $FILE_FNAME$
//
#ifndef HWSTOPWATCH_HPP_
#define HWSTOPWATCH_HPP_

#include "SysTickTimer.hpp"
#include "DS3231.h"

namespace Utility
{

class HwStopWatch
{
public:
    HwStopWatch(DS3231* pDS3231):m_pDS3231(pDS3231),m_Previous_Seconds(0){};
    ~HwStopWatch(){};
    char* GetTimeStr(char* pBuffer);
    void AddSec(unsigned char sec);
    bool Run();
private:
    unsigned char    Sec;
    unsigned char    Min;
    unsigned char    Hrs;
    unsigned char    m_Previous_Seconds;
    unsigned char    _GetSeconds();
    DS3231*          m_pDS3231;
};

}
#endif // HWSTOPWATCH_HPP_