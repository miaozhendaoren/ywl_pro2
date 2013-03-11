#define SEGLCD
#include "alldefs.h"
#include "msp430f149.h"
#include "PCF8562.h"
#include "segLcd.h"

/* 1888 �ĸ�������ʾλ������ */
#define G_a     (0x00000001 << 1)
#define G_b     (0x00000001 << 1)
#define G_c     (0x00000001 << 1)
#define G_d     (0x00000001 << 1)
#define G_e     (0x00000001 << 1)
#define G_f     (0x00000001 << 1)
#define G_g     (0x00000001 << 1)

#define S_a     (0x00000001 << 1)
#define S_b     (0x00000001 << 1)
#define S_c     (0x00000001 << 1)
#define S_d     (0x00000001 << 1)
#define S_e     (0x00000001 << 1)
#define S_f     (0x00000001 << 1)
#define S_g     (0x00000001 << 1)

#define B_a     (0x00000001 << 1)
#define B_b     (0x00000001 << 1)
#define B_c     (0x00000001 << 1)
#define B_d     (0x00000001 << 1)
#define B_e     (0x00000001 << 1)
#define B_f     (0x00000001 << 1)
#define B_g     (0x00000001 << 1)

#define EX_QIAN    (0x00000001 << 1)
#define EX_NEG     (0x00000001 << 1)
#define EX_DOT     (0x00000001 << 1)

#define EX_B       (0x00000001 << 1)
#define EX_N       (0x00000001 << 1)
#define EX_S       (0x00000001 << 1)
#define EX_T       (0x00000001 << 1)
#define EX_W       (0x00000001 << 1)
#define EX_K       (0x00000001 << 1)
#define EX_J       (0x00000001 << 1)

/* ����ʾλ��Ӧ��ʾֵ����� */
//��λ
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
                        G_a + G_b + G_c + G_d + G_f + G_f + G_g  //9
                        };
const u_int32 Ge_blink = G_a + G_b + G_c + G_d + G_e;             //��˸��λ

//ʮλ
const u_int32 Shi[10] = {
                        G_a + G_b + G_c + G_d + G_e + G_f,     //0
                        G_b + G_c,                             //1
                        G_a + G_b + G_g + G_e + G_d,           //2
                        G_a + G_b + G_c + G_g + G_d,           //3
                        G_b + G_c + G_g + G_f,                 //4
                        G_a + G_c + G_d + G_f + G_g,           //5
                        G_a + G_c + G_d + G_e + G_f + G_g,     //6
                        G_a + G_b + G_c,                       //7
                        G_a + G_b + G_c + G_d + G_e + G_f + G_g, //8                        
                        G_a + G_b + G_c + G_d + G_f + G_f + G_g  //9
                        };
const u_int32 Shi_blink = S_a + S_b + S_c + S_d + S_e;          //��˸ʮλ

//��λ
const u_int32 Bai[10] = {
                        G_a + G_b + G_c + G_d + G_e + G_f,     //0
                        G_b + G_c,                             //1
                        G_a + G_b + G_g + G_e + G_d,           //2
                        G_a + G_b + G_c + G_g + G_d,           //3
                        G_b + G_c + G_g + G_f,                 //4
                        G_a + G_c + G_d + G_f + G_g,           //5
                        G_a + G_c + G_d + G_e + G_f + G_g,     //6
                        G_a + G_b + G_c,                       //7
                        G_a + G_b + G_c + G_d + G_e + G_f + G_g, //8                        
                        G_a + G_b + G_c + G_d + G_f + G_f + G_g  //9
                        };
const u_int32 Bai_blink = B_a + B_b + B_c + B_d + B_e;           //��˸��λ


static u_int32 u32_disBuf[2] = {0,0};

/**************************************************************
���ƣ�CycleTask_SegDis
���ܣ�LCD��Ļ��ʾ����ѭ������
��ʾ˳��: ��u32_disStr[2]���������ֽڷ��͵���ʾ����
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
���ƣ�void MakeDisBuf(s_int16 dat,  u_int16 Dis_XX, u_int16 Blk_XX)
���ܣ�Ϊs_int16 �����ֺ� B W ��J K ���Ƿ���ʾ���� blink ������ʾ������
��ע�����ַ�Χ����-1999,1999�� ���Զ�����ǰ��0���Զ����㸺��
**************************************************************/
void MakeDisBuf(s_int16 dat,  u_int16 Dis_XX, u_int16 Blk_XX)
{
#define Abs(x) ((x)<0?(-(x)):(x))     //�����ֵ
  u_int8 num_buf[4] = {0,0,0,0};
  u_int32 u32_i;
  u_int16 u16_i;
  
   if( dat < -1999 || dat > 1999 )
    return;     
    
   u32_i = 0;
   if( dat < 0) u32_i |= EX_NEG;                              //��ʾ����
   u16_i = Abs(dat);
   if( u16_i/ > 999 )
      num_buf[3] =1 ;                   //ǧλ
   u16_i = u16_i % 1000;    num_buf[2] = u16_i/100;           //��λ
   u16_i = u16_i % 100;     num_buf[1] = ((u_int8)u16_i)/10;  //ʮλ
                            num_buf[0] = ((u_int8)u16_i)%10;  //��λ

   if( num_buf[3] )         u32_i |= EX_QIAN;                 //��ʾǧλ

   if( num_buf[3] )         u32_i |= Bai[num_buf[2]];         //��ʾ��λ,������ǰ��0
   else if( num_buf[2] )    u32_i |= Bai[num_buf[2]];       

   if( num_buf[3] || num_buf[2])   u32_i |= Bai[num_buf[1]];  //��ʾʮλ,������ǰ��0
   else if( num_buf[1] )    u32_i |= Shi[num_buf[1]];
                            u32_i |= Ge[num_buf[0]];          //��ʾ��λ
  
   if( Dis_XX & DIS_Dot )   u32_i |= Shi[num_buf[1]];       //�����С����ʮλ��ʹ��0ҲҪ��ʾ
   
   if( Dis_XX & DIS_Dot )   u32_i |= EX_DOT;                 //��ʾС����
   if( Dis_XX & DIS_B )     u32_i |= EX_B;                   //��ʾB
   if( Dis_XX & DIS_N )     u32_i |= EX_N;                   //��ʾN  
   
   u32_disBuf[0] = u32_i;
   u32_disBuf[1] = u32_i;
   
   if( Blk_XX )
   
}
