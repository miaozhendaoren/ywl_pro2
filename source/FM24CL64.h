#ifndef _FM24CL64_H
#define _FM24CL64_H  
                                            //ȫ�ֺ궨�忪ʼ-------------

/******************************************************************************/
#ifdef  FM24CL64                         //�ڲ�����������ʼ-----------


/*************************************************************************/
#else                                       //�ⲿ���ÿ�ʼ---------
extern u_int8   Read_FRAM(u_int16 start_addr, u_int8* pbuf, u_int8 num);
extern u_int8 Write_FRAM(u_int16 start_addr, const u_int8* pbuf, u_int8 num);
#endif
#endif