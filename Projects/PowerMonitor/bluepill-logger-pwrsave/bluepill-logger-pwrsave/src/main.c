/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "utils.h"
#include "ff.h"
#include "ds18b20.h"
#include <stdio.h> // printf
#include <stdlib.h> // strtoul
#include <string.h> // memset
#include <inttypes.h> // uint16_t and uin32_t formatters

/* Defines -------------------------------------------------------------------*/
#define TIMEOUT				5 // Time in seconds given to receive time/delay from UART
#define DEFAULT_DELAY       60 // One minute if not specified by user

#define MODE_PIN			GPIO_Pin_0
#define TEMP_ENABLE_PIN		GPIO_Pin_1
#define TEMP_PIN			GPIO_Pin_2
#define SD_ENABLE_PIN		GPIO_Pin_3
#define LED_PIN				GPIO_Pin_8
#define TX_PIN				GPIO_Pin_9
#define RX_PIN				GPIO_Pin_10

/* Global variables ----------------------------------------------------------*/
bool wokeUp = false;
bool wasInConfMode = false;

FATFS fs;
FIL fil;
FRESULT fr;
UINT nRead, nWritten;

char buff[64];
uint8_t buffP = 0;

char logName[12];
char timestamp[32];

GPIO_InitTypeDef gpioInitStruct;
USART_InitTypeDef usartInitStruct;

enum state {
	STATE_CONF_WAIT,		// Waiting for a command: set/get time/delay
	STATE_CONF_GOT_COMMAND, // Got the command, executing it
	STATE_LOG 				// Logging temperature
} cState = STATE_LOG;
enum command {
	COMM_SET_TIME, COMM_SET_DELAY, COMM_GET_TIME, COMM_GET_DELAY
} cCommand;

uint16_t logDelay = DEFAULT_DELAY;
uint16_t entry = 0;

/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
 set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

bool blinkLedAndWaitModeChange(uint16_t delayMillis);
void initPins();
void initRtc();
void initUart1(void);

FRESULT checkMemoryCard();
FRESULT openAppend(FIL* fp, const char* path);
void printCurrentDateTimeToString();
bool generateLogName(bool takeLast);
bool printCsvHeader();
bool bufferContainsCommand();

/* Functions -----------------------------------------------------------------*/
bool blinkLedAndWaitModeChange(uint16_t delayMillis) {
	GPIO_WriteBit(GPIOA, LED_PIN, 1);
	uint16_t i = delayMillis;
	while (i--) {
		delay_us(TIM2, 1000);
		if (GPIO_ReadInputDataBit(GPIOA, MODE_PIN)) {
			return true;
		}
	}
	GPIO_WriteBit(GPIOA, LED_PIN, 0);
	i = delayMillis;
	while (i--) {
		delay_us(TIM2, 1000);
		if (GPIO_ReadInputDataBit(GPIOA, MODE_PIN)) {
			return true;
		}
	}
	return false;
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

void printCurrentDateTimeToString() {
	uint32_t seconds, minutes, hours, days, year, month;
	seconds = RTC_GetCounter();
	/* calculate minutes */
	minutes = seconds / 60;
	seconds -= minutes * 60;
	/* calculate hours */
	hours = minutes / 60;
	minutes -= hours * 60;
	/* calculate days */
	days = hours / 24;
	hours -= days * 24;
	/* Unix time starts in 1970 */
	year = 1970;
	while (1) {
		bool leapYear = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
		uint16_t daysInYear = leapYear ? 366 : 365;
		if (days >= daysInYear) {
			days -= daysInYear;
			++year;
		} else {
			/* calculate the month and day */
			static const uint8_t daysInMonth[12] = { 31, 28, 31, 30, 31, 30, 31,
					31, 30, 31, 30, 31 };
			for (month = 0; month < 12; ++month) {
				uint8_t dim = daysInMonth[month];
				/* add a day to february if this is a leap year */
				if (month == 1 && leapYear)
					++dim;
				if (days >= dim)
					days -= dim;
				else
					break;
			}
			break;
		}
	}
	//strftime hogs up about 10kb of flash space, hence this:
	sprintf(timestamp, "%d-%02d-%02d, %02d:%02d:%02d", (int) year, (int) month,
			(int) days + 1, (int) hours, (int) minutes, (int) seconds);
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

bool printCsvHeader() {
	printCurrentDateTimeToString();
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

void initPins() {
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

	// Configuration of port A
	// PA0 - mode switch
	gpioInitStruct.GPIO_Pin = MODE_PIN;
	gpioInitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	gpioInitStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &gpioInitStruct);
	if (GPIO_ReadInputDataBit(GPIOA, MODE_PIN)) {
		cState = STATE_CONF_WAIT;
	}

	// PA1 - dS18B20 power
	gpioInitStruct.GPIO_Pin = TEMP_ENABLE_PIN;
	gpioInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &gpioInitStruct);
	GPIO_WriteBit(GPIOA, TEMP_ENABLE_PIN, 0);

	/* PA2 - DS18B20 signal set it to AIN
	 * to conserve power in config mode. */
	if (cState == STATE_CONF_WAIT) {
		gpioInitStruct.GPIO_Pin = TEMP_PIN;
		gpioInitStruct.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_Init(GPIOA, &gpioInitStruct);
	}

	// PA3 - SD memory card power
	gpioInitStruct.GPIO_Pin = SD_ENABLE_PIN;
	gpioInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &gpioInitStruct);
	GPIO_WriteBit(GPIOA, SD_ENABLE_PIN, 0);

	// PA8 - LED
	gpioInitStruct.GPIO_Pin = LED_PIN;
	gpioInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &gpioInitStruct);
	GPIO_WriteBit(GPIOA, LED_PIN, 0);

	/* PA9 and PA10 - UART TX -set them to AIN
	 * to conserve power while logging. */
	if (cState == STATE_LOG) {
		gpioInitStruct.GPIO_Pin = TX_PIN | RX_PIN;
		gpioInitStruct.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_Init(GPIOA, &gpioInitStruct);
	}
}

