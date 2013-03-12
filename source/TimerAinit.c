#define TIMERAINIT
#include "msp430f149.h"
#include "alldefs.h"

u_int16 u16start_CCR2 = 0;     //CCR2上升沿捕获时TAR的值
u_int16 u16end_CCR2 = 0;       //CCR2下降沿捕获时TAR的值
u_int16 u16ovCnt_TAR = 0;      //在CCR2 上升沿 和 下降沿 之间 TAR的溢出次数
                               //(从start_TAR的值开始到65535，再到下降沿时的end_TAR)
u_int8 u8isMeasuring = 0;        //是否正在测量(脉宽测量)中

static u_int8 u8CapPos_edge = 0;  //上升沿捕获
/*******************************************
函数名称：TimerA_reset
功    能：定时器A初始化
参    数：无
返回值  ：无
说    明：CCR2用于捕获,进入TAR的脉冲为MCLK或SMCLK，最后分频到1MHz
********************************************/
void TimerA_reset(void)
{
  u16start_CCR2 = 0;
  u16end_CCR2 = 0;
  u16ovCnt_TAR = 0;

  //脉宽计数特殊功能IO初始化

  /* P1.3 ----> CCR2捕获引脚CCI2A   接ICL7135 busy引脚 
  ** P1.4 SMCLK OUT 250K脉冲输出    接ICL7135 clk引脚
  */
  P1SEL |= BIT3 + BIT4;
  P1DIR &= ~BIT3;
  P1DIR |= BIT4;

  TACTL |= TACLR;
  CCTL2 = CAP + CM_1 + SCS + CCIS_0 + CCIE; //CCR2 捕获模式，上升沿捕获，同步，捕获输入端 CCI2A(),允许中断
  u8CapPos_edge = 1;
  
  TACTL = TASSEL_2 + ID_3 + MC_2 + TAIE;    //TACLK = SMCLKK，8分频为250K，连续增模式,允许溢出中断
}

/*******************************************
函数名称：TimerA_stop
功    能：定时器A停止计时
参    数：无
返回值  ：无
说    明：
********************************************/
void TimerA_stop(void)
{
  TACTL &= ~(MC0 +MC1);    //停止定时
}

/**********************************************
函数名称：TiemrA1_ISR
功    能：TimerA CCR1 CCR2 TA定时器中断服务程序
参    数：无
返回值  ：无
**********************************************/
#pragma vector=TIMERA1_VECTOR
__interrupt void TiemrA1_ISR(void)
{
  switch(TAIV)
  {
  case 2:         //TACCR1  中断向量
    break;
  case 4:         //TACCR2  中断向量  , 捕获中断
    {
         if( u8CapPos_edge )
         {//上升沿捕获
            u16start_CCR2 = CCR2;
            u8isMeasuring = 1;   
            CCTL2 = CAP + CM_2 + SCS + CCIS_0 + CCIE;   //CCR2 捕获模式，下降沿捕获，同步，捕获输入端 CCI2A(),允许中断
            u8CapPos_edge = 0;
         }else{//下降沿捕获
            u16end_CCR2 = CCR2;
            u8isMeasuring = 0;
            CCTL2 = CAP + CM_1 + SCS + CCIS_0 + CCIE;   //CCR2 捕获模式，上升沿捕获，同步，捕获输入端CCI2A(),允许中断
            u8CapPos_edge = 1;
         }
    }break;
  case 10:        //TAIFG   中断向量   , 溢出中断
    {
        if( u8isMeasuring )
        {//测量中
           u16ovCnt_TAR++;
        }
    }break;
    
  default:
    break;
  }
  
}