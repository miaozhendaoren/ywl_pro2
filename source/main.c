#include "msp430f149.h"
#include "alldefs.h"
#include "Com.h"
#include "BoardConfig.h"
#include <stdio.h>
#include "Lib.h"
#include "TimerAinit.h"
#include <stdlib.h>

void Clock_Init()
{
  u_int8 i;
  BCSCTL1&=~XT2OFF;
  BCSCTL2|=SELM1+SELS + DIVS_2;
  do{
    IFG1&=~OFIFG;
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);
  IFG1&=~OFIFG; 
}


void main(void)
{
  u_int8 str[20]; 
  u_int8 i;
  u_int32 u32result = 0;
  
  WDTCTL = WDTPW + WDTHOLD;
  
#if 1
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
#endif 

#if 0
     BCSCTL1 &= ~XT2OFF;
     do{
        IFG1 &= ~OFIFG;d
        for(i=255; i>0; i--);
     }while( (IFG1 &OFIFG) != 0 );
     BCSCTL2 = SELM_2 + SELS + DIVS_2;
    
#endif    
   
   //Clock_Init();       
   //���ڳ�ʼ��
   Uart0Init();       

   
   //���⹦��IO��ʼ��
    /* P1.1 ----> CCI0A input BIT1
    ** P1.2 ----> CCI1A input BIT2
    ** P1.3 ----> CCI2A input BIT3
    */      
    /* P1.1 ----> TA0 Out0    BIT1
    ** P1.2 ----> TA1 Out1    BIT2 
    ** P1.3 ----> TA2 Out2    BIT3
    */
    /*  P1.2 OUT 125K�������, P1.3  IN  busy���Ų�������  */
    P1SEL |= BIT2 + BIT3 + BIT4;
    P1DIR |= BIT2; 
    P1DIR &= ~BIT3;
    P1DIR |= BIT4;

    //�����ж�
    _EINT();
    
    //��������
    TimerA_reset();
    
    while(1)
    {
     // while( 1 == u8isMeasuring );    //һ��ת�����
      
      if(0 == u16ovCnt_TAR)
      {
        u32result = u16end_CCR2 - u16start_CCR2;
      }else{
        u32result = u16end_CCR2 + (65536L-u16start_CCR2) + 65536L*u16ovCnt_TAR;
      }
      u16end_CCR2 = 0;
      u16start_CCR2 = 0;
      u16ovCnt_TAR = 0;
      
//      delay_ms(500);
//      u32result = 23246; 
      sprintf((char *)str,"%ld", u32result );
      Uart0StrSnd(str);
      while(TXBUF_SNDING == u8TxbufStat);//�ȴ��������
      Uart0CharSnd('\n');
      while(TXBUF_SNDING == u8TxbufStat);
    }
    
}//end main()