void initUart1(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE);

	// PA9 - UART TX
	gpioInitStruct.GPIO_Pin = TX_PIN;
	gpioInitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpioInitStruct);

	// PA10 - UART RX
	gpioInitStruct.GPIO_Pin = RX_PIN;
	gpioInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpioInitStruct);

	USART_Cmd(USART1, ENABLE);

	/* Baud rate 9600, 8-bit data, One stop bit
	 * No parity, Do both Rx and Tx, No HW flow control	 */
	usartInitStruct.USART_BaudRate = 9600;
	usartInitStruct.USART_WordLength = USART_WordLength_8b;
	usartInitStruct.USART_StopBits = USART_StopBits_1;
	usartInitStruct.USART_Parity = USART_Parity_No;
	usartInitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usartInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1, &usartInitStruct);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	NVIC_EnableIRQ (USART1_IRQn);
}

void initRtc() {
	if (wokeUp) {
		/* No need to configure the RTC as the RTC configuration(clock source, enable,
		 prescaler,...) is kept after wake-up from STANDBY */
		RTC_WaitForSynchro();
	} else {
		/* Enable LSE */
		RCC_LSEConfig (RCC_LSE_ON);
		/* Wait till LSE is ready */
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
			;
		/* Select the RTC Clock Source */
		RCC_RTCCLKConfig (RCC_RTCCLKSource_LSE);
		/* Enable the RTC Clock */
		RCC_RTCCLKCmd (ENABLE);
		/* Wait for RTC APB registers synchronisation */
		RTC_WaitForSynchro();
		/* Set the RTC time base to 1s */
		RTC_SetPrescaler(32767);
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
//		RTC_ITConfig(RTC_IT_ALR, ENABLE);
//		RTC_WaitForLastTask();
	}
}

