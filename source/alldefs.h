#ifndef _ALLDEFS_H
#define _ALLDEFS_H

/* 类型定义 */


typedef  unsigned char u_int8;
typedef  unsigned int  u_int16;
typedef  unsigned long u_int32;
typedef  signed char  s_int8;
typedef  signed int s_int16;
typedef  signed long  s_int32;

typedef unsigned char uint8;
typedef unsigned int  uint16;

/* 精确延时宏定义 */
#define CPU_F   ((double)8000000)
#define delay_us(x)   __delay_cycles( ( long)(CPU_F*(double)x/1000000.0) )
#define delay_ms(x)   __delay_cycles( ( long)(CPU_F*(double)x/1000.0) )





#endif