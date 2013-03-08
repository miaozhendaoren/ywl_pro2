#ifndef _I2C_H
#define _I2C_H  
                                            //全局宏定义开始-------------

//IIC是否使用IO模拟
#define IIC_IO_SIM    1

enum _IICSTATE {eIIC_FREE, eIIC_BUSY};
typedef enum _IICSTATE IICSTATE;
/******************************************************************************/
#ifdef  I2C                                //内部定义声明开始-----------


/*************************************************************************/
#else                                       //外部引用开始---------
extern IICSTATE ge_iicState;

#if IIC_IO_SIM
extern void iic_init(void);
extern void iic_start(void);
extern void iic_stop(void);
extern void iic_SndByte(u_int8  ch);
extern void iic_master_ack(void);
extern void iic_master_nack(void);
extern u_int8 iic_RcvByte(void);
extern u_int8 iic_isSlaveAck(void);

#endif

#endif
#endif