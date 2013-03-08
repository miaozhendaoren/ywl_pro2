#ifndef _I2C_H
#define _I2C_H  
                                            //ȫ�ֺ궨�忪ʼ-------------

//IIC�Ƿ�ʹ��IOģ��
#define IIC_IO_SIM    1

enum _IICSTATE {eIIC_FREE, eIIC_BUSY};
typedef enum _IICSTATE IICSTATE;
/******************************************************************************/
#ifdef  I2C                                //�ڲ�����������ʼ-----------


/*************************************************************************/
#else                                       //�ⲿ���ÿ�ʼ---------
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