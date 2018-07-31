//******************
// CLASS: Clock
//
// DESCRIPTION:
//  This class is responsible for prividing the Clock functionality.
//
// CREATED: 17-01-2018, by Amit Chaudhary
//
// FILE: $FILE_FNAME$
//
#ifndef CLOCK_HPP_
#define CLOCK_HPP_

#include "Time.hpp"
#include "SysTickTimer.hpp"
#include "Test.h"
#include "DS3231.h"

namespace Utility
{

class Clock
{
public:
    static const unsigned char MAX_ALARMS = 10;
    typedef void (*AlarmHandler_t)();
    typedef unsigned int AlarmHandle_t;
    typedef enum
    {
        ALARM_NOT_USED = 0,
        ALARM_IN_USE,
        ALARM_ACTIVE,
        ALARM_PAUSED,
        ALARM_EXPIRED,
        ALARM_INVALID
    }AlarmState_t;

    typedef enum
    {
        ALARM_TYPE_ONCE,
        ALARM_TYPE_REPEAT,
        ALARM_TYPE_INVALID
    }AlarmType_t;
    typedef struct
    {
        AlarmType_t    Type;
        Time           time;
        Time           ReloadTime;
        AlarmHandler_t Handler;
        AlarmState_t   State;
    }Alarm_t;

    Clock(DS3231* p_1_Sec_Timer ):m_p_1_Sec_Timer(p_1_Sec_Timer){};
    ~Clock(){};
    Time* GetCurrentTime(){return m_CurrentTime.GetTime();}
    char * GetCurrentTimeString(char* pBuffer){return m_CurrentTime.Get(pBuffer);}
    unsigned char GetCurrentSec(){return m_CurrentTime.GetSec();}
    unsigned char GetCurrentMin(){return m_CurrentTime.GetMin();}
    unsigned char GetCurrentHrs(){return m_CurrentTime.GetHrs();}
    void SetCurrentTime(unsigned char hrs, unsigned char min, unsigned char sec){m_CurrentTime.Set(hrs,min,sec);}

    AlarmHandle_t CreateAlarm( Alarm_t nAlarm);
    AlarmHandle_t CreateAlarm(AlarmType_t AlarmType, Time time, AlarmHandler_t nAlarmHandler );
    bool StartAlarm (AlarmHandle_t nAlarmHandle);
    bool PauseAlarm (AlarmHandle_t nAlarmHandle);
    bool ResemeAlarm(AlarmHandle_t nAlarmHandle);
    bool DeleteAlarm(AlarmHandle_t nAlarmHandle);

    void Run();
    void RunAlarmHandler();

private:
    unsigned int GetRawMiliSecTicks(){return Bsp::SysTickTimer::GetTicks();}
    unsigned int GetRawMiliSecTicksSince(unsigned int LastTicks){return Bsp::SysTickTimer::GetTicksSince(LastTicks);}
    bool ReloadAlarmTime(AlarmHandle_t nAlarmHandle);
    void UpdateAlarms();
    unsigned char    _GetSeconds();
    DS3231*          m_p_1_Sec_Timer;
    Time             m_CurrentTime;
    unsigned int     m_Previous_Millis;
    unsigned char    m_CurrentAlarms;
    Alarm_t          m_Alarms[MAX_ALARMS];

};

}



#endif /* CLOCK_HPP_ */