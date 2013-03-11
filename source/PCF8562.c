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
    //方式设定: 允许显示、偏置1/2、静态
    //0 1 0 0 1 1  0  1
    //C 1 0 x E B M1 M0
  
  u_int8 initCode[2] = {IICADDR8562, 0x4d};
  if( ISendStr(IICADDR8562,0x4d,&initCode[0],0) )
    return (1);
  else
    return (0);
}

/**************************************************************
名称：u_int8  PCF8562_disStr(u_int8 *buf, u_int8 start_addr u_int8 num )
功能：PCF8562显示字符串, start_addr 为PCF8562开始显示的bit位序号, num为要显示的字节个数
返回： 成功返回1，否则返回0
**************************************************************/
u_int8  PCF8562_disStr(u_int8 *buf, u_int8 start_addr, u_int8 num  )
{
  start_addr = start_addr & 0x3f; //最高两位 C 0 P5 P4 P3 P2 P1 P0
  if( ISendStr(IICADDR8562, start_addr, buf, num) )
    return (1);
  else
    return (0);
}
