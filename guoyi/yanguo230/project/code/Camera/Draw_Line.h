#ifndef __DRAW_LINE_H_
#define __DRAW_LINE_H_

#include "qy_headfile.h"

void Patch_line_Left(int16 zuobiao_botton[2], int16 zuobiao_top[2]);
void Patch_line_Right(int16 zuobiao_botton[2], int16 zuobiao_top[2]);
void Add_mid_left(int16 start, int16 end);
void Patch_line_Right_Init(int16 start, int16 end);
void Patch_line_Left_Init(int16 start, int16 end);
void Right_Patch_Init(void);
void Left_Patch_Init(void);

#endif
