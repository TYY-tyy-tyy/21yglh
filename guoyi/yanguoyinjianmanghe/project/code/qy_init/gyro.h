#ifndef __GYRO_H_
#define __GYRO_H_

#include "qy_headfile.h"

#define ZERO_CALC_COUNT  1000

#define true  		1
#define false 		0
	
#define ANGLE_TO_RAD(x)     ( (x) * PI / 180.0 )                                // 角度转换为弧度
#define RAD_TO_ANGLE(x)     ( (x) * 180.0 / PI )                                // 弧度转换为角度

#define PI                  ( 3.1415926535898 )

//角度获取
extern float gyro_z[2], next_gyro_z;			//(被测)Z轴角速度

extern float angle_ringR;
extern float avl_gyro_z;

extern float gyro_x[2], next_gyro_x;			//(被测)Z轴角速度

extern float angle_pitch;
extern float avl_gyro_x;

//角度获取
void tuoluo_init(void);
void gyroscope_get_gyro(void);		//角速度获取
void angle_get(void);							//角度获取
void angle_clear(void);						//角度清零
 
int8 null_drift_calculate(void);	//零漂解算

//三轴
typedef enum
{
    imu_X,
    imu_Y,
    imu_Z,
    NUM_XYZ
}imu_info_enum;

//陀螺仪
typedef struct
{

   uint8 zero_calc_flag;//零漂计算标志

   float angle[NUM_XYZ];//简单积分得到的角度

   float last_angle[NUM_XYZ];//上一次角度

   float zero_angle[NUM_XYZ];//总漂移量

   float gyro[NUM_XYZ];//角速度

   int16 count;

}gyroscope_info_struct;

//加速度计
typedef struct
{
    float angle[NUM_XYZ];//加速度计得到的角度
    float acc[NUM_XYZ];//角加速度
}acc_info_struct;

//加速度计
typedef struct
{
    double mag[NUM_XYZ];//角加速度
}mag_info_struct;

//imu
typedef struct
{
    gyroscope_info_struct gyro; //陀螺仪
    acc_info_struct acc; //加速度计
    mag_info_struct mag;//磁力计
}imu_info_struct;

//===================================================变量声明===================================================
extern imu_info_struct imu;
extern float acc_ratio;      //加速度计比例
extern float gyro_ratio;    //陀螺仪比例

/* 陀螺仪积分标志位 */
extern int16 gyro_jifen_flag;

/* 陀螺仪变化值 */
extern int16 gyro_angle_dif;

/* 上一次的角度值 */
extern int16 last_angle;

/* 累计旋转的角度 */
extern int16 total_angle;
//===================================================变量声明===================================================

//===================================================函数声明===================================================
void get_imu_data(void);
void gyro_get_angle(void);
void acc_get_angle(void);
void gyro_data_init(void);
double angle_calc(double acc_angle, double gyro);
int16 get_gyro_dif(int16 angle_start,int16 angle_end);
void use_gyro_flag(void);
void gyro_360_tiaoshi(void);
//===================================================函数声明===================================================

#endif /* CODE_GYRO_GYRO_H_ */

