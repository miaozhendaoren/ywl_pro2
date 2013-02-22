#ifndef _BOARDCONFIG_H
#define _BOARDCONIFG_H  
                                            //全局宏定义开始-------------
// 注意： 8LED 和 2个7SEG_LED 不能同时使用，有干涉
#define BIT_RESET         0x7f    //复位值,所有都关闭
#define BIT_4245_OE       0x01    //4245_OE 低电平有效
#define BIT_4245_DIR      0x02    //4245_DIR  高电平: 5->3.3   低电平: 3.3->5
#define BIT_7SEGLED573_OC 0x0c    //7SEG_LED573_OC 低电平有效
#define BIT_8LED573_OE    0x10    //8LED573_OE  低电平有效
#define BIT_12864PSB      0x20    //12864PSB 高电平: 数据传输为并口  低电平:数据传输为串口
#define BIT_12864RST      0x40    //低电平有效 12864 进入复位状态
/******************************************************************************/
#ifdef  BOARDCONFIG                         //内部定义声明开始-----------

#define CtlOut P3DIR |= BIT3 + BIT6 + BIT7
#define Ctl_0  P3OUT &= ~(BIT3 + BIT6 + BIT7)
#define SER_0  P3OUT &= BIT6
#define SER_1  P3OUT |= BIT6
#define RCLK_0 P3OUT &= BIT3
#define RCLK_1 P3OUT |= BIT3
#define SRCLK_0 P3OUT &= BIT7
#define SRCLK_1 P3OUT |= BIT7


/*************************************************************************/
#else                                       //外部引用开始---------

extern uint8 u8BoardCTL;
extern void ConfigBoard(uint8 u8BoardCtl);



#endif
#endif