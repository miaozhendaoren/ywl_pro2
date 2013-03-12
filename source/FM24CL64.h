#ifndef _FM24CL64_H
#define _FM24CL64_H  
                                            //全局宏定义开始-------------

/******************************************************************************/
#ifdef  FM24CL64                         //内部定义声明开始-----------


/*************************************************************************/
#else                                       //外部引用开始---------
extern u_int8   Read_FRAM(u_int16 start_addr, u_int8* pbuf, u_int8 num);
extern u_int8 Write_FRAM(u_int16 start_addr, const u_int8* pbuf, u_int8 num);
#endif
#endif