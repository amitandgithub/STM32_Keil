

#include "Clock.hpp"

namespace Utility
{


unsigned char Clock::_GetSeconds()
{
   return m_p_1_Sec_Timer->getSec();
}

Clock::AlarmHandle_t Clock::CreateAlarm(Alarm_t nAlarm)
{
    // Search in the array of Alarms and return
    // index of any alarm which is not used
    // Else return zero, Indicating that no
    // available alarms available currently

    for(unsigned char i = 0; i < MAX_ALARMS; i++)
    {
        if( m_Alarms[i].State == ALARM_NOT_USED)
        {
            m_Alarms[i].State     = ALARM_IN_USE;
            m_Alarms[i].Type      = nAlarm.Type;
            m_Alarms[i].time      = nAlarm.time;
            m_Alarms[i].Handler   = nAlarm.Handler;
            return i;
        }
    }
   return 0;
}

Clock::AlarmHandle_t Clock::CreateAlarm(AlarmType_t AlarmType, Time time, AlarmHandler_t nAlarmHandler )
{
    // Search in the array of Alarms and return
    // index of any alarm which is not used
    // Else return zero, Indicating that no
    // available alarms available currently

    for(unsigned char i = 0; i < MAX_ALARMS; i++)
    {
        if( m_Alarms[i].State == ALARM_NOT_USED)
        {
            m_Alarms[i].State           = ALARM_IN_USE;
            m_Alarms[i].Type            = AlarmType;
            m_Alarms[i].time            = time;
            m_Alarms[i].ReloadTime      = time;
            m_Alarms[i].Handler         = nAlarmHandler;
            return i;
        }
    }
   return 0;
}

bool Clock::DeleteAlarm(AlarmHandle_t nAlarmHandle)
{
    if(nAlarmHandle < MAX_ALARMS )
    {
        m_Alarms[nAlarmHandle].State = ALARM_NOT_USED;
        return true;
    }
    return false;
}

bool Clock::StartAlarm(AlarmHandle_t nAlarmHandle)
{
    if(nAlarmHandle < MAX_ALARMS )
    {
        m_Alarms[nAlarmHandle].State = ALARM_ACTIVE;
        return true;
    }
    return false;
}

bool Clock::PauseAlarm(AlarmHandle_t nAlarmHandle)
{
    if(nAlarmHandle < MAX_ALARMS )
    {
        m_Alarms[nAlarmHandle].State = ALARM_PAUSED;
        return true;
    }
    return false;
}

bool Clock::ResemeAlarm(AlarmHandle_t nAlarmHandle)
{
    if(nAlarmHandle < MAX_ALARMS )
    {
        m_Alarms[nAlarmHandle].State = ALARM_ACTIVE;
        return true;
    }
    return false;
}

bool Clock::ReloadAlarmTime(AlarmHandle_t nAlarmHandle)
{
    if(nAlarmHandle < MAX_ALARMS )
    {
        m_Alarms[nAlarmHandle].ReloadTime = *(GetCurrentTime())+  m_Alarms[nAlarmHandle].time;
        m_Alarms[nAlarmHandle].State = ALARM_ACTIVE;
        return true;
    }
    return false;
}
void Clock::UpdateAlarms()
{
   for(unsigned char i = 0; i < MAX_ALARMS; i++)
    {
        if( m_Alarms[i].State == ALARM_ACTIVE)
        {

            if( (m_Alarms[i].ReloadTime.GetHrs() <=  m_CurrentTime.GetHrs()) &&
                (m_Alarms[i].ReloadTime.GetMin() <=  m_CurrentTime.GetMin()) &&
                (m_Alarms[i].ReloadTime.GetSec() <=  m_CurrentTime.GetSec()) )
            {
               m_Alarms[i].State =  ALARM_EXPIRED;
            }
        }
    }
}

void Clock::Run()
{
    unsigned char CurrentSeconds;
    static unsigned char Previous_Seconds = _GetSeconds();

    CurrentSeconds = _GetSeconds();
    if(Previous_Seconds != CurrentSeconds )
    {
        UpdateAlarms();
        Previous_Seconds = CurrentSeconds;
        return true;
    }

   if( m_CurrentTime.Run() == true )
   {
       // Time has been updated ,So update
       // the alarms too
        UpdateAlarms();
   }
}

void Clock::RunAlarmHandler()
{
    for(unsigned char i = 0; i < MAX_ALARMS; i++)
    {
        if( (m_Alarms[i].State == ALARM_EXPIRED) && (m_Alarms[i].Handler))
        {
            if(m_Alarms[i].Type == ALARM_TYPE_REPEAT)
            {
                ReloadAlarmTime(i);
            }
            else if(m_Alarms[i].Type == ALARM_TYPE_ONCE)
            {
                m_Alarms[i].State = ALARM_IN_USE;
            }

            m_Alarms[i].Handler();
        }
    }
}

}