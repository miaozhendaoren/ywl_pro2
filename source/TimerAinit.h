#ifndef _TIMERAINIT_H
#define _TIMERAINIT_H  
                                            //ȫ�ֺ궨�忪ʼ-------------

/******************************************************************************/
#ifdef  TIMERAINIT                          //�ڲ�����������ʼ-----------


/*************************************************************************/
#else                                       //�ⲿ���ÿ�ʼ---------

extern u_int16 u16start_CCR2;
extern u_int16 u16end_CCR2;
extern u_int16 u16ovCnt_TAR;
extern u_int8  u8isMeasuring;
extern void TimerA_reset(void);
extern void TimerA_stop(void);
#endif
#endif