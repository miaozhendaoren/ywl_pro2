#include <stdio.h>
#include <stdlib.h>
#include "msp430f149.h"
#include "alldefs.h"
#include "BoardConfig.h"
#include "Lib.h"

#include "TimerAinit.h"
#include "Com.h"
#include "db18b20.h"
#include "i2c.h"
#include "PCF8562.h"
#include "segLcd.h"
#include "PCF8563.h"
#include "FM24CL64.h"

void main(void)
{
    u_int8 i;
    u_int8 test[10];

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

    //debug_105
    ConfigBoard(0xb8);
    //debug_105_end
    
    //串口初始化
    Uart0Init();
    //I2C总线初始化
    Init_I2c();
    
    //开总中断    
    _EINT();    
    while(1)
    {
      /*
        //写入
        Write_FRAM(0,&test[0],9);
        Uart0NCharSnd(&test[0],9);
        while(TXBUF_SNDING == u8TxbufStat);
        Uart0CharSnd('\n');
        while(TXBUF_SNDING == u8TxbufStat);
        Uart0CharSnd('W');
        while(TXBUF_SNDING == u8TxbufStat);
        Uart0CharSnd('\n');
        while(TXBUF_SNDING == u8TxbufStat);
      
        //清零
        for(i=0; i<10; i++)
          test[i] = 0x30 + 0;
        Uart0NCharSnd(&test[0],9);
        while(TXBUF_SNDING == u8TxbufStat);
        Uart0CharSnd('\n');
        while(TXBUF_SNDING == u8TxbufStat);
      */
        
        //读出
        Read_FRAM(1,&test[0],9);
        Uart0NCharSnd(&test[0],9);        
        while(TXBUF_SNDING == u8TxbufStat);
        for(i=0; i<10; i++)
          test[i] = 0;        
        Uart0CharSnd('\n');
        while(TXBUF_SNDING == u8TxbufStat);
        
        delay_ms(1000);  
    }
}//end main()