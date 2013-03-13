#ifndef _I2C_H
#define _I2C_H  
                                            //全局宏定义开始-------------

//IIC是否使用IO模拟
#define IIC_IO_SIM    1

/******************************************************************************/
#ifdef  I2C                                //内部定义声明开始-----------

#if IIC_IO_SIM  
#define IIC_PORT_DIR  P1DIR         //debug_105
#define IIC_PORT_IN   P1IN          //SCL P1.2    原来SCL P2.6
#define IIC_PORT_OUT  P1OUT         //SDA P1.3    原来SDA P2.4

#define SDA    BIT3   
#define SCL    BIT2                 //debug_105_end

#define SDA_1  (IIC_PORT_DIR &= ~SDA)
#define SDA_0  (IIC_PORT_DIR |= SDA)

#define SCL_1  (IIC_PORT_OUT |=SCL)   
#define SCL_0  (IIC_PORT_OUT &=~SCL)   
#endif



/*************************************************************************/
#else                                       //外部引用开始---------
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