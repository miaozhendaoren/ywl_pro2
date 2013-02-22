#define LIB
#include "alldefs.h"
#include "msp430f149.h"
#include "Lib.h"
#include "math.h"

uint8 *pu8Float2Str( float fValue, uint8 u8DotLen);


/*******************************************
�������ƣ�uint8 *pu8Float2Str( float fValue, uint8 u8DotLen)
��    �ܣ���������ת��Ϊ�ַ��� (�ַ�����9Byte)
��    ����
          fValue  Ҫת���ĸ�����
          u8DotLen  С����λ��
����ֵ  : uint8 * ����,ָ���ַ�����ָ��
��    ע����ʾ���ţ�С����0,1��2��3λ����λ���4λ
          �ַ�����ʽΪ�� ����-100.999\0, �ַ�������10
                         ��Ч�ַ��ӷ� '?'��ʼ����'\0'����
��ʾ��Χ: -9999.999 ~ 9999.999
********************************************/
  uint8 *pu8Float2Str( float fValue, uint8 u8DotLen)
{
    unsigned long ul_i;
    uint8 i;
    uint8 u8IsNegative;
    
    //debug
    static uint8 u8Str[10] = {'?','?','?','?','?','?','?','?','?','\0'};    //�ַ�������10
    //debug_end
    uint8 u8Dec[7] = {0};
    
    if(fValue < -0.0001 )
      u8IsNegative = 1;
    else
      u8IsNegative = 0;
    
    fValue = fabs(fValue);            
      
    // ����Χ����
    
    if(u8DotLen == 3)
    {
      ul_i = (unsigned long ) (fValue * 1000.0);
    }else if(u8DotLen == 2)
    {
      ul_i = (unsigned long ) (fValue * 100.0);
    }else if(u8DotLen == 1)
    {
      ul_i = (unsigned long ) (fValue * 10.0);
      
    }else if(u8DotLen == 0)
    {
      ul_i = (unsigned long ) fValue;    
    }
        
    u8Dec[6] = ul_i % 10l;                     //9999999
    ul_i = ul_i / 10l;                         //123456^

    u8Dec[5] = ul_i % 10l;                     //9999999
    ul_i = ul_i / 10l;                        //12345^7
    
    u8Dec[4] = ul_i % 10;                     //9999999
    ul_i = ul_i / 10l;                        //1234^67
    
    u8Dec[3] = ul_i % 10;                     //9999999
    ul_i = ul_i / 10l;                      //123^567
    
    u8Dec[2] = ul_i % 10;                     //9999999
    ul_i = ul_i / 10l;                     //12^4567
    
    u8Dec[1] = ul_i % 10;                     //9999999
    ul_i = ul_i / 10l;                    //1^34567
    
    u8Dec[0] = ul_i % 10;                     //9999999
                                                //^234567
    i = 6;
    u8Str[8] = '0' + u8Dec[i--];

    if(u8DotLen == 1)
    {
      u8Str[7] = '.';
    }else{
      u8Str[7] = '0' + u8Dec[i--];
    }
    
    if(u8DotLen == 2)
    {
      u8Str[6] = '.';      
    }else{
      u8Str[6] = '0' + u8Dec[i--];
    }
    
    if(u8DotLen == 3)
    {
      u8Str[5] = '.';
    }else{
      u8Str[5] = '0' + u8Dec[i--];
    }
    
    u8Str[4] = '0' + u8Dec[i--];
    
    u8Str[3] = '0' + u8Dec[i--];
    
    if(u8DotLen != 0)
      u8Str[2] = '0' + u8Dec[i--];

    if(u8DotLen != 0)
      u8Str[1] = '0' + u8Dec[i];

    //����ǰ��0
    
    i = 1;
    while( u8Str[i] == '?' ) { i++; }    //�ӵ�һ������λ��ʼ ������ͷ�� '��'
    
    while( u8Str[i] == '0' ){            //�ӵ�һ��0��ʼֱ��һ����0����

      if( i == 8 )                       //��ǰ�����һλ
        break;  
      
      if ( u8Str[i+1] != '.' )          //��һλ����С���㣬�滻Ϊ '?'
        u8Str[i++] = '?';
      else
        break;
    }
          
    //��Ӹ���
    if( u8IsNegative )
    {
      u8Str[i-1] = '-';
      return &u8Str[i-1];
    }
    else
    {
      return &u8Str[i];
    }

}


/*******************************************
�������ƣ�uint8 *Bytes2Str()
��    �ܣ���������N���ֽ� ת��Ϊ�������ַ���,���ڽ�β��� '\0'
��    ����Bytes Ҫת�����ֽ�
          Str ת������ַ��� 
          ByteNum Ҫת�����ֽڸ���
����ֵ  : ת���˵��ֽڸ���
��    ע��
********************************************/
uint8  Bytes2Str( uint8 *Bytes, uint8 *Str, uint8 ByteNum )
{
  uint8 i=0;
  
  
  return i;
}

/*** ��������: uint8 ul2str(u_int32 u32dat, u_int8 str[]) 
**** ��    ��: ת��������(32λ)Ϊ�ַ������ͣ��Զ����'\0'
***  ��    ��: 
***  ��  ��ֵ��
***/
uint8  ul2str(u_int32 u32dat, u_int8 str[])
{
  u_int8 i;
  u_int32 u32j;

  u32j = u32dat;
  for(i=0; i<9; i++ )
  {
   u32j /= 10;
   if( 0 == u32j)
     break;
  }
  
  str[i] = '\0';
  for( ;i>0; i--)
  {
    if( u32dat < 10 && 0 != u32dat )
    {
        str[i] = '0' + u32dat;    
        break;
    }
    str[i] = '0' + u32dat%10;
    u32dat /= 10;
    if(0 == u32dat)
      str[i] = '0' + 0;
  }  
  return 1;
  
}