#define DB18B20
#include "alldefs.h"
#include "msp430f149.h"
#include "db18b20.h"

#define DQ1 P1OUT |= BIT5
#define DQ0 P1OUT &= ~BIT5
#define DQ_in   P1DIR &= ~BIT5
#define DQ_out  P1DIR |= BIT5
#define DQ_val  (P1IN & BIT5)

/*******************************************
�������ƣ�Init_18B20
��    �ܣ���DS18B20���и�λ����
��    ������
����ֵ  ����ʼ��״̬��־��1--ʧ�ܣ�0--�ɹ�
********************************************/
u_int8 Init_18B20(void)
{
    u_int8 Error;
    
    DQ_out;
    _DINT();
    DQ0;
    delay_us(500);
    DQ1;
    delay_us(55);
    DQ_in;
    _NOP();
    if(DQ_val)      
    {
        Error = 1;          //��ʼ��ʧ��
    }
    else
    {
        Error = 0;          //��ʼ���ɹ�
    }
    DQ_out;
    DQ1;
    _EINT();
    
    delay_us(400);
    
    return Error;
}
/*******************************************
�������ƣ�Write_18B20
��    �ܣ���DS18B20д��һ���ֽڵ�����
��    ����wdata--д�������
����ֵ  ����
********************************************/
void Write_18B20(u_int8 wdata)
{
    u_int8 i;
    
    _DINT();
    for(i = 0; i < 8;i++)
    {
        DQ0;
        delay_us(6);            //��ʱ6us
        if(wdata & 0X01)    DQ1;
        else                DQ0;
        wdata >>= 1;
        delay_us(50);           //��ʱ50us
        DQ1;
        delay_us(10);           //��ʱ10us
    }
    _EINT();
}
/*******************************************
�������ƣ�Read_18B20
��    �ܣ���DS18B20��ȡһ���ֽڵ�����
��    ������
����ֵ  ��������һ���ֽ�����
********************************************/
u_int8 Read_18B20(void)
{
    u_int8 i;
    u_int8 temp = 0;
    
    _DINT();
    for(i = 0;i < 8;i++)
    {
        temp >>= 1;
        DQ0;
        delay_us(6);            //��ʱ6us
        DQ1;
        delay_us(8);            //��ʱ9us
        DQ_in;
        _NOP();
        if(DQ_val)   temp |= 0x80;
        delay_us(45);           //��ʱ45us
        DQ_out;
        DQ1;
        delay_us(10);           //��ʱ10us
    }
    _EINT();
    
    return  temp;
}

/*******************************************
�������ƣ�Skip
��    �ܣ�����������ȡ��ƷID������
��    ������
����ֵ  ����
********************************************/
void Skip(void)
{
    Write_18B20(0xcc);
}
/*******************************************
�������ƣ�Convert
��    �ܣ������¶�ת������
��    ������
����ֵ  ����
********************************************/
void Convert(void)
{
    Write_18B20(0x44);
}
/*******************************************
�������ƣ�Read_SP
��    �ܣ����Ͷ�ScratchPad����
��    ������
����ֵ  ����
********************************************/
void Read_SP(void)
{
    Write_18B20(0xbe);
}
/*******************************************
�������ƣ�ReadTemp
��    �ܣ���DS18B20��ScratchPad��ȡ�¶�ת�����
��    ������
����ֵ  ����ȡ���¶���ֵ
********************************************/
u_int16 ReadTemp(void)
{
    u_int8 temp_low;
    u_int16  temp;
    
    temp_low = Read_18B20();      //����λ
    temp = Read_18B20();     //����λ
    temp = (temp<<8) | temp_low;
    
    return  temp;
}
/*******************************************
�������ƣ�ReadTemp
��    �ܣ�����DS18B20���һ���¶�ת��
��    ������
����ֵ  ���������¶���ֵ
********************************************/
u_int16 Do1Convert(void)
{
    u_int8 i; 
    
    do
    {
        i = Init_18B20();
    }
    while(i);
    Skip();
    Convert();
    for(i = 20;i > 0;i--)  
        delay_us(60000); //��ʱ800ms����
    do
    {
        i = Init_18B20();
    }
    while(i);
    Skip();
    Read_SP();
    return ReadTemp();
}
