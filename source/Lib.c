#define LIB
#include "alldefs.h"
#include "msp430f149.h"
#include "Lib.h"
#include "math.h"

uint8 *pu8Float2Str( float fValue, uint8 u8DotLen);


/*******************************************
函数名称：uint8 *pu8Float2Str( float fValue, uint8 u8DotLen)
功    能：将浮点数转换为字符串 (字符串长9Byte)
参    数：
          fValue  要转换的浮点数
          u8DotLen  小数点位数
返回值  : uint8 * 类型,指向字符串的指针
备    注：显示符号，小数点0,1，2，3位，高位最多4位
          字符串格式为： ？？-100.999\0, 字符串长度10
                         有效字符从非 '?'开始，以'\0'结束
显示范围: -9999.999 ~ 9999.999
********************************************/
  uint8 *pu8Float2Str( float fValue, uint8 u8DotLen)
{
    unsigned long ul_i;
    uint8 i;
    uint8 u8IsNegative;
    
    //debug
    static uint8 u8Str[10] = {'?','?','?','?','?','?','?','?','?','\0'};    //字符串长度10
    //debug_end
    uint8 u8Dec[7] = {0};
    
    if(fValue < -0.0001 )
      u8IsNegative = 1;
    else
      u8IsNegative = 0;
    
    fValue = fabs(fValue);            
      
    // 超范围处理
    
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

    //消除前导0
    
    i = 1;
    while( u8Str[i] == '?' ) { i++; }    //从第一个数字位开始 跳过开头的 '？'
    
    while( u8Str[i] == '0' ){            //从第一个0开始直到一个非0的数

      if( i == 8 )                       //当前是最后一位
        break;  
      
      if ( u8Str[i+1] != '.' )          //下一位不是小数点，替换为 '?'
        u8Str[i++] = '?';
      else
        break;
    }
          
    //添加负号
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
函数名称：uint8 *Bytes2Str()
功    能：将连续的N个字节 转换为连续的字符串,并在结尾添加 '\0'
参    数：Bytes 要转换的字节
          Str 转换后的字符串 
          ByteNum 要转换的字节个数
返回值  : 转换了的字节个数
备    注：
********************************************/
uint8  Bytes2Str( uint8 *Bytes, uint8 *Str, uint8 ByteNum )
{
  uint8 i=0;
  
  
  return i;
}

/*** 函数名称: uint8 ul2str(u_int32 u32dat, u_int8 str[]) 
**** 功    能: 转换长整形(32位)为字符串类型，自动添加'\0'
***  参    数: 
***  返  回值：
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