/* Main ----------------------------------------------------------------------*/
int main(void) {
	// Determine if uC has just woke up from standby mode
	if (PWR_GetFlagStatus(PWR_FLAG_SB) == SET) {
		PWR_ClearFlag (PWR_FLAG_SB);
		RTC_SetAlarm(0);
		wokeUp = true;
	}

	/* This code may be executed when uC
	 * has just been powered up or woke up
	 * after a set period of time waiting to measure temperature.
	 * This is common configuration for both cases: */
	START: initPins();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd (ENABLE);
	initRtc();

	uint16_t bkp1 = BKP_ReadBackupRegister(BKP_DR1);
	if (bkp1 > DEFAULT_DELAY) {
		logDelay = bkp1;
	} else {
		BKP_WriteBackupRegister(BKP_DR1, DEFAULT_DELAY);
	}

	/* From microcontroller's point of view,
	 * it's continuously in config mode
	 * (provided the switch is flipped), and only
	 * runs logging mode once because putting
	 * it to standby mode resets it.
	 */
	if (cState == STATE_CONF_WAIT) {
		initUart1();
		buffP = 0;
		wasInConfMode = true;
	}
	while (GPIO_ReadInputDataBit(GPIOA, MODE_PIN)) {
		if (cState == STATE_CONF_WAIT) {
			uint32_t ts = RTC_GetCounter();
			GPIO_WriteBit(GPIOA, LED_PIN, ts % 5 == 0);
		} else if (cState == STATE_CONF_GOT_COMMAND) {
			GPIOA->ODR ^= LED_PIN;
			if (cCommand == COMM_GET_TIME) {
				printf("t%" PRIu32 "t\r\n", RTC_GetCounter());
				buff[0] = 0;
				buff[1] = 0;
				buffP = 0;
				cState = STATE_CONF_WAIT;
				continue;
			}
			if (cCommand == COMM_GET_DELAY) {
				printf("d%" PRIu16 "d\r\n", logDelay);
				buff[0] = 0;
				buff[1] = 0;
				buffP = 0;
				cState = STATE_CONF_WAIT;
				continue;
			}
			if (cCommand == COMM_SET_TIME) {
				char* pos1 = &buff[0];
				char* pos2;
				uint32_t startTime = RTC_GetCounter();
				while (RTC_GetCounter() - startTime < TIMEOUT) {
					pos2 = NULL;
					for (uint8_t i = 0; i < sizeof(buff); i++) {
						if (buff[i] == 't' && buff[i - 1] == 's') {
							pos2 = &buff[i];
							break;
						}
					}
					if (pos2) {
						uint32_t newTime = strtoul(pos1, &pos2, 10);
						RTC_SetCounter(newTime);
						memset(buff, 0, sizeof buff);
						cState = STATE_CONF_WAIT;
						break;
					}
				}
				if (cState != STATE_CONF_WAIT) {
					buff[0] = 0;
					buff[1] = 0;
					cState = STATE_CONF_WAIT;
				}
				buffP = 0;
				continue;
			}
			if (cCommand == COMM_SET_DELAY) {
				char* pos1 = &buff[0];
				char* pos2;
				uint32_t startTime = RTC_GetCounter();
				while (RTC_GetCounter() - startTime < TIMEOUT) {
					pos2 = NULL;
					for (uint8_t i = 0; i < sizeof(buff); i++) {
						if (buff[i] == 'd' && buff[i - 1] == 's') {
							pos2 = &buff[i];
							break;
						}
					}
					if (pos2) {
						logDelay = strtol(pos1, &pos2, 10);
						memset(buff, 0, sizeof buff);
						cState = STATE_CONF_WAIT;
						BKP_WriteBackupRegister(BKP_DR1, logDelay);
						break;
					}
				}
				if (cState != STATE_CONF_WAIT) {
					buff[0] = 0;
					buff[1] = 0;
					cState = STATE_CONF_WAIT;
				}
				buffP = 0;
				continue;
			}
			GPIOA->ODR ^= LED_PIN;
		}
	}
	USART_Cmd(USART1, DISABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO,
			DISABLE);
	GPIO_WriteBit(GPIOA, LED_PIN, 1);
	GPIO_WriteBit(GPIOA, TEMP_ENABLE_PIN, 1);
	GPIO_WriteBit(GPIOA, SD_ENABLE_PIN, 1);
	setup_delay_timer (TIM2);
	// TPS22960's 75uS rise time + some more just to be sure
	delay_us(TIM2, 100);
	ds18b20_init(GPIOA, TEMP_PIN, TIM2);
	bool canLog = false;
	while (!canLog) {
		simple_float temperature;
		do {
			temperature = ds18b20_get_temperature_simple();
			if (temperature.is_valid) {
				break;
			}
			if (blinkLedAndWaitModeChange(100)) {
				cState = STATE_CONF_WAIT;
				goto START;
			}
		} while (!temperature.is_valid);

		printCurrentDateTimeToString();
		bool getNewFile = !wokeUp || wasInConfMode;
		if (generateLogName(getNewFile) == false) {
			while (checkMemoryCard() != FR_OK) {
				if (blinkLedAndWaitModeChange(1000)) {
					cState = STATE_CONF_WAIT;
					goto START;
				}
			}
			continue;
		}
		if (getNewFile && (printCsvHeader() == false)) {
			while (checkMemoryCard() != FR_OK) {
				if (blinkLedAndWaitModeChange(1000)) {
					cState = STATE_CONF_WAIT;
					goto START;
				}
			}
			continue;
		}
		entry++;
		sprintf(buff, "%d\t%s\t%d.%03d\n", entry, timestamp,
				temperature.integer, temperature.fractional);
		fr = f_mount(&fs, "", 1);
		if (fr != FR_OK) {
			while (checkMemoryCard() != FR_OK) {
				if (blinkLedAndWaitModeChange(1000)) {
					cState = STATE_CONF_WAIT;
					goto START;
				}
			}
			continue;
		}
		/* Open or create a log file ready to append */
		fr = openAppend(&fil, logName);
		if (fr == FR_OK) {
			canLog = true;
			/* Append a line */
			f_puts(buff, &fil);
			f_close(&fil);
		} else {
			while (checkMemoryCard() != FR_OK) {
				if (blinkLedAndWaitModeChange(1000)) {
					cState = STATE_CONF_WAIT;
					goto START;
				}
			}
			continue;
		}
	}
//	GPIO_WriteBit(GPIOA, LED_PIN, 0);
//	GPIO_WriteBit(GPIOA, TEMP_ENABLE_PIN, 0);
//	GPIO_WriteBit(GPIOA, SD_ENABLE_PIN, 0);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);
	SPI_Cmd(SPI1, DISABLE);
	gpioInitStruct.GPIO_Pin = GPIO_Pin_All;
	gpioInitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &gpioInitStruct);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, DISABLE);
	PWR_WakeUpPinCmd(ENABLE);
	RTC_SetAlarm(RTC_GetCounter() + logDelay);
	RTC_WaitForLastTask();

	PWR_EnterSTANDBYMode();
}

