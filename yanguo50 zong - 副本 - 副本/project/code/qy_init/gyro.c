#include "gyro.h"

imu_info_struct imu;

/* 陀螺仪积分标志位 */
int gyro_jifen_flag = 0;

/* 陀螺仪变化值 */
int16 gyro_angle_dif = 0;

/* 上一次的角度值 */
int16 last_angle = 0;

/* 累计旋转的角度 */
int16 total_angle = 0;

#include "gyro.h"

//角度
float gyro_z[2]={0}, next_gyro_z=0;			//(被测)Z轴角速度
float gyro_x[2]={0}, next_gyro_x=0;			//(被测)x轴角速度
float angle_ringR=0;
float angle_pitch=0;

float null_drift_z=0;	//零漂
float avl_gyro_z=0;   //测量

float null_drift_x=0;	//零漂
float avl_gyro_x=0;   //测量

/****************角度获取****************/

void gyroscope_get_gyro(void)
{
	imu963ra_get_gyro();
	gyro_z[0] = 0.9*(imu963ra_gyro_z)+0.1*gyro_z[1];
	gyro_z[1] = gyro_z[0];
	gyro_x[0] = 0.9*(imu963ra_gyro_x)+0.1*gyro_x[1];
	gyro_x[1] = gyro_x[0];
//	avl_gyro_z = imu963ra_gyro_transition(imu963ra_gyro_z);
	next_gyro_z = imu963ra_gyro_transition(gyro_z[0]);
	avl_gyro_z = next_gyro_z - null_drift_z;
	
	next_gyro_x = imu963ra_gyro_transition(gyro_x[0]);
	avl_gyro_x = next_gyro_x - null_drift_x;
	
	
	angle_get();
}

void angle_get(void)
{
	angle_ringR+=0.005*avl_gyro_z;
	angle_pitch+=0.005*avl_gyro_x;
}

void angle_clear(void)
{
	angle_ringR=0;
	angle_pitch = 0;
}


int8 null_drift_calculate(void)
{
	static int16 cnt_null=0;
	static float temp_z=0 , temp_x=0;
	static int8 ret=0;
	
	//采集数据
//	
	
	temp_z += next_gyro_z;
	temp_x += next_gyro_x;
		
	cnt_null++;
	if(cnt_null>=200)
	{
		ret = 1;
		null_drift_x = temp_x/200;
		null_drift_z = temp_z/200;
		angle_clear();
	}
	
	return ret;
}

void gyro_data_init(void)
{
    imu.gyro.count = 0;
    imu.gyro.zero_calc_flag = true;
}

void get_imu_data(void)
{
#if IMU_TYPE
    imu963ra_get_acc();         // 获取   963的加速度测量数值
    imu963ra_get_gyro();        // 获取963的角速度测量数值

#else
    imu660ra_get_acc();         // 获取963的加速度测量数值
    imu660ra_get_gyro();        // 获取963的角速度测量数值
#endif
}

//660ra
//陀螺仪获取角度
void gyro_get_angle(void)
{
#if IMU_TYPE
        // 简单积分
        imu.gyro.angle[imu_Z] -= imu963ra_gyro_z /  GYRO_SPL * 0.005;
        imu.gyro.angle[imu_Y] += imu963ra_gyro_y /  GYRO_SPL * 0.005;
        imu.gyro.angle[imu_X] += imu963ra_gyro_x /  GYRO_SPL * 0.005;
#else
        // 简单积分
        imu.gyro.angle[imu_Z] -= imu660ra_gyro_z / GYRO_SPL* 0.005;
        imu.gyro.angle[imu_Y] += imu660ra_gyro_y / GYRO_SPL* 0.005;
        imu.gyro.angle[imu_X] += imu660ra_gyro_x / GYRO_SPL* 0.005;
#endif


    if (imu.gyro.zero_calc_flag) // 计算零漂标志
    {
        // 计算零漂
        if (imu.gyro.count < ZERO_CALC_COUNT) // 零漂计算次数
        {
            imu.gyro.zero_angle[imu_Z] += imu.gyro.angle[imu_Z] - imu.gyro.last_angle[imu_Z];
            imu.gyro.last_angle[imu_Z] = imu.gyro.angle[imu_Z];

            imu.gyro.zero_angle[imu_Y] += imu.gyro.angle[imu_Y] - imu.gyro.last_angle[imu_Y];
            imu.gyro.last_angle[imu_Y] = imu.gyro.angle[imu_Y];

            imu.gyro.zero_angle[imu_X] += imu.gyro.angle[imu_X] - imu.gyro.last_angle[imu_X];
            imu.gyro.last_angle[imu_X] = imu.gyro.angle[imu_X];

            imu.gyro.count++;
        }
        // 算出零漂
        else
        {
            imu.gyro.angle[imu_Z] -= imu.gyro.zero_angle[imu_Z];
            imu.gyro.zero_angle[imu_Z] /= ZERO_CALC_COUNT;

            imu.gyro.angle[imu_Y] -= imu.gyro.zero_angle[imu_Y];
            imu.gyro.zero_angle[imu_Y] /= ZERO_CALC_COUNT;

            imu.gyro.angle[imu_X] -= imu.gyro.zero_angle[imu_X];
            imu.gyro.zero_angle[imu_X] /= ZERO_CALC_COUNT;

            imu.gyro.zero_calc_flag = false;
            imu.gyro.count = 0;
        }
    }
    // 减去零漂
    else
    {
        imu.gyro.angle[imu_Z] -= imu.gyro.zero_angle[imu_Z];
        imu.gyro.angle[imu_Y] -= imu.gyro.zero_angle[imu_Y];
        imu.gyro.angle[imu_X] -= imu.gyro.zero_angle[imu_X];

        if(imu.gyro.angle[imu_Z] > 180)
            imu.gyro.angle[imu_Z] -= 360;
        else if(imu.gyro.angle[imu_Z] < -180)
            imu.gyro.angle[imu_Z] += 360;
    }
}

