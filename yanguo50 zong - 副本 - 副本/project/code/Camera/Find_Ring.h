#ifndef __FIND_RING_H_
#define __FIND_RING_H_

#include "qy_headfile.h"

/* 左环枚举 */
typedef enum
{
    Left_0,
    Left_1,
    Left_2,
    Left_3,
    Left_4,
    Left_5,
    Left_6,
    Left_7,

}Left_Ring;

/* 右环枚举 */
typedef enum
{
    Right_0,
    Right_1,
    Right_2,
    Right_3,
    Right_4,
    Right_5,
    Right_6,
    Right_7,

}Right_Ring;

/* 单行的白点总数 */
extern int8 White_Nums;

/* 左右圆环标志位 */
extern int8 Find_Left_FLAG;
extern int8 Find_Right_FLAG;

/* 左圆环左右脉冲 */
extern uint16 Encoder_jifen_L;
extern uint16 Encoder_jifen_R;

/* 陀螺仪角度记录变量 */
extern int16 gyro_angle_start;
extern int16 gyro_angle_end;

/* 左右边局部丢线数量 */
extern int8 Left_local_LostNums;
extern int8 Right_local_LostNums;

/* 进环的次数 */
extern int8 Left_time;
extern int8 Right_time;

/* 环的数量 */
extern int16 Left_Ring_num;
extern int16 Right_Ring_num;

/* 陀螺仪积分角度 */
extern int16 Gyro_In_Left;//70
extern int16 Gyro_Out_Left;

extern int16 Gyro_In_Right;
extern int16 Gyro_Out_Right;

/* 出入环积分 */
extern uint16 Left_Enc_In;
extern uint16 Left_Enc_Out;
extern uint16 Right_Enc_In;
extern uint16 Right_Enc_Out;

extern uint8 r_con,l_con;

void Find_Left_Ring(void);
void Find_Right_Ring(void);
void Find_Ring(void);

#endif /* CODE_CAMERA_FIND_RING_FIND_RING_H_ */