bool bufferContainsCommand() {
	if ((buff[0] == 'g' && buff[1] == 't')
			|| ((buff[0] == 't' && buff[1] == 'g'))) {
		cCommand = COMM_GET_TIME;
		return true;
	}
	if ((buff[0] == 'g' && buff[1] == 'd')
			|| ((buff[0] == 'd' && buff[1] == 'g'))) {
		cCommand = COMM_GET_DELAY;
		return true;
	}
	if ((buff[0] == 's' && buff[1] == 't')
			|| ((buff[0] == 't' && buff[1] == 's'))) {
		cCommand = COMM_SET_TIME;
		return true;
	}
	if ((buff[0] == 's' && buff[1] == 'd')
			|| ((buff[0] == 'd' && buff[1] == 's'))) {
		cCommand = COMM_SET_DELAY;
		return true;
	}
	return false;
}

/* Interrupt handlers---------------------------------------------------------*/
void USART1_IRQHandler(void) {
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		buff[buffP] = (char) USART_ReceiveData(USART1);
		if (cState == STATE_CONF_WAIT) {
			if (bufferContainsCommand()) {
				cState = STATE_CONF_GOT_COMMAND;
				buff[0] = 0;
				buff[1] = 0;
				buffP = 0;
			} else {
				buffP = !buffP;
			}
		} else {
			if (buffP == (sizeof(buff) - 1)) {
				buffP = 0;
			} else {
				buffP++;
			}
		}
	}
}

/**
 * @brief  Retargets the C library printf function to the USART.
 * @param  None
 * @retval None
 */
PUTCHAR_PROTOTYPE {
	USART_SendData(USART1, (uint8_t) ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {
	}
	return ch;
}
