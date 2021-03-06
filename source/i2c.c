#define I2C
#include "alldefs.h"
#include "msp430f149.h"
#include "i2c.h"

/*  I2C 代码分为三级：
**    1.I2C总线驱动 ：实现硬件I2C和软件模拟I2C时(编译时配置)
**                    向I2C总线上的设备发送多字节、从I2C总线上的设备接收多字节
**
**    2.I2C设备驱动 ：用到I2C总线的设备有3个: PCF8562-段码液晶驱动芯片、
**                                            PCF8563-RTC时钟芯片
**                                            FM24CL04-4K铁电寄存器
**    3.上层应用代码:
*/
/*     本文件实现  I2C总线初始化、I2C发送接收函数
*/


/*******************************************************************
                  I2c总线初始化
函数原型: void  init_I2c();  
功能:     根据IIC配置(硬件 or IO模拟)初始化IIC总线
********************************************************************/
void Init_I2c(void)
{
#if IIC_IO_SIM
  /*  上电初始化前，SDA、SCL引脚都为输入、通过上拉输出高电平/  */
  /*  初始化后，SDA引脚为输入，输出高电平、SCL为输出、输出低电平  */
  SCL_0;                  //SCL在下面一句执行后输出低电平
  IIC_PORT_DIR |= SCL;    //SCL引脚设置为输出  
  SDA_1;                  //SDA引脚设置为输入,被上拉，输出高电平
  delay_us(50);
#else

#endif  
  
}


#if IIC_IO_SIM
/*---------------------IO模拟IIC总线---------------------------*/
/*******************************************************************
                      起动总线函数               
函数原型: void   Start_I2c();  
功能:      启动I2C总线,即发送I2C起始条件.  
********************************************************************/
void Start_I2c(void)
{
   SDA_1;     /*发送起始条件的数据信号*/
   delay_us(1);
   SCL_1;
   delay_us(5);      /*起始条件建立时间大于4.7us,延时*/
    
   SDA_0;     /*发送起始信号*/
   delay_us(5);      /* 起始条件锁定时间大于4μs*/
        
   SCL_0;     /*钳住I2C总线，准备发送或接收数据 */
   delay_us(2);
}

/*******************************************************************
                       结束总线函数               
函数原型: void   Stop_I2c();  
功能:      结束I2C总线,即发送I2C结束条件.  
********************************************************************/
void Stop_I2c(void)
{
   SDA_0;    /*发送结束条件的数据信号*/
   delay_us(1);     /*发送结束条件的时钟信号*/
   SCL_1;    /*结束条件建立时间大于4us*/ 
   delay_us(5);
  
   SDA_1;    /*发送I2C总线结束信号*/
   delay_us(4);
}
/*******************************************************************
                  字节数据发送函数               
函数原型: void   SendByte(u_int8 c, u_int8* pAck);
功能:      将数据c发送出去,可以是地址,也可以是数据,发完后等待应答,并对
           此状态位进行操作.(不应答或非应答都使 *pAck=0)     
         发送数据正常，*pAck=1; *pAck=0表示被控器无应答或损坏。
********************************************************************/
void   SendByte(u_int8 c, u_int8 *pAck)
{
u_int8 BitCnt;

for(BitCnt=0;BitCnt<8;BitCnt++)   /*要传送的数据长度为8位*/
     {
      if((c<<BitCnt)&0x80)SDA_1;    /*判断发送位*/
        else   SDA_0;                
      delay_us(1);
      SCL_1;                /*置时钟线为高，通知被控器开始接收数据位*/
      
      delay_us(5);              /*保证时钟高电平周期大于4μs*/
             
      SCL_0; 
     }
    
     delay_us(2);
     SDA_1;                 /*8位发送完后释放数据线，准备接收应答位*/
     delay_us(2);   
     SCL_1;
     delay_us(5);
     if( IIC_PORT_IN&SDA ){  *pAck=0; } //while(1);   }    //debug_103   debug_103_end
        else *pAck=1;         /*判断是否接收到应答信号*/
     SCL_0;
     delay_us(2);
}
/*******************************************************************
                  字节数据接收函数               
函数原型: u_int8   RcvByte();
功能:      用来接收从器件传来的数据,并判断总线错误(不发应答信号)，
           发完后请用应答函数应答从机。  
********************************************************************/ 
u_int8 RcvByte(void)
{
   u_int8 retc;
   u_int8 BitCnt;
  
   retc=0; 
   SDA_1;                /*置数据线为输入方式*/
   for(BitCnt=0;BitCnt<8;BitCnt++)
       {
         delay_us(1);   
         SCL_0;                   /*置时钟线为低，准备接收数据位*/
        
         delay_us(5);                  /*时钟低电平周期大于4.7μs*/
       
         SCL_1;                   /*置时钟线为高使数据线上数据有效*/
         delay_us(3);
         retc=retc<<1;
         if( IIC_PORT_IN&SDA ) retc |= 0x01;   /*读数据位,接收的数据位放入retc中 */
         delay_us(2); 
       }
   SCL_0;    
   delay_us(2);
   return(retc);
}

/********************************************************************
                      应答子函数
函数原型:   void Ack_I2c(u_int8 a);
功能:       主控器进行应答信号(可以是应答或非应答信号，由位参数a决定)
********************************************************************/
void Ack_I2c(u_int8 dat)
{
  
   if(!dat)SDA_0;            /*在此发出应答或非应答信号 */
         else SDA_1;
   delay_us(3);      
   SCL_1;
  
   delay_us(5);                     /*时钟低电平周期大于4μs*/
    
   SCL_0;                      /*清时钟线，钳住I2C总线以便继续接收*/
   delay_us(2);    
}

/*------------------------IO模拟IIC总线结束--------------------------------*/
#else
/*------------------------硬件IIC总线开始--------------------------------*/






#endif