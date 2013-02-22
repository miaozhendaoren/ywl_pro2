#define LCD1602
#include "alldefs.h"  
#include "msp430f149.h"
#include "BoardConfig.h"
#include "Lcd1602.h"
#include "Lib.h"


void Lcd1602Reset(void);
void Lcd1602Clr(void);
void Lcd1602CursorOn(uint8 u8CurOn);
void Lcd1602LocateXY(uint8 u8Xpos, uint8 u8Ypos);
void Lcd1602PutCharXY(uint8 u8Xpos, uint8 u8Ypo, uint8 u8Char);
void Lcd1602PutStrXY(uint8 u8Xpos, uint8 u8Ypos, uint8 *u8pStr);
void Lcd1602PutNCharXY(uint8 u8Xpos,uint8 u8Ypos, uint8 u8CharNum, uint8 *pu8Str);
void Lcd1602CursorOn(uint8 u8CurOn);

static void Lcd1602PutChar(uint8 u8Char);
static void LcdWriteCmd(uint8 Cmd, uint8 Chk);
static void LcdWriteDat(uint8 Dat);
static void LcdWaitForEnable(void);

/*******************************************
�������ƣ�Lcd1602Reset
��    �ܣ���1602Һ��ģ����и�λ����
��    ������
����ֵ  ����
********************************************/
void Lcd1602Reset(void) 
{
    LcdCtlOut;                       //�����߶˿���Ϊ���״̬ 
    LcdDatOutCtl;                    //���ݶ˿���Ϊ���״̬  
  
    LcdWriteCmd(0x38, 0);	    //�涨�ĸ�λ����
    delay_ms(5);
    LcdWriteCmd(0x38, 0);		
    delay_ms(5);
    LcdWriteCmd(0x38, 0);
    delay_ms(5);

    LcdWriteCmd(0x38, 1);		//��ʾģʽ����
    LcdWriteCmd(0x08, 1);		//��ʾ�ر�
    LcdWriteCmd(0x01, 1);	        //��ʾ����
    LcdWriteCmd(0x06, 1);		//д�ַ�ʱ���岻�ƶ�
    LcdWriteCmd(0x0c, 1);		//��ʾ���������α꣬����˸
}
/*******************************************
�������ƣ�Lcd1602Clr
��    �ܣ�LCD1602 ������ʾ
��    ������
����ֵ  ����
********************************************/
void Lcd1602Clr(void) 
{
  LcdWriteCmd(0x01,1);
}

/*******************************************
�������ƣ�Lcd1602Cursor( uint8 u8CurOn)
��    �ܣ�1602 �����ʾ ����
��    ����u8CurOnΪ0 ���أ�����
����ֵ  ����
********************************************/
void Lcd1602CursorOn(uint8 u8CurOn) 
{
  if(u8CurOn)  LcdWriteCmd(0x0f,1);
  else         LcdWriteCmd(0x0c,1);
}

/*******************************************
�������ƣ�Lcd1602PutCharXY(uint8 u8Xpos, uint8 u8Ypos, uint8 u8Char)
��    �ܣ�����Ļ X,Y λ����ʾһ���ַ�
��    ����u8Xpos,u8Ypos,λ������  u8Char Ҫ��ʾ���ַ�
����ֵ  ����
********************************************/
void Lcd1602PutCharXY(uint8 u8Xpos, uint8 u8Ypos, uint8 u8Char) 
{
  Lcd1602LocateXY(u8Xpos, u8Ypos);
  LcdWriteDat(u8Char);  
}


/*******************************************
�������ƣ�Lcd1602PutStrXY(uint8 u8Xpos, uint8 u8Ypos, uint8 *u8pStr)
��    �ܣ�����Ļ X,Y λ����ʾһ���ַ�
��    ����u8Xpos,u8Ypos,λ������  uint8 *u8pStr �ַ���ָ��
����ֵ  ����
********************************************/
void Lcd1602PutStrXY(uint8 u8Xpos, uint8 u8Ypos, uint8 *u8pStr)
{
  
  Lcd1602LocateXY(u8Xpos, u8Ypos);
  
  while(*u8pStr!= '\0'){                  //��'\0'�綨�ַ������� 
    LcdWriteDat( *u8pStr++ );
    u8Xpos++;
    if(u8Xpos > 15)                       //�ж��ַ����Ƿ񳬳���һ��
    {
      u8Xpos = 0;
      u8Ypos++;
      Lcd1602LocateXY(u8Xpos,u8Ypos);
    }    
  }
  
}


