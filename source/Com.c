#define COM
#include "alldefs.h"
#include "msp430f149.h"
#include "Com.h"

volatile uint8 u8RxbufStat;                  //uart0接收缓冲状态
volatile uint8 u8TxbufStat;                  //uart0发送缓冲状态

uint8 u8Rxbuf0[RXBUFSIZE];          //uart0接收缓冲
volatile uint8 u8Rxbuf0cnt;                  //usart0 要接收的字节数
uint8 u8Txbuf0[TXBUFSIZE];          //uart0发送缓冲
volatile uint8 u8Txbuf0cnt;                  //uart0 要发送的字节数

void Uart0Init(void);               //串口0初始化
                                    //串口0打包 ，并启动发送
//__interrupt vector=USART0RX_VECTOR  Uart0RcvISR()
                                    //串口0接收到 一帧 报告函数
static void stUart0TimeOutTimerB(uint16 TimeOutMs );    //串口0接收超时 启动
static void spUart0TimerOutTimerB();                      //串口0计时超时 停止
//__interrupt vector=TIMERB0_VECTOR  Uart0TimerOutedISR()
                                                        //串口0超时到
void Uart0FrameSnd(uint8 u8DatLen, uint8 *u8pDat);
void Uart0StrSnd(uint8 *u8Str);
void Uart0CharSnd(uint8 u8Char);


//
//需添加 功能 ： 发送一个字符
//                发送一个字符串

//

