#define COM
#include "alldefs.h"
#include "msp430f149.h"
#include "Com.h"

volatile uint8 u8RxbufStat;                  //uart0���ջ���״̬
volatile uint8 u8TxbufStat;                  //uart0���ͻ���״̬

uint8 u8Rxbuf0[RXBUFSIZE];          //uart0���ջ���
volatile uint8 u8Rxbuf0cnt;                  //usart0 Ҫ���յ��ֽ���
uint8 u8Txbuf0[TXBUFSIZE];          //uart0���ͻ���
volatile uint8 u8Txbuf0cnt;                  //uart0 Ҫ���͵��ֽ���

void Uart0Init(void);               //����0��ʼ��
                                    //����0��� ������������
//__interrupt vector=USART0RX_VECTOR  Uart0RcvISR()
                                    //����0���յ� һ֡ ���溯��
static void stUart0TimeOutTimerB(uint16 TimeOutMs );    //����0���ճ�ʱ ����
static void spUart0TimerOutTimerB();                      //����0��ʱ��ʱ ֹͣ
//__interrupt vector=TIMERB0_VECTOR  Uart0TimerOutedISR()
                                                        //����0��ʱ��
void Uart0FrameSnd(uint8 u8DatLen, uint8 *u8pDat);
void Uart0StrSnd(uint8 *u8Str);
void Uart0CharSnd(uint8 u8Char);


//
//����� ���� �� ����һ���ַ�
//                ����һ���ַ���

//

/*******************************************
�������ƣ�Uart0Init
��    �ܣ�uart0 ���ڳ�ʼ�� 9600-N-1 ����״̬
��    ������
����ֵ  ����
********************************************/
void Uart0Init(void)
{
   P3SEL |= BIT4 + BIT5;    //���� pin P3.4 -TXD P3.5 RXD
   P4DIR |= BIT4;
   P4DIR &= ~BIT5;
   
   U0CTL |= SWRST;
   U0CTL |= CHAR;
   U0TCTL = SSEL0;          //UCLK = ACLK  32k/9600
   UBR00 = 0x03;
   UBR10 = 0x00;
   U0MCTL = 0x4a;

   ME1 |= UTXE0 + URXE0;

   U0CTL &= ~SWRST;

   IE1 |= UTXIE0 + URXIE0;

   u8TxbufStat = TXBUF_IDLING;
   u8RxbufStat = RXBUF_RCVING;
                          
}

/******************************************
�������ƣ�Uart0CharSnd(uint8 u8Char )
��    �ܣ�����һ���ַ�
��    ����Ҫ���͵��ַ�
����ֵ  ���ޣ����ͻ���æµʱֱ�ӷ���
**************************************************/
void Uart0CharSnd(uint8 u8Char)
{
  if ( u8TxbufStat != TXBUF_IDLING )
    return;
  u8Txbuf0[0] = u8Char;
  u8Txbuf0cnt = 1;
  u8TxbufStat = TXBUF_SNDING;
  IFG1 &= ~UTXIFG0;               
  U0TXBUF = u8Txbuf0[0];          //��������
}


/******************************************
�������ƣ�Uart0NCharSnd(uint8 *Str , uint8 charNum)
��    �ܣ�����N���Լ���/
��    ��: Str �ַ�����ĸ�� �ַ�����
����ֵ  ���ޣ����ͻ���æµʱֱ�ӷ���
**************************************************/
void Uart0NCharSnd(uint8 *Str, uint8 charNum)
{
  uint8 i;
  
  if( u8TxbufStat != TXBUF_IDLING )
    return;
  
  i = 0;
  while( i < charNum )
  {
      if(i >= TXBUFSIZE  )
      break;
      u8Txbuf0[i++] = *Str++;
  }
  
  u8Txbuf0cnt = charNum;
  
  u8TxbufStat = TXBUF_SNDING;
  
  IFG1 &= ~UTXIFG0;               
  U0TXBUF = u8Txbuf0[0];          //��������


}


/******************************************
�������ƣ�Uart0StrSnd(uint8 *u8Str )
��    �ܣ���һ���ַ������뷢�ͻ��岢 ���� ����
��    ����*u8Str Ҫ���뷢�ͻ��������ַ���ָ��
����ֵ  ���ַ�������ʱֻ����һ����,��ǰ���ͻ���æ����ֱ�ӷ���
**************************************************/
void Uart0StrSnd(uint8 *u8Str)
{
  uint8 i;

  if( u8TxbufStat != TXBUF_IDLING )
    return;
  
  i = 0;
  while( i < TXBUFSIZE )
  {
    if(i >= TXBUFSIZE  )
      break;
    u8Txbuf0[i] = *u8Str++;
    if( *u8Str == '\0' )
      break;
    i++;
  }
  
  u8Txbuf0cnt = i+1;
  
  u8TxbufStat = TXBUF_SNDING;
  
  IFG1 &= ~UTXIFG0;               
  U0TXBUF = u8Txbuf0[0];          //��������


}

