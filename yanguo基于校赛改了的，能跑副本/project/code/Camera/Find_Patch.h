#ifndef __FIND_PATCH_H_
#define __FIND_PATCH_H_

#include "qy_headfile.h"

/* 左右上角点位置 */
extern int Left_up_Patch;
extern int Right_up_Patch;

/* 左右下角点位置 */
extern int Left_dowm_Patch;
extern int Left_dowm_Patch_x;
extern int Right_dowm_Patch;
extern int Right_dowm_Patch_x;

/* 某行黑点的数量，用于丢线保护 */
extern int Black_counts;

/* 丢线保护标志位 */
extern int lost_line_protect;

/*丢线数量*/
extern int Lost_Left_lost_nums;
extern int Lost_Right_lost_nums;

void Lost_connection_left(int start, int end);
int Find_left_up_point(int start, int end);
int Find_Right_up_point(int start, int end);
int Find_left_dowm_point(int start, int end);
int Find_Right_dowm_point(int start, int end);
int White_counts_weight(int p);
void Black_counts_weight(int p);

#endif 
