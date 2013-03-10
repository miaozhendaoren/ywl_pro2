#include "msp430f149.h"
#include "alldefs.h"
#include "BoardConfig.h"
#include "Com.h"
#include <stdio.h>
#include "Lib.h"
#include "TimerAinit.h"
#include <stdlib.h>
#include "db18b20.h"
#include "i2c.h"
#include "PCF8562.h"
#include "PCF8563.h"
#include "FM24CL64.h"

void main(void)
{
  unsigned long long_i;
  u_int8 i,j;    
  WDTCTL = WDTPW + WDTHOLD;
  
   //系统时钟初始化
    BCSCTL1 &=~ XT2OFF;                 //打开XT振荡器 ;基本时钟系统控制寄存器1
    IE1 &= ~OFIE;
    do{
      IFG1&=~OFIFG;                   //清除震荡标志; IFG1为中断标志寄存器
      for(i=0;i<100;i++)
         _NOP();                      
    }while((IFG1&OFIFG)!=0);          //等待震荡稳定   
    IFG1&=~OFIFG;                     //清除震荡标志;
    BCSCTL2 |= SELM_2;                //MCLK = XT2CLK = 8MHz
    BCSCTL2|= SELM_2 + SELS + DIVS_2; //XT2CLK = 8MHz, SMCLK from XT2CLK/4 = 2MHz

    //开发板配置
//    ConfigBoard(0xb8);
    
   //串口初始化
   Uart0Init();       

   //特殊功能IO初始化
    /*
    ** P1.3 ----> CCI2A input BIT3
    */      
    /* P1.4 脉冲输出
    */
    /*  P1.4 OUT 125K脉冲输出, P1.3  IN  busy引脚捕获输入  */
    P1SEL |= BIT2 + BIT3 + BIT4;
    P1DIR |= BIT2; 
    P1DIR &= ~BIT3;
    P1DIR |= BIT4;

    
    //I2C总线初始化
    Init_I2c();
    PCF8562_init();
    
    delay_ms(100);
    //PCF8562初始化
    
    //开总中断
    _EINT();    

    long_i = 0x00000000;
    while(1)
    {
      for(j=0; j<=32; j++)
      {
          long_i = 0x80000000 >> j;
          PCF8562_disStr((u_int8*)(&long_i),0x00, 4);
          delay_ms(800);
      }
      long_i = 0xffffffff;
      PCF8562_disStr((u_int8 *)(&long_i),0x00,4);
      delay_ms(800);
      for(j=0; j<=32; j++)
      {
          long_i = 0x7fffffff >> j;
          PCF8562_disStr((u_int8*)(&long_i),0x00, 4);
          delay_ms(800);
      }
      long_i = 0xffffffff;
      PCF8562_disStr((u_int8 *)(&long_i),0x00,4);
      delay_ms(800);

    }
    
}//end main()