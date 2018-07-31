


#define Timed(x) Timeout = 0xFFFF; while (x) { if (Timeout -- == 0) goto errReturn ;}

namespace Bsp {
  
class I2c
{
public:
  void I2c_Demo_Init();
  u32 I2C_Write_Poll(I2C_TypeDef* I2Cx , const unsigned char * buf ,u32 nbyte , unsigned char SlaveAddress) ;
  u32 I2C_Read_Poll(I2C_TypeDef* I2Cx, u8 *buf,u32 nbyte, u8 SlaveAddress);
  uint8_t ScanDevice(I2C_TypeDef* m_I2Cx,uint8_t SlaveAddress);

};

}