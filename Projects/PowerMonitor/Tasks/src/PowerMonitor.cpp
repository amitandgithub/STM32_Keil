/*
//
// File Name:  PowerMonitor_task.cpp
//
// Author: Amit Chaudhary
//
// Date: 16 June 2018
//
*/
  

#include "PowerMonitor_task.hpp"
#include "I2CDriver.hpp"
#include "INA219.hpp"

#define SAMPLES 1000UL
using namespace Bsp;

static I2CDriver INA219_I2C(I2CDriver::I2C1_B6_B7, nullptr, I2CDriver::Master, I2CDriver::BaudRate_400000 ); // I2C1_B6_B7
static INA219 INA219_Obj(&INA219_I2C,0x80);

INA219::Power_t Power;
float mAH,CurrentSamples;
uint32_t samples = 0;

PowerMonitor_task::PowerMonitor_task()
{
	
}

PowerMonitor_task::PowerMonitor_task(TaskBase::FREQ_t Frequency, TaskBase::ALLOWEDLOOPTIME_t AllowedLoopTime): TaskBase(Frequency, AllowedLoopTime)
{
	
}
void PowerMonitor_task::Init()
{	
	INA219_I2C.HwInit();
	INA219_Obj.SetCalibration_32V_2A();//SetCalibration_32V_2A,SetCalibration_16V_400mA
}

void PowerMonitor_task::Run()
{
	INA219_Obj.Run(&Power);
	
	mAH +=  Power.Current;
	CurrentSamples = CurrentSamples + Power.Current;
	
	samples++;
	
	if(samples == SAMPLES)
	{
		mAH = mAH/SAMPLES;	
		CurrentSamples = CurrentSamples/SAMPLES;
		
		Power.mAH += mAH/(3600.0*1);
		//Power.Current = CurrentSamples;
	}

}

	
