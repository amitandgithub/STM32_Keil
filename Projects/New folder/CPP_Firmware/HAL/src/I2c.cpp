
#include "stm32f10x.h"
#include "stm32f10x_i2c.h"
#include "I2c.hpp"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
namespace Bsp {
  
void I2c::I2c_Demo_Init()
{
	I2C_InitTypeDef I2cConfig;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable I2C1 clock */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C1, ENABLE );	
	
	/* Enable GPIOB clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );
	
	/* Configure I2C SCL(PB8) and SDA (PB9) as Alternate Output Push Pull configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init( GPIOB, &GPIO_InitStructure );
	
	/* I2C1 Reset */
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1 , ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1 , DISABLE);
		
	I2cConfig.I2C_Ack = I2C_Ack_Enable;
	I2cConfig.I2C_Mode = I2C_Mode_I2C;
	I2cConfig.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2cConfig.I2C_DutyCycle = I2C_DutyCycle_2;
	I2cConfig.I2C_ClockSpeed =400000;
	I2cConfig.I2C_OwnAddress1 = 0x50;
	
	I2C_Init(I2C1,&I2cConfig);
}


u32 I2c::I2C_Write_Poll(I2C_TypeDef* I2Cx , const unsigned char * buf ,u32 nbyte , unsigned char SlaveAddress) 
{
	volatile uint32_t Timeout = 0;
	if (nbyte)
	{
		Timed(I2C_GetFlagStatus(I2Cx , I2C_FLAG_BUSY));
		// Intiate Start Sequence
		I2C_GenerateSTART(I2Cx , ENABLE);
		Timed (! I2C_CheckEvent(I2Cx , I2C_EVENT_MASTER_MODE_SELECT));
		// Send Address EV5
		I2C_Send7bitAddress(I2Cx , SlaveAddress ,	I2C_Direction_Transmitter);
		Timed (! I2C_CheckEvent(I2Cx , I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
		// EV6 Write first byte EV8_1
		I2C_SendData(I2Cx , *buf ++);
		while (--nbyte) 
		{
			// wait on BTF
			Timed (! I2C_GetFlagStatus(I2Cx , I2C_FLAG_BTF));
			I2C_SendData(I2Cx , *buf ++);
		}
		Timed (! I2C_GetFlagStatus(I2Cx , I2C_FLAG_BTF));
		I2C_GenerateSTOP(I2Cx , ENABLE);
		Timed(I2C_GetFlagStatus(I2C1 , I2C_FLAG_STOPF));
	}
	return 0;
	errReturn:
		#if PRINT
		printf("Error In I2c : 0x%x\r\n",(unsigned int)I2C_GetLastEvent(I2Cx));
		#endif
	return 1;
}

u32 I2c::I2C_Read_Poll(I2C_TypeDef* I2Cx, u8 *buf,u32 nbyte, u8 SlaveAddress)
{
  vu32 Timeout = 0;
  if (!nbyte)
    return 0;

	// Wait for idle I2C interface

  Timed(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

  // Enable Acknowledgement, clear POS flag

  I2C_AcknowledgeConfig(I2Cx, ENABLE);
  I2C_PECPositionConfig(I2Cx, I2C_PECPosition_Current);

  // Intiate Start Sequence (wait for EV5

  I2C_GenerateSTART(I2Cx, ENABLE);
  Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

  // Send Address

  I2C_Send7bitAddress(I2Cx, SlaveAddress, I2C_Direction_Receiver);

  // EV6
  
  Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDR));

  if (nbyte == 1)
    {

      // Clear Ack bit      

      I2C_AcknowledgeConfig(I2Cx, DISABLE);       

      // EV6_1 -- must be atomic -- Clear ADDR, generate STOP

      __disable_irq();
      (void) I2Cx->SR2;                           
      I2C_GenerateSTOP(I2Cx,ENABLE);      
      __enable_irq();

      // Receive data   EV7

      Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE));
      *buf++ = I2C_ReceiveData(I2Cx);

    }
  else if (nbyte == 2)
    {
      // Set POS flag

      I2C_PECPositionConfig(I2Cx, I2C_PECPosition_Next);

      // EV6_1 -- must be atomic and in this order

      __disable_irq();
      (void) I2Cx->SR2;                           // Clear ADDR flag
      I2C_AcknowledgeConfig(I2Cx, DISABLE);       // Clear Ack bit
      __enable_irq();

      // EV7_3  -- Wait for BTF, program stop, read data twice

      Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF));

      __disable_irq();
      I2C_GenerateSTOP(I2Cx,ENABLE);
      *buf++ = I2Cx->DR;
      __enable_irq();

      *buf++ = I2Cx->DR;

    }
  else 
    {
      (void) I2Cx->SR2;                           // Clear ADDR flag
      while (nbyte-- != 3)
	{
	  // EV7 -- cannot guarantee 1 transfer completion time, wait for BTF 
    //        instead of RXNE

	  Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF)); 
	  *buf++ = I2C_ReceiveData(I2Cx);
	}

      Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF));  

      // EV7_2 -- Figure 1 has an error, doesn't read N-2 !

      I2C_AcknowledgeConfig(I2Cx, DISABLE);           // clear ack bit

      __disable_irq();
      *buf++ = I2C_ReceiveData(I2Cx);             // receive byte N-2
      I2C_GenerateSTOP(I2Cx,ENABLE);                  // program stop
      __enable_irq();

      *buf++ = I2C_ReceiveData(I2Cx);             // receive byte N-1

      // wait for byte N

      Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)); 
      *buf++ = I2C_ReceiveData(I2Cx);

      nbyte = 0;

    }

  // Wait for stop

  Timed(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF));
  return 0;

 errReturn:
 	#if PRINT
		printf("Error In I2c : 0x%x\r\n",(unsigned int)I2C_GetLastEvent(I2Cx));
	#endif
  // Any cleanup here
  return 1;

}

static uint8_t slaveAddress = 0;
uint8_t I2c::ScanDevice(I2C_TypeDef* m_I2Cx,uint8_t SlaveAddress)
{
   vu32 Timeout = 0;

  Timed(I2C_GetFlagStatus(m_I2Cx , I2C_FLAG_BUSY));
  // Intiate Start Sequence
  I2C_GenerateSTART(m_I2Cx , ENABLE);
  Timed (! I2C_CheckEvent(m_I2Cx , I2C_EVENT_MASTER_MODE_SELECT));
  // Send Address EV5
  I2C_Send7bitAddress(m_I2Cx , SlaveAddress ,	I2C_Direction_Transmitter);
  Timed (! I2C_CheckEvent(m_I2Cx , I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  slaveAddress = SlaveAddress;
  // EV6 Write first byte EV8_1
  return 0;
 errReturn:
  return 1;
}


}
