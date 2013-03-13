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
  
   //ϵͳʱ�ӳ�ʼ��
    BCSCTL1 &=~ XT2OFF;                 //��XT���� ;����ʱ��ϵͳ���ƼĴ���1
    IE1 &= ~OFIE;
    do{
      IFG1&=~OFIFG;                   //����𵴱�־; IFG1Ϊ�жϱ�־�Ĵ���
      for(i=0;i<100;i++)
         _NOP();                      
    }while((IFG1&OFIFG)!=0);          //�ȴ����ȶ�   
    IFG1&=~OFIFG;                     //����𵴱�־;
    BCSCTL2 |= SELM_2;                //MCLK = XT2CLK = 8MHz
    BCSCTL2|= SELM_2 + SELS + DIVS_2; //XT2CLK = 8MHz, SMCLK from XT2CLK/4 = 2MHz

    //debug_105
    ConfigBoard(0xb8);
    //debug_105_end
    
    //���ڳ�ʼ��
    Uart0Init();
    //I2C���߳�ʼ��
    Init_I2c();
    
    //�����ж�    
    _EINT();    
    while(1)
    {
      /*
        //д��
        Write_FRAM(0,&test[0],9);
        Uart0NCharSnd(&test[0],9);
        while(TXBUF_SNDING == u8TxbufStat);
        Uart0CharSnd('\n');
        while(TXBUF_SNDING == u8TxbufStat);
        Uart0CharSnd('W');
        while(TXBUF_SNDING == u8TxbufStat);
        Uart0CharSnd('\n');
        while(TXBUF_SNDING == u8TxbufStat);
      
        //����
        for(i=0; i<10; i++)
          test[i] = 0x30 + 0;
        Uart0NCharSnd(&test[0],9);
        while(TXBUF_SNDING == u8TxbufStat);
        Uart0CharSnd('\n');
        while(TXBUF_SNDING == u8TxbufStat);
      */
        
        //����
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