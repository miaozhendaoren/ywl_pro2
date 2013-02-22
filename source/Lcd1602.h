#ifndef _LCD1602_H
#define _LCD1602_H  
                                            //全局宏定义开始-------------

/******************************************************************************/
#ifdef  LCD1602                             //内部定义声明开始-----------




#define LcdCtlOut P3DIR |= BIT0 + BIT1 + BIT2
#define Lcd_RS0   P3OUT &= ~BIT0
#define Lcd_RS1   P3OUT |= BIT0
#define Lcd_RW0   P3OUT &= ~BIT1
#define Lcd_RW1   P3OUT |= BIT1
#define Lcd_E0    P3OUT &= ~BIT2
#define Lcd_E1    P3OUT |= BIT2

#define LcdDatOutCtl P4DIR = 0xff
#define LcdDatInCtl  P4DIR = 0x00
#define LcdDatOut P4OUT
#define LcdDatIn  P4IN


/*************************************************************************/
#else                                       //外部引用开始---------
extern  void Lcd1602Reset(void);
extern  void Lcd1602Clr(void);
extern  void Lcd1602LocateXY(uint8 u8Xpos, uint8 uYpos);
extern  void Lcd1602PutCharXY(uint8 u8Xpo, uint8 u8Ypos, uint8 u8Char);
extern  void Lcd1602PutStrXY(uint8 u8Xpos, uint8 u8Ypos, uint8 *pu8Str);
extern  void Lcd1602PutNCharXY(uint8 u8Xpos,uint8 u8Ypos, uint8 u8CharNum, uint8 *pu8Str);
extern  void Lcd1602CursorOn(uint8 u8CurOn);



#endif
#endif