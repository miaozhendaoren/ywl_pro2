#ifndef _I2C_H
#define _I2C_H  
                                            //ȫ�ֺ궨�忪ʼ-------------

//IIC�Ƿ�ʹ��IOģ��
#define IIC_IO_SIM    1

/******************************************************************************/
#ifdef  I2C                                //�ڲ�����������ʼ-----------

#if IIC_IO_SIM  
#define IIC_PORT_DIR  P2DIR
#define IIC_PORT_IN   P2IN   
#define IIC_PORT_OUT  P2OUT   

#define SDA    BIT4   
#define SCL    BIT6  

#define SDA_1  (IIC_PORT_DIR &= ~SDA)
#define SDA_0  (IIC_PORT_DIR |= SDA)

#define SCL_1  (IIC_PORT_OUT |=SCL)   
#define SCL_0  (IIC_PORT_OUT &=~SCL)   
#endif



/*************************************************************************/
#else                                       //�ⲿ���ÿ�ʼ---------
extern void Init_I2c(void);
extern void Start_I2c(void);
extern void Stop_I2c(void);
extern void   SendByte(u_int8 c, u_int8 *pAck);
extern u_int8 RcvByte(void);
extern void Ack_I2c(u_int8 dat);
#if IIC_IO_SIM

#endif

#endif
#endif