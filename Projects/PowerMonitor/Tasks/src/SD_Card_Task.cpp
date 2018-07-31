/*
//
// File Name:  SD_Card_Task.cpp
//
// Author: Amit Chaudhary
//
// Date: 12 July 2018
//
*/
  
#include <stddef.h>
#include <stdio.h>
#include "SD_Card_Task.hpp"
#include "ff.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "SysTickTimer.hpp"
#include "INA219.hpp"

FRESULT checkMemoryCard();
FRESULT openAppend(FIL* fp, const char* path);
bool generateLogName(bool getNewFile);
bool printCsvHeader();
void initPins();
int SD_Test (void);

FRESULT r;
char filename[] = "Amt";
#define SD_ENABLE_PIN		GPIO_Pin_3
FATFS fs;
FIL fil;
FRESULT fr;
UINT nRead, nWritten;
char buff[64];
uint8_t buffP = 0;

char logName[12];
char timestamp[32];



SD_Card_Task::SD_Card_Task()
{

}

SD_Card_Task::SD_Card_Task(TaskBase::FREQ_t Frequency, TaskBase::ALLOWEDLOOPTIME_t AllowedLoopTime): TaskBase(Frequency, AllowedLoopTime)
{

}
void SD_Card_Task::Init()
{	
	//initPins();

	
}

extern Bsp::INA219::Power_t Power;
char Voltage[10];
char Current[10];
extern uint8_t ftoa(float n, char *res, int afterpoint);
void SD_Card_Task::Run()
{
	uint8_t str_size;
	
	fr = f_mount(&fs, "", 1);
	if (fr != FR_OK) 
	{
		//while(1);
	}
	/* Opens an existing file. If not exist, creates a new file. */
	fr = f_open(&fil, "PowerJuly28.txt", FA_WRITE | FA_OPEN_ALWAYS);
	//Bsp::SysTickTimer::DelayTicks(100);
	if (fr == FR_OK) 
	{
		/* Seek to end of the file to append data */
		fr = f_lseek(&fil, f_size(&fil));
		//Bsp::SysTickTimer::DelayTicks(100);
		if (fr != FR_OK) f_close(&fil);
		//f_puts("My Name is Amit Chaudhary\n", &fil);
		str_size = ftoa(Power.Voltage, (char*)Voltage, 4);
		str_size = ftoa(Power.Current, (char*)Current, 4);
		Voltage[9] = Current[9] = 0;
		f_printf(&fil,"Voltage = %s,Current = %s\n",Voltage,Current);//Power.Power);
		f_close(&fil);

		
		fr = f_mount(0, "", 0);
		if (fr != FR_OK) 
		{
			//while(1);
		}
	}
	else
	{
		//while(1);
	}

}


FRESULT openAppend(FIL* fp, /* [OUT] File object to create */
const char* path /* [IN]  File name to be opened */
) {
	/* Opens an existing file. If not exist, creates a new file. */
	fr = f_open(fp, path, FA_WRITE | FA_OPEN_ALWAYS);
	if (fr == FR_OK) {
		/* Seek to end of the file to append data */
		fr = f_lseek(fp, f_size(fp));
		if (fr != FR_OK)
			f_close(fp);
	}
	return fr;
}

FRESULT checkMemoryCard() {
	fr = f_mount(&fs, "", 1);
	if (fr != FR_OK) {
		return fr;
	}
	fr = openAppend(&fil, logName);
	if (fr != FR_OK) {
		return fr;
	}
	return FR_OK;
}

bool printCsvHeader() {
	//printCurrentDateTimeToString();
	fr = f_mount(&fs, "", 1);
	if (fr != FR_OK) {
		return false;
	}
	/* Open or create a log file ready to append */
	fr = openAppend(&fil, logName);
	if (fr == FR_OK) {
		/* Append a line */
		sprintf(buff, "Start date\t%s\n", timestamp);
		f_puts(buff, &fil);
		f_puts("No\tTimestamp\tTemp, C\n", &fil);
		f_close(&fil);
		return true;
	}
	return false;
}

bool generateLogName(bool getNewFile) {
	uint16_t logNumber = 1;
	sprintf(logName, "%08d.csv", logNumber);
	fr = f_mount(&fs, "", 1);
	if (fr != FR_OK) {
		return false;
	}
	do {
		fr = f_open(&fil, logName, FA_OPEN_EXISTING);
		if (fr == FR_NO_FILE) {
			if (!getNewFile) {
				logNumber--;
				sprintf(logName, "%08d.csv", logNumber);
			}
			return true;
		} else if (fr == FR_OK) {
			f_close(&fil);
			logNumber++;
			sprintf(logName, "%08d.csv", logNumber);
		}
	} while (fr == FR_OK);
	return false;
}
void initPins() 
{
	GPIO_InitTypeDef gpioInitStruct;
	// Disable all pins for power saving
	RCC_APB2PeriphClockCmd(
			RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC
					| RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE
					| RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG, ENABLE);

	gpioInitStruct.GPIO_Pin = GPIO_Pin_All;
	gpioInitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &gpioInitStruct);
	GPIO_Init(GPIOC, &gpioInitStruct);
	GPIO_Init(GPIOD, &gpioInitStruct);
	GPIO_Init(GPIOE, &gpioInitStruct);
	GPIO_Init(GPIOF, &gpioInitStruct);
	GPIO_Init(GPIOG, &gpioInitStruct);

	gpioInitStruct.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_Init(GPIOA, &gpioInitStruct);

	RCC_APB2PeriphClockCmd(
			RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD
					| RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF
					| RCC_APB2Periph_GPIOG, DISABLE);

	// PA3 - SD memory card power
	gpioInitStruct.GPIO_Pin = SD_ENABLE_PIN;
	gpioInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &gpioInitStruct);
	GPIO_WriteBit(GPIOA, SD_ENABLE_PIN, (BitAction)0);

}

int SD_Test (void)
{
    //FIL fil;        /* File object */
    char line[100]; /* Line buffer */
    FRESULT fr;     /* FatFs return code */


    /* Register work area to the default drive */
   // f_mount(&fil, "", 0);

    /* Open a text file */
    fr = f_open(&fil, "message.txt", FA_READ);
    if (fr) return (int)fr;

    /* Read all lines and display it */
    f_puts("My Name is Amit Chaudhary\n", &fil);

    /* Close the file */
    f_close(&fil);

    return 0;
}


