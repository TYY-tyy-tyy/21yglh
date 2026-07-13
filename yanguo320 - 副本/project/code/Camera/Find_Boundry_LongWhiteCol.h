#ifndef __FIND_BOUNDRY_LONGWHITECOL_H_
#define __FIND_BOUNDRY_LONGWHITECOL_H_

#include "qy_headfile.h"

/* 左右丢线相关变量 */
extern int16 Right_Lost_Line_count;
extern int16 Left_Lost_Line_count;

/* 最长白列白点数量 */
extern int16 White_Column_MID;

extern uint16 ALL_reference_col_farthest,ALL_White_Column_MID;

/* 边界相关变量声明 */
extern int16 Left_Line[MT9V03X_H];
extern int16 Right_Line[MT9V03X_H];

/* 每行误差 */
extern int16 Road_Wide[MT9V03X_H];

/* 参考列所在的列数 */
extern uint8 reference_col;
/* 参考列的最远端 */
extern uint8 reference_col_farthest;

extern uint8 remote_distance[MT9V03X_W];

void Find_Boundry_LongWhiteCol(void);
void Get_Long_White_col(void);

#endif 
