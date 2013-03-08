#define I2C
#include "alldefs.h"
#include "msp430f149.h"
#include "i2c.h"

/*  I2C 代码分为三级：
**    1.I2C总线驱动 ：实现硬件I2C和软件模拟I2C时(编译时配置)
**                    向I2C总线发数据及从I2C总线获取数据
**                    及应答从设备 和 检测从设备的应答
**                    总线互斥
**
**    2.I2C设备驱动 ：用到I2C总线的设备有3个: PCF8562-段码液晶驱动芯片、
**                                            PCF8563-RTC时钟芯片
**                                            FM24CL04-4K铁电寄存器
**                    主要内容：具体设备操作，如初始化、读写数据
**    3.上层应用代码:
**                    操作顺序：
**                              1.检测总线当前是否可被使用,初始化线
**                              2.驱动设备
**                              3.发送或接收数据
*/

/*     本文件实现  I2C总线驱动
**                  1.硬件I2C 或 IO模拟I2C配置
**                  2.硬件I2C 硬件模块 配置
**                  3.IO模拟I2C IO口 配置
**                  4.硬件初始化总线、发送、接收字节
**                  5.软件初始化总线、发送、接收字节
**                  6.总线互斥
*/

/**********************************************************************
IIC总线变量定义
***********************************************************************/
IICSTATE ge_iicState = eIIC_FREE; 


#if IIC_IO_SIM
/*---------------------IO模拟IIC总线---------------------------*/
#define IIC_PORT_DIR  P2DIR
#define IIC_PORT_IN   P2IN   
#define IIC_PORT_OUT  P2OUT   

#define SDA    BIT6   
#define SCL    BIT7  
#define SDA_in  (IIC_PORT_DIR &= ~SDA)
#define SDA_out (IIC_PORT_DIR |= SDA)
#define SDA_1  (IIC_PORT_OUT |= SDA)
#define SDA_0  (IIC_PORT_OUT &= SDA)

#define SCL_out (IIC_PORT_DIR |= SCL)
#define SCL_1  (IIC_PORT_OUT |=SCL)   
#define SCL_0  (IIC_PORT_OUT &=~SCL)   
#define IIC_TIME 100   
  
/*******************************************************************  
名称: void iic_init();    
功能: IO模拟IIC总线 硬件初始化
********************************************************************/   
void iic_init(void)   
{   
 SCL_0;
 SCL_out;
 SDA_1;
 SDA_out;
}   
/*******************************************************************  
                     起动总线函数                 
函数原型: void  iic_start();    
功能:启动I2C总线    
********************************************************************/   
void iic_start(void)   
{   
 SDA_1; delay_us(IIC_TIME);   
 SCL_1; delay_us(IIC_TIME);

 SDA_0; delay_us(IIC_TIME);   
 SCL_0; delay_us(IIC_TIME);   
}   
/*******************************************************************  
                      结束总线函数                 
函数原型: void  iic_stop();    
功能:       结束I2C总线
    
********************************************************************/   
void iic_stop(void)   
{   
 SDA_0; delay_us(IIC_TIME);   
 SCL_1; delay_us(IIC_TIME);   
 SDA_1; delay_us(IIC_TIME);   
 SCL_0; delay_us(IIC_TIME);   
}   
/*******************************************************************  
名称: void  iic_SndByte(u_int8 ch);  
功能: 向从机发送一字节,发送完毕后需用isack()检测从机是否接收
********************************************************************/   
void  iic_SndByte(u_int8  ch)   
{   
 u_int8 i = 8;   
 SCL_0; delay_us(IIC_TIME);     
 while(i--)   
 {   
  if(ch & 0x80)
    SDA_1;   
  else
    SDA_0;
  ch <<= 1;   
  delay_us(IIC_TIME);     
  SCL_1; delay_us(IIC_TIME);     
  SCL_0; delay_us(IIC_TIME);     
 }     
}   
/*******************************************************************  
函数原型: u_int8  iic_RcvByte();  
功能:  接收总线数据1字节 , 发完后请用应答函数。    
********************************************************************/      
u_int8  iic_RcvByte(void)   
{   
 u_int8 rdata = 0;   
 u_int8 i = 8;   
 SDA_in;
 delay_us(IIC_TIME);   
 while(i--)   
 {   
  rdata <<= 1;   
  SCL_0; delay_us(IIC_TIME);     
  SCL_1; delay_us(IIC_TIME);     
  if(IIC_PORT_IN &SDA) rdata |= 0x01; // 读入数据            
 }   
 SCL_0;       
 delay_us(IIC_TIME);   

 return(rdata);   
}   
   
/***************************************************************
名称：void iic_master_ack(void)
功能：主机 positive应答
***************************************************************/
void iic_master_ack(void)
{   
 SDA_0; SDA_out;delay_us(IIC_TIME);   
 SCL_1; delay_us(IIC_TIME);   
 SCL_0; delay_us(IIC_TIME);   
}   

/***************************************************************
名称：void iic_master_nack(void)
功能：主机 negative应答
***************************************************************/
void iic_master_nack(void)
{   
 SDA_1; SDA_out; delay_us(IIC_TIME);   
 SCL_1; delay_us(IIC_TIME);     
 SCL_0; delay_us(IIC_TIME);     
}   
/***************************************************************
名称：u_int8 iic_isSlaveAck(void)
功能：主机检测从机应答是否为ACK,是ACK返回1，否则返回0
***************************************************************/   
u_int8 iic_isSlaveAck(void)   
{   
 u_int8 a;  
 SDA_in; 
 SCL_1;                   //产生应答时钟
 delay_us(IIC_TIME);
 a=IIC_PORT_IN &SDA;
 SCL_0;
 SDA_1;
 SDA_out;       //总线空闲期间默认SDA_1
 return(a);    
}  

/*------------------------IO模拟IIC总线结束--------------------------------*/
#else
/*------------------------硬件IIC总线开始--------------------------------*/






#endif