/*******************************************
函数名称：Uart0Init
功    能：uart0 串口初始化 9600-N-1 接收状态
参    数：无
返回值  ：无
********************************************/
void Uart0Init(void)
{
   P3SEL |= BIT4 + BIT5;    //设置 pin P3.4 -TXD P3.5 RXD
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
函数名称：Uart0CharSnd(uint8 u8Char )
功    能：发送一个字符
参    数：要发送的字符
返回值  ：无，发送缓冲忙碌时直接返回
**************************************************/
void Uart0CharSnd(uint8 u8Char)
{
  if ( u8TxbufStat != TXBUF_IDLING )
    return;
  u8Txbuf0[0] = u8Char;
  u8Txbuf0cnt = 1;
  u8TxbufStat = TXBUF_SNDING;
  IFG1 &= ~UTXIFG0;               
  U0TXBUF = u8Txbuf0[0];          //触发发送
}


/******************************************
函数名称：Uart0NCharSnd(uint8 *Str , uint8 charNum)
功    能：发送N个自己付/
参    数: Str 字符首字母， 字符个数
返回值  ：无，发送缓冲忙碌时直接返回
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
  U0TXBUF = u8Txbuf0[0];          //触发发送


}


/******************************************
函数名称：Uart0StrSnd(uint8 *u8Str )
功    能：将一个字符串放入发送缓冲并 触发 发送
参    数：*u8Str 要送入发送缓冲区的字符串指针
返回值  ：字符串过长时只发送一部分,或当前发送缓冲忙，都直接返回
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
  U0TXBUF = u8Txbuf0[0];          //触发发送


}

/*******************************************
函数名称：Uart0FrameSnd(uint8 u8DatLen, uint8 *u8pDat)
功    能：将要发送的帧数据放入 发送缓冲，计算校验和 并触发发送
参    数：u8DatLen 帧长 *u8pDat 要形成帧数据的地址指针
返回值  ：数据过长或当前发送缓冲忙 都直接返回
********************************************/
void Uart0FrameSnd(uint8 u8DatLen, uint8 *u8pDat)
{
  uint8 i;
  uint8 u8Sum;                          //校验和
  
  if (u8DatLen + 1 > TXBUFSIZE)                     //检查数据是否超范围
    return;
  if ( u8TxbufStat != TXBUF_IDLING )            //发送缓冲 是否空闲
    return;                     
  
  for(i=0,u8Sum=0; i<u8DatLen; i++)
  {
    u8Txbuf0[i] = *u8pDat;
    u8Sum += *u8pDat++;
  }
  u8Txbuf0[i] = u8Sum;                 //添加校验和
  
  u8Txbuf0cnt = u8DatLen + 1;
  
  u8TxbufStat = TXBUF_SNDING;
  
  IFG1 &= ~UTXIFG0;               
  U0TXBUF = u8Txbuf0[0];          //触发发送
          
}

/*******************************************
函数名称：Uart0TxISR()
功    能：uart0 发送 中断服务程序
参    数：无
返回值  ：无
********************************************/
#pragma vector=UART0TX_VECTOR

__interrupt void Uart0TxISR(void) 
{
  static uint8 i=0;
  
  i++;
  
  if(i < u8Txbuf0cnt )
    U0TXBUF = u8Txbuf0[i];
  else                                                //一帧发送完成，处理标志
  {
     u8TxbufStat  = TXBUF_IDLING;
     u8Txbuf0cnt = 0;
     i = 0;
  }

}

/*******************************************
函数名称：Uart0RxISR()
功    能：uart0 接收 中断服务程序
参    数：无
返回值  ：无
********************************************/
static uint8 i=0;//内部计数器， 通信超时服务 会用到
#pragma vector= UART0RX_VECTOR
__interrupt void Uart0RxISR(void) 
{
 
  uint8 tmp;
  
  tmp = U0RXBUF;
  
  //clr and stop 超时 计时器
  spUart0TimerOutTimerB();
  
  if (u8RxbufStat != RXBUF_RCVING)
    return;
  if ( i == 0 )     //接收到帧头
  {
    if(tmp != 0x5a) //判断帧头是否正确
      return;
    u8Rxbuf0[i++] = tmp; //正确接收到帧头
                         //start 超时 计时器
    stUart0TimeOutTimerB(10);
    return;
  }
  
  
  
  if( i == 1 )    //接收到帧长度
  {
    if(tmp > RXBUFSIZE )  //缓冲不够
    { i=0; return;}
    u8Rxbuf0cnt = tmp;      //缓冲 够
    u8Rxbuf0[i++] = tmp;
                            //start 超时 计时器
    stUart0TimeOutTimerB(10);
    return;
  }
  
  if( i < u8Rxbuf0cnt-1 )//存储帧数据
  {
    u8Rxbuf0[i++] = tmp;
                            //start  超时 计时器
    stUart0TimeOutTimerB(10);
    return;
  }  
  
  if( i >= u8Rxbuf0cnt-1 ) //收到帧结尾
  {
    u8Rxbuf0[i] = tmp;
    for(i=0,tmp=0; i<u8Rxbuf0cnt-1; i++)
    {
      tmp += u8Rxbuf0[i];   //计算校验和
    }
    
    if(tmp != u8Rxbuf0[u8Rxbuf0cnt-1] ) //校验和错误
    {
      i=0;
      u8Rxbuf0cnt = 0;
      
      return;
    }
  
  }
    
  //成功接收一帧数据   
  i = 0;  
  IE1 &= ~UTXIE0;
  U0TXBUF = 0x5b;         //回送 "0x5B"
  while(! (IFG1 & UTXIFG0) );
  IFG1 &= ~ UTXIFG0;
  IE1 |= UTXIE0;
  
  //向主程序 发送消息.
  u8RxbufStat = RXBUF_DEALING;
  
  
  
   
}





/*******************************************
函数名称：stUart0TimeOutTimerB(uint16 TimeOutMs )
功    能：启动 uart0 超时计时器 
参    数：uint16  TimeOutMs 超时毫秒数
返回值  ：无
说    明：定时器 TBCLK 来自 ACLK ，能在 LP3 下 计时
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
函数名称：spUart0TimeOutTimerB(void )
功    能：停止uart0 超时计时
参    数：
返回值  ：无
说    明：在超时时间内 接收完 下一字节 调用该函数
********************************************/
static void spUart0TimerOutTimerB(void)
{
  TBCCTL0 &= ~CCIE;
  TBCTL |= MC_0; 
}

/*******************************************
函数名称：Uart0TimerOutedISR(void )
功    能：停止uart0 超时计时
参    数：
返回值  ：无
说    明：超时时间到
********************************************/
#pragma vector = TIMERB0_VECTOR
__interrupt void Uart0TimerOutedISR(void)
{
  i = 0;
  u8Rxbuf0cnt = 0;  
  spUart0TimerOutTimerB();
}