/* ����������  ͬʱ  �� ��ʱ�� ���õĳ��� */
#define FM24CL64
#include "alldefs.h"              
#include "msp430f149.h"
#include "i2c.h"
#include "FM24CL64.h"

#define IICADDR24CL64   0x01

/**************************************************************
���ƣ�u_int8   Read_FRAM(u_int16 start_addr, u_int8* pbuf, u_int8 num)
���ܣ���FRAM  start_addr  ��ʼ�� num ���ֽ� �� pbufָ��Ļ�������
����: �������ֽڸ���
**************************************************************/
u_int8   Read_FRAM(u_int16 start_addr, u_int8* pbuf, u_int8 num)
{
  u_int8 i = 0;  
  u_int8 ack;
  
  ack = 0;                             //Ӧ����Ϊ0
  Start_I2c();                         //��������
  SendByte(IICADDR24CL64&0xfe,&ack);   //����FRAM�ӻ���ַ-д
  if(ack==0)return(0);
  
  SendByte((u_int8)(start_addr>>8),&ack); //д�����ӵ�ַ-���ֽ�
  if(ack==0)return(0);
  SendByte((u_int8)(start_addr&0x00ff), &ack);  
  if(ack==0)return(0);                 //д�����ӵ�ַ-���ֽ�
 
  Start_I2c();                         //��������
  SendByte(IICADDR24CL64|0x01,&ack);   //����FRAM�ӻ���ַ-��
  if(ack==0)return(0);
  
  for(i=0;i<num-1;i++)
  {   
    *pbuf=RcvByte();                /*��������*/
     Ack_I2c(0);                 /*���;ʹ�λ*/  
    pbuf++;
  } 
  *pbuf=RcvByte();
  Ack_I2c(1);                    /*���ͷ�Ӧλ*/
  Stop_I2c();                    /*��������*/ 
  return(i);
}


/**************************************************************
���ƣ�u_int8 Write_FRAM( u_int16 start_addr, const u_int8* pbuf, u_int8 num )
���ܣ�дpbufָ��Ļ����� num���ֽ� �� FRAM start_addr ��ʼ��
����������д��ĸ���
**************************************************************/
u_int8 Write_FRAM(u_int16 start_addr, const u_int8* pbuf, u_int8 num)
{
  u_int8 i = 0;
  u_int8 ack;
    
  ack = 0;                              //��ʼ��Ϊ��Ӧ��  
  Start_I2c();                          //��������

  SendByte(IICADDR24CL64 & 0xfe,&ack);  //����FRAM�ӻ���ַ-д
  if(ack==0)return(0);
  SendByte( (u_int8)(start_addr>>8), &ack); //����FRAM�洢�ռ��ַ-���ֽ�
  if(ack==0)return(0);
  SendByte( (u_int8)(start_addr&0x00ff), &ack); //����FRAM�洢�ռ��ַ-���ֽ�
  if(ack==0)return(0);
  
  for(i=0;i<num;i++)
  {   
    SendByte(*pbuf,&ack);             /*��������*/
    if(ack==0)return(0);
    pbuf++;
  } 
  Stop_I2c();                 /*��������*/ 
  
  return(i);  
}


