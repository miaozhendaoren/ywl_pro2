#define PCF8562
#include "msp430f149.h"
#include "alldefs.h"
#include "I2C.h"
#include "PCF8562.h"


#define IICADDR8562    0x70      //8562 IIC 地址 0x70

/**************************************************************
名称：u_int8 PCF8562_init(void)
功能：PCF8562初始化：静态,1/2偏置,显示,成功返回1
**************************************************************/
u_int8  PCF8562_init(void)
{ 
    //初始化iic硬件端口
    iic_init();

    iic_start();
    //发送 PCF8562 IIC地址
    iic_SndByte(IICADDR8562);
    
    if( 0 == iic_isSlaveAck() )
    {//PCF8562无响应
      iic_stop();
      return (0);
    }
    
    //方式设定: 允许显示、偏置1/2、静态
    iic_SndByte(0x4d);  //0 1 0 0 1 1  0  1
                        //C 1 0 x E B M1 M0
    if( 0 == iic_isSlaveAck() )
    {
      iic_stop();
      return(0);
    }
    
    iic_stop();
    return(1);
}

/**************************************************************
名称：u_int8  PCF8562_disStr(u_int8 *buf, u_int8 num )
功能：PCF8562显示字符串,返回PCF8562接收的字节个数,num为字节个数，
              buf为显示缓冲区
**************************************************************/
u_int8  PCF8562_disStr(u_int8 *buf, u_int8 num )
{
  u_int8 i = 0;

  if( num > 4 )
      return 0;
  
    iic_init();
    iic_start();
    iic_SndByte(IICADDR8562);
    if( 0 == iic_isSlaveAck() )
    {//PCF8562无响应
      iic_stop();
      return (i);
    }

    iic_SndByte(0x00);  //从地址指针0开始
    if( 0 == iic_isSlaveAck() )
    {//PCF8562无响应
      iic_stop();
      return (i);
    }        
    for( ; num; num--)
    {
      iic_SndByte(buf[i]);
      if( 0 == iic_isSlaveAck() )
      {
        iic_stop();
        return(i);
      }
      i++;
    }
    
    iic_stop();
    return (i);   
}

