#define SEGLCD
#include "alldefs.h"
#include "msp430f149.h"
#include "PCF8562.h"
#include "segLcd.h"

/* 段码液晶驱动,用到i2c总线驱动
**
*/

/* 1888 四个数字显示位缓冲区 */
#define G_a     (0x80000000 >> 24)
#define G_b     (0x80000000 >> 25)
#define G_c     (0x80000000 >> 26)
#define G_d     (0x80000000 >> 27)
#define G_e     (0x80000000 >> 28)
#define G_f     (0x80000000 >> 29)
#define G_g     (0x80000000 >> 30)

#define S_a     (0x80000000 >> 16)
#define S_b     (0x80000000 >> 17)
#define S_c     (0x80000000 >> 18)
#define S_d     (0x80000000 >> 19)
#define S_e     (0x80000000 >> 20)
#define S_f     (0x80000000 >> 21)
#define S_g     (0x80000000 >> 22)

#define B_a     (0x80000000 >> 23)
#define B_b     (0x80000000 >>  8)
#define B_c     (0x80000000 >>  9)
#define B_d     (0x80000000 >> 10)
#define B_e     (0x80000000 >> 11)
#define B_f     (0x80000000 >> 12)
#define B_g     (0x80000000 >> 13)

#define EX_QIAN    (0x80000000 >> 14)
#define EX_NEG     (0x80000000 >> 15)
#define EX_DOT     (0x80000000 >> 31)

#define EX_K       (0x80000000 >> 0)
#define EX_J       (0x80000000 >> 1)
#define EX_E       (0x80000000 >> 2)
#define EX_T       (0x80000000 >> 3)
#define EX_N       (0x80000000 >> 5)
#define EX_S       (0x80000000 >> 4)
#define EX_B       (0x80000000 >> 7)
#define EX_W       (0x80000000 >> 6)



/* 各显示位对应显示值的码表 */
//个位
const u_int32 Ge[10] = {
                        G_a + G_b + G_c + G_d + G_e + G_f,     //0
                        G_b + G_c,                             //1
                        G_a + G_b + G_g + G_e + G_d,           //2
                        G_a + G_b + G_c + G_g + G_d,           //3
                        G_b + G_c + G_g + G_f,                 //4
                        G_a + G_c + G_d + G_f + G_g,           //5
                        G_a + G_c + G_d + G_e + G_f + G_g,     //6
                        G_a + G_b + G_c,                       //7
                        G_a + G_b + G_c + G_d + G_e + G_f + G_g, //8                        
                        G_a + G_b + G_c + G_d + G_f + G_g  //9
                        }; 
//十位
const u_int32 Shi[10] = {
                        S_a + S_b + S_c + S_d + S_e + S_f,     //0
                        S_b + S_c,                             //1
                        S_a + S_b + S_g + S_e + S_d,           //2
                        S_a + S_b + S_c + S_g + S_d,           //3
                        S_b + S_c + S_g + S_f,                 //4
                        S_a + S_c + S_d + S_f + S_g,           //5
                        S_a + S_c + S_d + S_e + S_f + S_g,     //6
                        S_a + S_b + S_c,                       //7
                        S_a + S_b + S_c + S_d + S_e + S_f + S_g, //8                        
                        S_a + S_b + S_c + S_d + S_f + S_g  //9
                        };
//百位
const u_int32 Bai[10] = {
                        B_a + B_b + B_c + B_d + B_e + B_f,     //0
                        B_b + B_c,                             //1
                        B_a + B_b + B_g + B_e + B_d,           //2
                        B_a + B_b + B_c + B_g + B_d,           //3
                        B_b + B_c + B_g + B_f,                 //4
                        B_a + B_c + B_d + B_f + B_g,           //5
                        B_a + B_c + B_d + B_e + B_f + B_g,     //6
                        B_a + B_b + B_c,                       //7
                        B_a + B_b + B_c + B_d + B_e + B_f + B_g, //8                        
                        B_a + B_b + B_c + B_d + B_f + B_g  //9
                        };

static u_int32 u32_disBuf[2] = {0,0};

/**************************************************************
名称：CycleTask_SegDis
功能：LCD屏幕显示函数循环函数
显示顺序: 将u32_disStr[2]缓冲区的字节发送到显示器上
**************************************************************/
void CycleTask_SegDis(void)
{
  static u_int8 i = 0;

  if( 0 == i )
  {
      PCF8562_disStr( (u_int8*)(&u32_disBuf[0]),0x00, 4);
      i = 1;
  }else{
      PCF8562_disStr( (u_int8*)(&u32_disBuf[1]),0x00,4 );
      i = 0;
  }
}