/*******************************************
�������ƣ�Lcd1602PutNCharXY(uint8 u8Xpos,uint8 u8Ypos, uint8 u8CharNum, uint8 *pu8Str)
��    �ܣ�����Ļ X,Y λ����ʾ N ���ַ�
��    ����u8Xpos,u8Ypos,λ������   u8CharNum,�ַ�����  uint8 *u8pStr �ַ���ָ��
����ֵ  ����
********************************************/
void Lcd1602PutNCharXY(uint8 u8Xpos,uint8 u8Ypos, uint8 u8CharNum, uint8 *pu8Str)
{
  
  Lcd1602LocateXY(u8Xpos, u8Ypos);

    
  while( u8CharNum-- > 0)
  {                  
    LcdWriteDat( *pu8Str++ );
    u8Xpos++;
    if(u8Xpos > 15)                       //�ж��ַ����Ƿ񳬳���һ��
    {
      u8Xpos = 0;
      u8Ypos++;
      Lcd1602LocateXY(u8Xpos,u8Ypos);
    }    
  }
  
}

/*******************************************
�������ƣ�Lcd1602LocateXY
��    �ܣ���Һ��������ʾ�ַ�λ�õ�������Ϣ
��    ����x--λ�õ�������
          y--λ�õ�������
����ֵ  ����
********************************************/
void Lcd1602LocateXY(uint8 x,uint8 y) 
{
    uint8 temp;

    temp = x&0x0f;
    y &= 0x01;
    if(y)   temp |= 0x40;  //����ڵ�2��
    temp |= 0x80;

    LcdWriteCmd(temp,1);
}

/*******************************************
�������ƣ�LcdWriteCmd
��    �ܣ���Һ��ģ��д������
��    ����Cmd--���
          Chk--�Ƿ���æ�ı�־��1����æ��0������
����ֵ  ����
********************************************/
static void LcdWriteCmd(uint8 Cmd, uint8 Chk)
{
    if (Chk) LcdWaitForEnable();   // ���æ�ź�?
    
    Lcd_RS0;	
    Lcd_RW0; 
    _NOP();

    LcdDatOut = Cmd;             //��������д�����ݶ˿� 
    _NOP();					
    
    Lcd_E1;                     //����ʹ�������ź�
    _NOP();
    _NOP();
    Lcd_E0;			
}

/*******************************************
�������ƣ�LcdWriteDat
��    �ܣ���Һ����ʾ�ĵ�ǰ��ַд����ʾ����
��    ����Dat--��ʾ�ַ�����
����ֵ  ����
********************************************/
static void LcdWriteDat(uint8 Dat)
{
    LcdWaitForEnable();        //�ȴ�Һ����æ

    Lcd_RS1;
    Lcd_RW0; 
    _NOP();

    LcdDatOut = Dat;        //����ʾ����д�����ݶ˿�
    _NOP();

    Lcd_E1;                 //����ʹ�������ź�
    _NOP(); 
    _NOP(); 
    Lcd_E0;		
}


/*******************************************
�������ƣ�LcdWaitForEnable
��    �ܣ��ȴ�1602Һ������ڲ�����
��    ������
����ֵ  ����
********************************************/
static void LcdWaitForEnable(void)
{
  LcdDatInCtl;  //��P4���л�Ϊ����״̬

  Lcd_RS0;
  Lcd_RW1;
  _NOP();
  Lcd_E1;
  _NOP();
  _NOP();
	
  while((LcdDatIn & 0x80)!=0);  //���æ��־

  Lcd_E0;
  
  LcdDatOutCtl;  //��P4���л�Ϊ���״̬
  

}

