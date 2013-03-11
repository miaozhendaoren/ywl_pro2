#ifndef _SEGLCD_H
#define _SEGLCD_H  
                                            //全局宏定义开始-------------

#define DIS_Dot       0x0020
#define DIS_B         0x0040
#define DIS_N         0x0080
#define DIS_J         0x0100
#define DIS_K         0x0200
#define DIS_W         0x0400
#define DIS_R         0x0800
#define DIS_X         0x1000
#define DIS_Y         0x2000
#define DIS_Z         0x4000
#define DIS_U         0x8000

#define BLK_Dot       (DIS_Dot)
#define BLK_B         (DIS_B)
#define BLK_N         0x0080
#define BLK_J         0x0100
#define BLK_K         0x0200
#define BLK_W         0x0400
#define BLK_R         0x0800
#define BLK_X         0x1000
#define BLK_Y         0x2000
#define BLK_Z         0x4000
#define BLK_U         0x8000

/******************************************************************************/
#ifdef  SEGLCD                          //内部定义声明开始-----------


/*************************************************************************/
#else                                       //外部引用开始---------



#endif
#endif