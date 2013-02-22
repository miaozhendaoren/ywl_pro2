#ifndef _LIB_H
#define _LIB_H  
                                            //全局宏定义开始-------------

/******************************************************************************/
#ifdef  LIB                         //内部定义声明开始-----------

/*************************************************************************/
#else                                       //外部引用开始---------

extern uint8 *pu8Float2Str( float fValue, uint8 u8DotLen);
extern uint8  Bytes2Str( uint8 *Bytes, uint8 *Str, uint8 ByteNum );
extern uint8  ul2str(u_int32 u32dat, u_int8 str[]);

#endif
#endif