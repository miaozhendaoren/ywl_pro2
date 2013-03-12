#define TIMERAINIT
#include "msp430f149.h"
#include "alldefs.h"

u_int16 u16start_CCR2 = 0;     //CCR2�����ز���ʱTAR��ֵ
u_int16 u16end_CCR2 = 0;       //CCR2�½��ز���ʱTAR��ֵ
u_int16 u16ovCnt_TAR = 0;      //��CCR2 ������ �� �½��� ֮�� TAR���������
                               //(��start_TAR��ֵ��ʼ��65535���ٵ��½���ʱ��end_TAR)
u_int8 u8isMeasuring = 0;        //�Ƿ����ڲ���(�������)��

static u_int8 u8CapPos_edge = 0;  //�����ز���
/*******************************************
�������ƣ�TimerA_reset
��    �ܣ���ʱ��A��ʼ��
��    ������
����ֵ  ����
˵    ����CCR2���ڲ���,����TAR������ΪMCLK��SMCLK������Ƶ��1MHz
********************************************/
void TimerA_reset(void)
{
  u16start_CCR2 = 0;
  u16end_CCR2 = 0;
  u16ovCnt_TAR = 0;

  //����������⹦��IO��ʼ��

  /* P1.3 ----> CCR2��������CCI2A   ��ICL7135 busy���� 
  ** P1.4 SMCLK OUT 250K�������    ��ICL7135 clk����
  */
  P1SEL |= BIT3 + BIT4;
  P1DIR &= ~BIT3;
  P1DIR |= BIT4;

  TACTL |= TACLR;
  CCTL2 = CAP + CM_1 + SCS + CCIS_0 + CCIE; //CCR2 ����ģʽ�������ز���ͬ������������� CCI2A(),�����ж�
  u8CapPos_edge = 1;
  
  TACTL = TASSEL_2 + ID_3 + MC_2 + TAIE;    //TACLK = SMCLKK��8��ƵΪ250K��������ģʽ,��������ж�
}

/*******************************************
�������ƣ�TimerA_stop
��    �ܣ���ʱ��Aֹͣ��ʱ
��    ������
����ֵ  ����
˵    ����
********************************************/
void TimerA_stop(void)
{
  TACTL &= ~(MC0 +MC1);    //ֹͣ��ʱ
}

/**********************************************
�������ƣ�TiemrA1_ISR
��    �ܣ�TimerA CCR1 CCR2 TA��ʱ���жϷ������
��    ������
����ֵ  ����
**********************************************/
#pragma vector=TIMERA1_VECTOR
__interrupt void TiemrA1_ISR(void)
{
  switch(TAIV)
  {
  case 2:         //TACCR1  �ж�����
    break;
  case 4:         //TACCR2  �ж�����  , �����ж�
    {
         if( u8CapPos_edge )
         {//�����ز���
            u16start_CCR2 = CCR2;
            u8isMeasuring = 1;   
            CCTL2 = CAP + CM_2 + SCS + CCIS_0 + CCIE;   //CCR2 ����ģʽ���½��ز���ͬ������������� CCI2A(),�����ж�
            u8CapPos_edge = 0;
         }else{//�½��ز���
            u16end_CCR2 = CCR2;
            u8isMeasuring = 0;
            CCTL2 = CAP + CM_1 + SCS + CCIS_0 + CCIE;   //CCR2 ����ģʽ�������ز���ͬ�������������CCI2A(),�����ж�
            u8CapPos_edge = 1;
         }
    }break;
  case 10:        //TAIFG   �ж�����   , ����ж�
    {
        if( u8isMeasuring )
        {//������
           u16ovCnt_TAR++;
        }
    }break;
    
  default:
    break;
  }
  
}