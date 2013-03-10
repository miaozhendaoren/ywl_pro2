#define PCF8562
#include "msp430f149.h"
#include "alldefs.h"
#include "I2C.h"
#include "PCF8562.h"


#define IICADDR8562    0x70      //8562 IIC ��ַ 0x70

/**************************************************************
���ƣ�u_int8 PCF8562_init(void)
���ܣ�PCF8562��ʼ������̬,1/2ƫ��,��ʾ,�ɹ�����1
**************************************************************/
u_int8  PCF8562_init(void)
{ 
    //��ʽ�趨: ������ʾ��ƫ��1/2����̬
    //0 1 0 0 1 1  0  1
    //C 1 0 x E B M1 M0
  
  u_int8 initCode[2] = {IICADDR8562, 0x4d};
  if( ISendStr(IICADDR8562,0x4d,&initCode[0],0) )
    return (1);
  else
  {//    while(1);
       return (0);
  }
}

/**************************************************************
���ƣ�u_int8  PCF8562_disStr(u_int8 *buf, u_int8 start_addr u_int8 num )
���ܣ�PCF8562��ʾ�ַ���, start_addr ΪPCF8562��ʼ��ʾ��bitλ���, numΪҪ��ʾ���ֽڸ���
���أ� �ɹ�����1�����򷵻�0
**************************************************************/
u_int8  PCF8562_disStr(u_int8 *buf, u_int8 start_addr, u_int8 num  )
{
  start_addr = start_addr & 0x3f; //�����λ C 0 P5 P4 P3 P2 P1 P0
  if( ISendStr(IICADDR8562, start_addr, buf, num) )
    return (1);
  else
    return (0);
}

