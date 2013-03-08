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
    //��ʼ��iicӲ���˿�
    iic_init();

    iic_start();
    //���� PCF8562 IIC��ַ
    iic_SndByte(IICADDR8562);
    
    if( 0 == iic_isSlaveAck() )
    {//PCF8562����Ӧ
      iic_stop();
      return (0);
    }
    
    //��ʽ�趨: ������ʾ��ƫ��1/2����̬
    iic_SndByte(0x4d);  //0 1 0 0 1 1  0  1
                        //C 1 0 x E B M1 M0
    if( 0 == iic_isSlaveAck() )
    {
      iic_stop();
      return(0);
    }
    
    iic_stop();
    return(1);
}

/**************************************************************
���ƣ�u_int8  PCF8562_disStr(u_int8 *buf, u_int8 num )
���ܣ�PCF8562��ʾ�ַ���,����PCF8562���յ��ֽڸ���,numΪ�ֽڸ�����
              bufΪ��ʾ������
**************************************************************/
u_int8  PCF8562_disStr(u_int8 *buf, u_int8 num )
{
  u_int8 i = 0;

  if( num > 4 )
      return 0;
  
    iic_init();
    iic_start();
    iic_SndByte(IICADDR8562);
    if( 0 == iic_isSlaveAck() )
    {//PCF8562����Ӧ
      iic_stop();
      return (i);
    }

    iic_SndByte(0x00);  //�ӵ�ַָ��0��ʼ
    if( 0 == iic_isSlaveAck() )
    {//PCF8562����Ӧ
      iic_stop();
      return (i);
    }        
    for( ; num; num--)
    {
      iic_SndByte(buf[i]);
      if( 0 == iic_isSlaveAck() )
      {
        iic_stop();
        return(i);
      }
      i++;
    }
    
    iic_stop();
    return (i);   
}

