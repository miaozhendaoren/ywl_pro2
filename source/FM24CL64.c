/* 开发板配置  同时  有 延时等 共用的程序 */
#define FM24CL64
#include "alldefs.h"              
#include "msp430f149.h"
#include "i2c.h"
#include "FM24CL64.h"

#define IICADDR24CL64   0x01

/**************************************************************
名称：u_int8   Read_FRAM(u_int16 start_addr, u_int8* pbuf, u_int8 num)
功能：从FRAM  start_addr  开始读 num 个字节 到 pbuf指向的缓冲区中
返回: 读到的字节个数
**************************************************************/
u_int8   Read_FRAM(u_int16 start_addr, u_int8* pbuf, u_int8 num)
{
  u_int8 i = 0;  
  u_int8 ack;
  
  ack = 0;                             //应答置为0
  Start_I2c();                         //启动总线
  SendByte(IICADDR24CL64&0xfe,&ack);   //发送FRAM从机地址-写
  if(ack==0)return(0);
  
  SendByte((u_int8)(start_addr>>8),&ack); //写器件子地址-高字节
  if(ack==0)return(0);
  SendByte((u_int8)(start_addr&0x00ff), &ack);  
  if(ack==0)return(0);                 //写器件子地址-低字节
 
  Start_I2c();                         //重启总线
  SendByte(IICADDR24CL64|0x01,&ack);   //发送FRAM从机地址-读
  if(ack==0)return(0);
  
  for(i=0;i<num-1;i++)
  {   
    *pbuf=RcvByte();                /*发送数据*/
     Ack_I2c(0);                 /*发送就答位*/  
    pbuf++;
  } 
  *pbuf=RcvByte();
  Ack_I2c(1);                    /*发送非应位*/
  Stop_I2c();                    /*结束总线*/ 
  return(i);
}


/**************************************************************
名称：u_int8 Write_FRAM( u_int16 start_addr, const u_int8* pbuf, u_int8 num )
功能：写pbuf指向的缓冲区 num个字节 到 FRAM start_addr 开始处
参数：返回写入的个数
**************************************************************/
u_int8 Write_FRAM(u_int16 start_addr, const u_int8* pbuf, u_int8 num)
{
  u_int8 i = 0;
  u_int8 ack;
    
  ack = 0;                              //初始化为非应答  
  Start_I2c();                          //启动总线

  SendByte(IICADDR24CL64 & 0xfe,&ack);  //发送FRAM从机地址-写
  if(ack==0)return(0);
  SendByte( (u_int8)(start_addr>>8), &ack); //发送FRAM存储空间地址-高字节
  if(ack==0)return(0);
  SendByte( (u_int8)(start_addr&0x00ff), &ack); //发送FRAM存储空间地址-低字节
  if(ack==0)return(0);
  
  for(i=0;i<num;i++)
  {   
    SendByte(*pbuf,&ack);             /*发送数据*/
    if(ack==0)return(0);
    pbuf++;
  } 
  Stop_I2c();                 /*结束总线*/ 
  
  return(i);  
}


