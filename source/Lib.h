#ifndef _LIB_H
#define _LIB_H  
                                            //ȫ�ֺ궨�忪ʼ-------------

/******************************************************************************/
#ifdef  LIB                         //�ڲ�����������ʼ-----------

/*************************************************************************/
#else                                       //�ⲿ���ÿ�ʼ---------

extern uint8 *pu8Float2Str( float fValue, uint8 u8DotLen);
extern uint8  Bytes2Str( uint8 *Bytes, uint8 *Str, uint8 ByteNum );
extern uint8  ul2str(u_int32 u32dat, u_int8 str[]);

#endif
#endif