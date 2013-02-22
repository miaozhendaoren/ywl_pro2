#ifndef _BOARDCONFIG_H
#define _BOARDCONIFG_H  
                                            //ȫ�ֺ궨�忪ʼ-------------
// ע�⣺ 8LED �� 2��7SEG_LED ����ͬʱʹ�ã��и���
#define BIT_RESET         0x7f    //��λֵ,���ж��ر�
#define BIT_4245_OE       0x01    //4245_OE �͵�ƽ��Ч
#define BIT_4245_DIR      0x02    //4245_DIR  �ߵ�ƽ: 5->3.3   �͵�ƽ: 3.3->5
#define BIT_7SEGLED573_OC 0x0c    //7SEG_LED573_OC �͵�ƽ��Ч
#define BIT_8LED573_OE    0x10    //8LED573_OE  �͵�ƽ��Ч
#define BIT_12864PSB      0x20    //12864PSB �ߵ�ƽ: ���ݴ���Ϊ����  �͵�ƽ:���ݴ���Ϊ����
#define BIT_12864RST      0x40    //�͵�ƽ��Ч 12864 ���븴λ״̬
/******************************************************************************/
#ifdef  BOARDCONFIG                         //�ڲ�����������ʼ-----------

#define CtlOut P3DIR |= BIT3 + BIT6 + BIT7
#define Ctl_0  P3OUT &= ~(BIT3 + BIT6 + BIT7)
#define SER_0  P3OUT &= BIT6
#define SER_1  P3OUT |= BIT6
#define RCLK_0 P3OUT &= BIT3
#define RCLK_1 P3OUT |= BIT3
#define SRCLK_0 P3OUT &= BIT7
#define SRCLK_1 P3OUT |= BIT7


/*************************************************************************/
#else                                       //�ⲿ���ÿ�ʼ---------

extern uint8 u8BoardCTL;
extern void ConfigBoard(uint8 u8BoardCtl);



#endif
#endif