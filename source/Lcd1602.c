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
函数名称：Lcd1602Reset
功    能：对1602液晶模块进行复位操作
参    数：无
返回值  ：无
********************************************/
void Lcd1602Reset(void) 
{
    LcdCtlOut;                       //控制线端口设为输出状态 
    LcdDatOutCtl;                    //数据端口设为输出状态  
  
    LcdWriteCmd(0x38, 0);	    //规定的复位操作
    delay_ms(5);
    LcdWriteCmd(0x38, 0);		
    delay_ms(5);
    LcdWriteCmd(0x38, 0);
    delay_ms(5);

    LcdWriteCmd(0x38, 1);		//显示模式设置
    LcdWriteCmd(0x08, 1);		//显示关闭
    LcdWriteCmd(0x01, 1);	        //显示清屏
    LcdWriteCmd(0x06, 1);		//写字符时整体不移动
    LcdWriteCmd(0x0c, 1);		//显示开，不开游标，不闪烁
}
/*******************************************
函数名称：Lcd1602Clr
功    能：LCD1602 清屏显示
参    数：无
返回值  ：无
********************************************/
void Lcd1602Clr(void) 
{
  LcdWriteCmd(0x01,1);
}

/*******************************************
函数名称：Lcd1602Cursor( uint8 u8CurOn)
功    能：1602 光标显示 开关
参    数：u8CurOn为0 光标关，否则开
返回值  ：无
********************************************/
void Lcd1602CursorOn(uint8 u8CurOn) 
{
  if(u8CurOn)  LcdWriteCmd(0x0f,1);
  else         LcdWriteCmd(0x0c,1);
}

/*******************************************
函数名称：Lcd1602PutCharXY(uint8 u8Xpos, uint8 u8Ypos, uint8 u8Char)
功    能：在屏幕 X,Y 位置显示一个字符
参    数：u8Xpos,u8Ypos,位置坐标  u8Char 要显示的字符
返回值  ：无
********************************************/
void Lcd1602PutCharXY(uint8 u8Xpos, uint8 u8Ypos, uint8 u8Char) 
{
  Lcd1602LocateXY(u8Xpos, u8Ypos);
  LcdWriteDat(u8Char);  
}


/*******************************************
函数名称：Lcd1602PutStrXY(uint8 u8Xpos, uint8 u8Ypos, uint8 *u8pStr)
功    能：在屏幕 X,Y 位置显示一个字符
参    数：u8Xpos,u8Ypos,位置坐标  uint8 *u8pStr 字符串指针
返回值  ：无
********************************************/
void Lcd1602PutStrXY(uint8 u8Xpos, uint8 u8Ypos, uint8 *u8pStr)
{
  
  Lcd1602LocateXY(u8Xpos, u8Ypos);
  
  while(*u8pStr!= '\0'){                  //以'\0'界定字符串结束 
    LcdWriteDat( *u8pStr++ );
    u8Xpos++;
    if(u8Xpos > 15)                       //判断字符数是否超出第一行
    {
      u8Xpos = 0;
      u8Ypos++;
      Lcd1602LocateXY(u8Xpos,u8Ypos);
    }    
  }
  
}


/*******************************************
函数名称：Lcd1602PutNCharXY(uint8 u8Xpos,uint8 u8Ypos, uint8 u8CharNum, uint8 *pu8Str)
功    能：在屏幕 X,Y 位置显示 N 个字符
参    数：u8Xpos,u8Ypos,位置坐标   u8CharNum,字符个数  uint8 *u8pStr 字符串指针
返回值  ：无
********************************************/
void Lcd1602PutNCharXY(uint8 u8Xpos,uint8 u8Ypos, uint8 u8CharNum, uint8 *pu8Str)
{
  
  Lcd1602LocateXY(u8Xpos, u8Ypos);

    
  while( u8CharNum-- > 0)
  {                  
    LcdWriteDat( *pu8Str++ );
    u8Xpos++;
    if(u8Xpos > 15)                       //判断字符数是否超出第一行
    {
      u8Xpos = 0;
      u8Ypos++;
      Lcd1602LocateXY(u8Xpos,u8Ypos);
    }    
  }
  
}

/*******************************************
函数名称：Lcd1602LocateXY
功    能：向液晶输入显示字符位置的坐标信息
参    数：x--位置的列坐标
          y--位置的行坐标
返回值  ：无
********************************************/
void Lcd1602LocateXY(uint8 x,uint8 y) 
{
    uint8 temp;

    temp = x&0x0f;
    y &= 0x01;
    if(y)   temp |= 0x40;  //如果在第2行
    temp |= 0x80;

    LcdWriteCmd(temp,1);
}

/*******************************************
函数名称：LcdWriteCmd
功    能：向液晶模块写入命令
参    数：Cmd--命令，
          Chk--是否判忙的标志，1：判忙，0：不判
返回值  ：无
********************************************/
static void LcdWriteCmd(uint8 Cmd, uint8 Chk)
{
    if (Chk) LcdWaitForEnable();   // 检测忙信号?
    
    Lcd_RS0;	
    Lcd_RW0; 
    _NOP();

    LcdDatOut = Cmd;             //将命令字写入数据端口 
    _NOP();					
    
    Lcd_E1;                     //产生使能脉冲信号
    _NOP();
    _NOP();
    Lcd_E0;			
}

/*******************************************
函数名称：LcdWriteDat
功    能：向液晶显示的当前地址写入显示数据
参    数：Dat--显示字符数据
返回值  ：无
********************************************/
static void LcdWriteDat(uint8 Dat)
{
    LcdWaitForEnable();        //等待液晶不忙

    Lcd_RS1;
    Lcd_RW0; 
    _NOP();

    LcdDatOut = Dat;        //将显示数据写入数据端口
    _NOP();

    Lcd_E1;                 //产生使能脉冲信号
    _NOP(); 
    _NOP(); 
    Lcd_E0;		
}


/*******************************************
函数名称：LcdWaitForEnable
功    能：等待1602液晶完成内部操作
参    数：无
返回值  ：无
********************************************/
static void LcdWaitForEnable(void)
{
  LcdDatInCtl;  //将P4口切换为输入状态

  Lcd_RS0;
  Lcd_RW1;
  _NOP();
  Lcd_E1;
  _NOP();
  _NOP();
	
  while((LcdDatIn & 0x80)!=0);  //检测忙标志

  Lcd_E0;
  
  LcdDatOutCtl;  //将P4口切换为输出状态
  

}