/*******************************************
�������ƣ�Uart0FrameSnd(uint8 u8DatLen, uint8 *u8pDat)
��    �ܣ���Ҫ���͵�֡���ݷ��� ���ͻ��壬����У��� ����������
��    ����u8DatLen ֡�� *u8pDat Ҫ�γ�֡���ݵĵ�ַָ��
����ֵ  �����ݹ�����ǰ���ͻ���æ ��ֱ�ӷ���
********************************************/
void Uart0FrameSnd(uint8 u8DatLen, uint8 *u8pDat)
{
  uint8 i;
  uint8 u8Sum;                          //У���
  
  if (u8DatLen + 1 > TXBUFSIZE)                     //��������Ƿ񳬷�Χ
    return;
  if ( u8TxbufStat != TXBUF_IDLING )            //���ͻ��� �Ƿ����
    return;                     
  
  for(i=0,u8Sum=0; i<u8DatLen; i++)
  {
    u8Txbuf0[i] = *u8pDat;
    u8Sum += *u8pDat++;
  }
  u8Txbuf0[i] = u8Sum;                 //���У���
  
  u8Txbuf0cnt = u8DatLen + 1;
  
  u8TxbufStat = TXBUF_SNDING;
  
  IFG1 &= ~UTXIFG0;               
  U0TXBUF = u8Txbuf0[0];          //��������
          
}

/*******************************************
�������ƣ�Uart0TxISR()
��    �ܣ�uart0 ���� �жϷ������
��    ������
����ֵ  ����
********************************************/
#pragma vector=UART0TX_VECTOR

__interrupt void Uart0TxISR(void) 
{
  static uint8 i=0;
  
  i++;
  
  if(i < u8Txbuf0cnt )
    U0TXBUF = u8Txbuf0[i];
  else                                                //һ֡������ɣ������־
  {
     u8TxbufStat  = TXBUF_IDLING;
     u8Txbuf0cnt = 0;
     i = 0;
  }

}

/*******************************************
�������ƣ�Uart0RxISR()
��    �ܣ�uart0 ���� �жϷ������
��    ������
����ֵ  ����
********************************************/
static uint8 i=0;//�ڲ��������� ͨ�ų�ʱ���� ���õ�
#pragma vector= UART0RX_VECTOR
__interrupt void Uart0RxISR(void) 
{
 
  uint8 tmp;
  
  tmp = U0RXBUF;
  
  //clr and stop ��ʱ ��ʱ��
  spUart0TimerOutTimerB();
  
  if (u8RxbufStat != RXBUF_RCVING)
    return;
  if ( i == 0 )     //���յ�֡ͷ
  {
    if(tmp != 0x5a) //�ж�֡ͷ�Ƿ���ȷ
      return;
    u8Rxbuf0[i++] = tmp; //��ȷ���յ�֡ͷ
                         //start ��ʱ ��ʱ��
    stUart0TimeOutTimerB(10);
    return;
  }
  
  
  
  if( i == 1 )    //���յ�֡����
  {
    if(tmp > RXBUFSIZE )  //���岻��
    { i=0; return;}
    u8Rxbuf0cnt = tmp;      //���� ��
    u8Rxbuf0[i++] = tmp;
                            //start ��ʱ ��ʱ��
    stUart0TimeOutTimerB(10);
    return;
  }
  
  if( i < u8Rxbuf0cnt-1 )//�洢֡����
  {
    u8Rxbuf0[i++] = tmp;
                            //start  ��ʱ ��ʱ��
    stUart0TimeOutTimerB(10);
    return;
  }  
  
  if( i >= u8Rxbuf0cnt-1 ) //�յ�֡��β
  {
    u8Rxbuf0[i] = tmp;
    for(i=0,tmp=0; i<u8Rxbuf0cnt-1; i++)
    {
      tmp += u8Rxbuf0[i];   //����У���
    }
    
    if(tmp != u8Rxbuf0[u8Rxbuf0cnt-1] ) //У��ʹ���
    {
      i=0;
      u8Rxbuf0cnt = 0;
      
      return;
    }
  
  }
    
  //�ɹ�����һ֡����   
  i = 0;  
  IE1 &= ~UTXIE0;
  U0TXBUF = 0x5b;         //���� "0x5B"
  while(! (IFG1 & UTXIFG0) );
  IFG1 &= ~ UTXIFG0;
  IE1 |= UTXIE0;
  
  //�������� ������Ϣ.
  u8RxbufStat = RXBUF_DEALING;
  
  
  
   
}





/*******************************************
�������ƣ�stUart0TimeOutTimerB(uint16 TimeOutMs )
��    �ܣ����� uart0 ��ʱ��ʱ�� 
��    ����uint16  TimeOutMs ��ʱ������
����ֵ  ����
˵    ������ʱ�� TBCLK ���� ACLK ������ LP3 �� ��ʱ
********************************************/
static void stUart0TimeOutTimerB(uint16 TimeOutMs )
{
  TBCTL  = TBCLR;
  TBCCTL0 = CCIE;
  TBCCR0 = TimeOutMs * ( 32678/8/1000 );
  TBCTL |= TBSSEL_1 + ID_3 + MC_1;      // ACLK/8  = 32678 / 8
  return;
}


/*******************************************
�������ƣ�spUart0TimeOutTimerB(void )
��    �ܣ�ֹͣuart0 ��ʱ��ʱ
��    ����
����ֵ  ����
˵    �����ڳ�ʱʱ���� ������ ��һ�ֽ� ���øú���
********************************************/
static void spUart0TimerOutTimerB(void)
{
  TBCCTL0 &= ~CCIE;
  TBCTL |= MC_0; 
}

/*******************************************
�������ƣ�Uart0TimerOutedISR(void )
��    �ܣ�ֹͣuart0 ��ʱ��ʱ
��    ����
����ֵ  ����
˵    ������ʱʱ�䵽
********************************************/
#pragma vector = TIMERB0_VECTOR
__interrupt void Uart0TimerOutedISR(void)
{
  i = 0;
  u8Rxbuf0cnt = 0;  
  spUart0TimerOutTimerB();
}