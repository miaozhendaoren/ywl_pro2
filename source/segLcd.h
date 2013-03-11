#ifndef _SEGLCD_H
#define _SEGLCD_H  
                                            //全局宏定义开始-------------

#define DIS_NONE      0x0000
#define DIS_Dot       0x0001
#define DIS_K         0x0002
#define DIS_J         0x0004
#define DIS_E         0x0008
#define DIS_T         0x0010
#define DIS_N         0x0020
#define DIS_S         0x0040
#define DIS_B         0x0080
#define DIS_W         0x0100
#define DIS_Ge        0x0200
#define DIS_Bai       0x0400
#define DIS_Shi       0x0800
#define DIS_Qian      0X1000

#define BLK_NONE       0x0000
#define BLK_Dot       (DIS_Dot)
#define BLK_K         (DIS_K)
#define BLK_J         (DIS_J)
#define BLK_E         (DIS_E)
#define BLK_T         (DIS_T)
#define BLK_N         (DIS_N)
#define BLK_S         (DIS_S)
#define BLK_B         (DIS_B)
#define BLK_W         (DIS_W)
#define BLK_Ge        (DIS_Ge)
#define BLK_Shi       (DIS_Shi)
#define BLK_Bai       (DIS_Bai)
#define BLK_Qian      (DIS_Qian)



/******************************************************************************/
#ifdef  SEGLCD                          //内部定义声明开始-----------


/*************************************************************************/
#else                                       //外部引用开始---------

extern void CycleTask_SegDis(void);
extern void MakeDisBuf(s_int16 dat,  u_int16 Dis_XX, u_int16 Blk_XX);

#endif
#endif