/**************************************************************
名称：void MakeDisBuf(s_int16 dat,  u_int16 Dis_XX, u_int16 Blk_XX)
功能：为s_int16 的数字和 B W 。J K 等是否显示，和 blink 生成显示缓冲区
备注：数字范围：【-1999,1999】 、自动消除前导0、自动计算负号
**************************************************************/
void MakeDisBuf(s_int16 dat,  u_int16 Dis_XX, u_int16 Blk_XX)
{
#define Abs(x) ((x)<0?(-(x)):(x))     //求绝对值
  u_int8 num_buf[4] = {0,0,0,0};
  u_int32 u32_i;
  u_int16 u16_i;
  
   if( dat < -1999 || dat > 1999 )
    return;     
    
   u32_i = 0;
   if( dat < 0) u32_i |= EX_NEG;                              //显示负号
   u16_i = Abs(dat);
   if( u16_i > 999 )
      num_buf[3] =1 ;                   //千位
   u16_i = u16_i % 1000;    num_buf[2] = u16_i/100;           //百位
   u16_i = u16_i % 100;     num_buf[1] = ((u_int8)u16_i)/10;  //十位
                            num_buf[0] = ((u_int8)u16_i)%10;  //个位

   if( num_buf[3] )         u32_i |= EX_QIAN;                 //显示千位

   if( num_buf[3] )         u32_i |= Bai[num_buf[2]];         //显示百位,并消除前导0
   else if( num_buf[2] )    u32_i |= Bai[num_buf[2]];       

   if( num_buf[3] || num_buf[2])   u32_i |= Shi[num_buf[1]];  //显示十位,并消除前导0
   else if( num_buf[1] )    u32_i |= Shi[num_buf[1]];
                            
                            u32_i |= Ge[num_buf[0]];          //显示个位
  
   if( Dis_XX & DIS_Dot )   u32_i |= Shi[num_buf[1]];       //如果是小数，十位即使是0也要显示
   
   if( Dis_XX & DIS_Dot )   u32_i |= EX_DOT;                 //显示小数点
   if( Dis_XX & DIS_K )     u32_i |= EX_K;                   //显示K
   if( Dis_XX & DIS_J )     u32_i |= EX_J;                   //显示J  
   if( Dis_XX & DIS_E )     u32_i |= EX_E;
   if( Dis_XX & DIS_T )     u32_i |= EX_T;
   if( Dis_XX & DIS_N )     u32_i |= EX_N;
   if( Dis_XX & DIS_S )     u32_i |= EX_S;
   if( Dis_XX & DIS_B )     u32_i |= EX_B;
   if( Dis_XX & DIS_W )     u32_i |= EX_W;
   
   u32_disBuf[0] = u32_i;                                   //数据送入缓冲区
   u32_disBuf[1] = u32_i;
   
   
   //blink  闪烁设置
   if( Blk_XX & BLK_Dot )  u32_disBuf[1] &= ~EX_DOT;
   if( Blk_XX & BLK_K )    u32_disBuf[1] &= ~EX_K;
   if( Blk_XX & BLK_J )    u32_disBuf[1] &= ~EX_J;
   if( Blk_XX & BLK_E )    u32_disBuf[1] &= ~EX_E;
   if( Blk_XX & BLK_T )    u32_disBuf[1] &= ~EX_T;
   if( Blk_XX & BLK_N )    u32_disBuf[1] &= ~EX_N;
   if( Blk_XX & BLK_S )    u32_disBuf[1] &= ~EX_S;
   if( Blk_XX & BLK_B )    u32_disBuf[1] &= ~EX_B;
   if( Blk_XX & BLK_W )    u32_disBuf[1] &= ~EX_W;
   if( Blk_XX & BLK_Ge )   u32_disBuf[1] &= ~(G_a +G_b +G_c + G_d +G_e +G_f +G_g);
   if( Blk_XX & BLK_Shi )  u32_disBuf[1] &= ~(S_a +S_b +S_c + S_d +S_e +S_f +S_g);
   if( Blk_XX & BLK_Bai )  u32_disBuf[1] &= ~(B_a +B_b +B_c + B_d +B_e +B_f +B_g);
   if( Blk_XX & BLK_Qian)  u32_disBuf[1] &= ~EX_QIAN;
   
}
