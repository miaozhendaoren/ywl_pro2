#include "msp430f149.h"
#include "alldefs.h"
#include "Com.h"
#include "BoardConfig.h"
#include <stdio.h>
#include "Lib.h"
#include "TimerAinit.h"
#include <stdlib.h>

void main(void)
{
  u_int8 str[20]; 
  u_int8 i;
  u_int32 u32result = 0;
  float Vref = 1.00;    //基准电压1.00
  float Vin = 0.0;
    
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

   //串口初始化
   Uart0Init();       

   
   //特殊功能IO初始化
    /* P1.1 ----> CCI0A input BIT1
    ** P1.2 ----> CCI1A input BIT2
    ** P1.3 ----> CCI2A input BIT3
    */      
    /* P1.1 ----> TA0 Out0    BIT1
    ** P1.2 ----> TA1 Out1    BIT2 
    ** P1.3 ----> TA2 Out2    BIT3
    */
    /*  P1.2 OUT 125K脉冲输出, P1.3  IN  busy引脚捕获输入  */
    P1SEL |= BIT2 + BIT3 + BIT4;
    P1DIR |= BIT2; 
    P1DIR &= ~BIT3;
    P1DIR |= BIT4;

    //开总中断
    _EINT();    
    while(1)
    {
      //清除上次测量结果
      u16end_CCR2 = 0;
      u16start_CCR2 = 0;
      u16ovCnt_TAR = 0;
      //启动测量
      TimerA_reset();
      //等待测量完成
      while( 1 == u8isMeasuring );    
      //关闭测量
      TimerA_stop();
      //计算测量结果
      if(0 == u16ovCnt_TAR)
      {
        u32result = u16end_CCR2 - u16start_CCR2;
      }else{
        u32result = u16end_CCR2 + (65536L-u16start_CCR2) + 65536L*u16ovCnt_TAR;
      }
      //计算结果
      
      if(u32result >= 10000)
        Vin = (u32result - 10000) * Vref / 10000;
        
      //测量结果输出到串口
      if( u32result )
      {
        sprintf((char *)str,"%f", Vin );
        Uart0StrSnd(str);
        while(TXBUF_SNDING == u8TxbufStat);//等待发送完成
        Uart0CharSnd('\n');
        while(TXBUF_SNDING == u8TxbufStat);      
      }
    }
    
}//end main()