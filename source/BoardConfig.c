/* ����������  ͬʱ  �� ��ʱ�� ���õĳ��� */
#define BOARDCONFIG
#include "alldefs.h"              
#include "msp430f149.h"
#include "BoardConfig.h"

void ConfigBoard(uint8 u8BoardCtl);
                             // ������ ���� ���������ô˺��� ���� ������

void ConfigBoard(uint8 u8BoardCtl)
{
  uint8 i;

  CtlOut;
  Ctl_0;
  for(i=0; i<8; i++)
  {
    SRCLK_0;
    if(u8BoardCtl & 0x80) SER_1;
    else                 SER_0;
    SRCLK_1;
    u8BoardCtl <<= 1;
  }
  RCLK_1;
  _NOP();
  RCLK_0;
}
