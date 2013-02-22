#ifndef _COM_H
#define _COM_H  
                                            //全局宏定义开始-------------
#define RXBUFSIZE     20
#define TXBUFSIZE     20
#define RXBUF_RCVING   0x12
#define RXBUF_DEALING  0x21
#define TXBUF_SNDING   0x13
#define TXBUF_IDLING   0x31

/******************************************************************************/
#ifdef  COM                                //内部定义声明开始-----------


/*************************************************************************/
#else                                       //外部引用开始---------

extern volatile uint8 u8RxbufStat;
extern uint8 u8Rxbuf0[RXBUFSIZE];
extern void Uart0Init(void);
extern void Uart0FrameSnd(uint8 u8DatLen, uint8 *u8pDat);
extern void Uart0StrSnd(uint8 *u8Str);
extern void Uart0CharSnd(uint8 u8Char);
extern volatile uint8 u8TxbufStat;
extern void Uart0NCharSnd(uint8 *Str, uint8 charNum);
#endif
#endif