#ifndef _I2C_H
#define _I2C_H  
                                            //全局宏定义开始-------------

enum _IICSTATE {eIIC_FREE, eIIC_BUSY};
typedef enum _IICSTATE IICSTATE;

//IIC是否使用IO模拟
#define IIC_IO_SIM    1

/******************************************************************************/
#ifdef  I2C                                //内部定义声明开始-----------

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
#else                                       //外部引用开始---------
extern IICSTATE ge_iicState;
extern void Init_I2c(void);

#if IIC_IO_SIM
extern u_int8 ISendStr(u_int8 sla,u_int8 suba,u_int8 *s,u_int8 no);
extern u_int8 IRcvStr(u_int8 sla,u_int8 suba,u_int8 *s,u_int8 no);

#endif

#endif
#endif