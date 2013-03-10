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

    //����������
//    ConfigBoard(0xb8);
    
   //���ڳ�ʼ��
   Uart0Init();       

   //���⹦��IO��ʼ��
    /*
    ** P1.3 ----> CCI2A input BIT3
    */      
    /* P1.4 �������
    */
    /*  P1.4 OUT 125K�������, P1.3  IN  busy���Ų�������  */
    P1SEL |= BIT2 + BIT3 + BIT4;
    P1DIR |= BIT2; 
    P1DIR &= ~BIT3;
    P1DIR |= BIT4;

    
    //I2C���߳�ʼ��
    Init_I2c();
    PCF8562_init();
    
    delay_ms(100);
    //PCF8562��ʼ��
    
    //�����ж�
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