#define I2C
#include "alldefs.h"
#include "msp430f149.h"
#include "i2c.h"

/*  I2C �����Ϊ������
**    1.I2C�������� ��ʵ��Ӳ��I2C�����ģ��I2Cʱ(����ʱ����)
**                    ��I2C���߷����ݼ���I2C���߻�ȡ����
**                    ��Ӧ����豸 �� �����豸��Ӧ��
**                    ���߻���
**
**    2.I2C�豸���� ���õ�I2C���ߵ��豸��3��: PCF8562-����Һ������оƬ��
**                                            PCF8563-RTCʱ��оƬ
**                                            FM24CL04-4K����Ĵ���
**                    ��Ҫ���ݣ������豸���������ʼ������д����
**    3.�ϲ�Ӧ�ô���:
**                    ����˳��
**                              1.������ߵ�ǰ�Ƿ�ɱ�ʹ��,��ʼ����
**                              2.�����豸
**                              3.���ͻ��������
*/

/*     ���ļ�ʵ��  I2C��������
**                  1.Ӳ��I2C �� IOģ��I2C����
**                  2.Ӳ��I2C Ӳ��ģ�� ����
**                  3.IOģ��I2C IO�� ����
**                  4.Ӳ����ʼ�����ߡ����͡������ֽ�
**                  5.�����ʼ�����ߡ����͡������ֽ�
**                  6.���߻���
*/

/**********************************************************************
IIC���߱�������
***********************************************************************/
IICSTATE ge_iicState = eIIC_FREE; 


#if IIC_IO_SIM
/*---------------------IOģ��IIC����---------------------------*/
#define IIC_PORT_DIR  P2DIR
#define IIC_PORT_IN   P2IN   
#define IIC_PORT_OUT  P2OUT   

#define SDA    BIT6   
#define SCL    BIT7  
#define SDA_in  (IIC_PORT_DIR &= ~SDA)
#define SDA_out (IIC_PORT_DIR |= SDA)
#define SDA_1  (IIC_PORT_OUT |= SDA)
#define SDA_0  (IIC_PORT_OUT &= SDA)

#define SCL_out (IIC_PORT_DIR |= SCL)
#define SCL_1  (IIC_PORT_OUT |=SCL)   
#define SCL_0  (IIC_PORT_OUT &=~SCL)   
#define IIC_TIME 100   
  
/*******************************************************************  
����: void iic_init();    
����: IOģ��IIC���� Ӳ����ʼ��
********************************************************************/   
void iic_init(void)   
{   
 SCL_0;
 SCL_out;
 SDA_1;
 SDA_out;
}   
/*******************************************************************  
                     �����ߺ���                 
����ԭ��: void  iic_start();    
����:����I2C����    
********************************************************************/   
void iic_start(void)   
{   
 SDA_1; delay_us(IIC_TIME);   
 SCL_1; delay_us(IIC_TIME);

 SDA_0; delay_us(IIC_TIME);   
 SCL_0; delay_us(IIC_TIME);   
}   
/*******************************************************************  
                      �������ߺ���                 
����ԭ��: void  iic_stop();    
����:       ����I2C����
    
********************************************************************/   
void iic_stop(void)   
{   
 SDA_0; delay_us(IIC_TIME);   
 SCL_1; delay_us(IIC_TIME);   
 SDA_1; delay_us(IIC_TIME);   
 SCL_0; delay_us(IIC_TIME);   
}   
/*******************************************************************  
����: void  iic_SndByte(u_int8 ch);  
����: ��ӻ�����һ�ֽ�,������Ϻ�����isack()���ӻ��Ƿ����
********************************************************************/   
void  iic_SndByte(u_int8  ch)   
{   
 u_int8 i = 8;   
 SCL_0; delay_us(IIC_TIME);     
 while(i--)   
 {   
  if(ch & 0x80)
    SDA_1;   
  else
    SDA_0;
  ch <<= 1;   
  delay_us(IIC_TIME);     
  SCL_1; delay_us(IIC_TIME);     
  SCL_0; delay_us(IIC_TIME);     
 }     
}   
/*******************************************************************  
����ԭ��: u_int8  iic_RcvByte();  
����:  ������������1�ֽ� , ���������Ӧ������    
********************************************************************/      
u_int8  iic_RcvByte(void)   
{   
 u_int8 rdata = 0;   
 u_int8 i = 8;   
 SDA_in;
 delay_us(IIC_TIME);   
 while(i--)   
 {   
  rdata <<= 1;   
  SCL_0; delay_us(IIC_TIME);     
  SCL_1; delay_us(IIC_TIME);     
  if(IIC_PORT_IN &SDA) rdata |= 0x01; // ��������            
 }   
 SCL_0;       
 delay_us(IIC_TIME);   

 return(rdata);   
}   
   
/***************************************************************
���ƣ�void iic_master_ack(void)
���ܣ����� positiveӦ��
***************************************************************/
void iic_master_ack(void)
{   
 SDA_0; SDA_out;delay_us(IIC_TIME);   
 SCL_1; delay_us(IIC_TIME);   
 SCL_0; delay_us(IIC_TIME);   
}   

/***************************************************************
���ƣ�void iic_master_nack(void)
���ܣ����� negativeӦ��
***************************************************************/
void iic_master_nack(void)
{   
 SDA_1; SDA_out; delay_us(IIC_TIME);   
 SCL_1; delay_us(IIC_TIME);     
 SCL_0; delay_us(IIC_TIME);     
}   
/***************************************************************
���ƣ�u_int8 iic_isSlaveAck(void)
���ܣ��������ӻ�Ӧ���Ƿ�ΪACK,��ACK����1�����򷵻�0
***************************************************************/   
u_int8 iic_isSlaveAck(void)   
{   
 u_int8 a;  
 SDA_in; 
 SCL_1;                   //����Ӧ��ʱ��
 delay_us(IIC_TIME);
 a=IIC_PORT_IN &SDA;
 SCL_0;
 SDA_1;
 SDA_out;       //���߿����ڼ�Ĭ��SDA_1
 return(a);    
}  

/*------------------------IOģ��IIC���߽���--------------------------------*/
#else
/*------------------------Ӳ��IIC���߿�ʼ--------------------------------*/






#endif