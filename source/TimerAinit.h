#ifndef _TIMERAINIT_H
#define _TIMERAINIT_H  
                                            //全局宏定义开始-------------

/******************************************************************************/
#ifdef  TIMERAINIT                          //内部定义声明开始-----------


/*************************************************************************/
#else                                       //外部引用开始---------

extern u_int16 u16start_CCR2;
extern u_int16 u16end_CCR2;
extern u_int16 u16ovCnt_TAR;
extern u_int8  u8isMeasuring;
extern void TimerA_reset(void);
extern void TimerA_stop(void);
#endif
#endif