//加速计获取角度
void acc_get_angle(void)
{
#if IMU_TYPE
        double acc_x = imu963ra_acc_x / ACC_SPL;
        double acc_y = imu963ra_acc_y / ACC_SPL;
        double acc_z = imu963ra_acc_z / ACC_SPL;

        imu.acc.angle[imu_X]= RAD_TO_ANGLE(atan( acc_y / acc_z));
        imu.acc.angle[imu_Y]= -RAD_TO_ANGLE(atan( acc_x / sqrt(acc_y * acc_y +acc_z * acc_z)));
#else
        double acc_x = imu660ra_acc_x / ACC_SPL;
        double acc_y = imu660ra_acc_y / ACC_SPL;
        double acc_z = imu660ra_acc_z / ACC_SPL;

        imu.acc.angle[imu_X] = RAD_TO_ANGLE (atan( acc_y / acc_z));
        imu.acc.angle[imu_Y] = RAD_TO_ANGLE (atan(acc_x / sqrt(acc_y * acc_y +acc_z * acc_z)));
#endif

}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     得到转向的角度
// 参数说明
// 返回参数
// 使用示例
// 备注信息     用于进出圆环的角度判断
//------------------------------------------------------------------------------------------------------------------
int16 get_gyro_dif(int16 angle_start,int16 angle_end)
{
    int16 Y1,Y2,result;

    /* 数据处理 */
    //得到Y1
    if(angle_start >= 0)
    {
        Y1 = 180 - angle_start;
    }
    else
    {
        Y1 = -180 - angle_start;
    }

    //得到Y2
    if(angle_end >= 0)
    {
        Y2 = 180 - angle_end;
    }
    else
    {
        Y2 = -180 - angle_end;
    }

    /* 结果计算 */
    result = Y2 - Y1;

    /* 结果处理 */
    if(result > 180) result -= 360;
    if(result < -180) result += 360;

//    int16 result;
//
//    result = angle_end - angle_start;
//
//    /* 结果处理 */
//    if(result > 180) result -= 360;
//    if(result < -180) result += 360;

    /* 返回结果 */
    return result;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     判断陀螺仪记录的标志位
// 参数说明
// 返回参数
// 使用示例
// 备注信息     用于元素
//------------------------------------------------------------------------------------------------------------------
void use_gyro_flag(void)
{
    /* 陀螺仪积分标志位 */
    if(gyro_jifen_flag == 1)
    {
        gyro_angle_start = (int16)imu.gyro.angle[imu_Z];
        last_angle = (int16)imu.gyro.angle[imu_Z];
    }
    else if(gyro_jifen_flag == 2)
    {
        gyro_angle_end = (int16)imu.gyro.angle[imu_Z];
    }
    else
    {
        gyro_angle_start = 0;
        gyro_angle_end = 0;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     360度陀螺仪调试
// 参数说明
// 返回参数
// 使用示例
// 备注信息     用于元素
//------------------------------------------------------------------------------------------------------------------
void gyro_360_tiaoshi(void)
{
    if(gyro_jifen_flag == 0)
    {
        gyro_jifen_flag = 1;
    }

    use_gyro_flag();

    gyro_jifen_flag = 2;
    if(total_angle < -360)
    {
        Left_Out = 0;
        Right_Out = 0;
    }
    else
    {
        gyro_angle_dif = get_gyro_dif(last_angle,gyro_angle_end);

        total_angle += gyro_angle_dif;

        last_angle = gyro_angle_end;

        Left_Out = 1000;
        Right_Out = 5000;

    }
    Left_Motor_Speed(Left_Out);
    Right_Motor_Speed(Right_Out);
}
