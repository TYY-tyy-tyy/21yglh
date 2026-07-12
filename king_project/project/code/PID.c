#include "PID.h"

volatile uint16 Out_servo = SERVO_DUTY_MID;

uint8 clamp_u8(int16 x, uint8 min_v, uint8 max_v)
{
    if (x < min_v) return min_v;
    if (x > max_v) return max_v;
    return (uint8)x;
}

/* 限制输出范围的函数 */
float limit_float(float x, float min, float max)
{
    if (x > max) return max;
    if (x < min) return min;
    return x;
}


volatile PID_tf pid_rf = {25.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
volatile PID_tf pid_lf = {25.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
/* 后轮增量式 PID */
void Increment_PIDf(volatile PID_tf *pid, float tar_val, float act_val)
{
    float error;
    float d_err;
    float dd_err;
    float delta;
    float new_out;

    // 计算误差
    error = tar_val - act_val;

    // 计算误差变化量
    d_err  = error - pid->err1;
    dd_err = error - 2 * pid->err1 + pid->err2;

    // 计算 PID 输出
    delta = 0.0;
    delta += pid->kp * d_err;        // P 项
    delta += pid->ki * error;        // I 项
    delta += pid->kd * dd_err;       // D 项

    // 更新输出
    new_out = pid->out + delta;
    pid->out = limit_float(new_out, OUT_MIN, OUT_MAX);  // 限制输出范围

    // 更新误差历史
    pid->err2 = pid->err1;
    pid->err1 = error;
}



volatile PID_tf servo_pidf = {
		50.0f,  //P   65
		0.0f, 	//I
		0.0f, 	//D
		0.20f, 	//陀螺仪  0.23
		0.0f,   //KP2
		0.0f, 			
		0.0f, 			
		0.0f		
};  // 初始值可以根据需要调整



// 绝对值计算
float abs_float(float x)
{
    if (x < 0) return -x;
    return x;
}

// 简化版舵机PID控制
void PID_servof(volatile PID_tf *pid)
{
    // 1. 计算误差
    uint8 ref = controlReferenceLine;
    float err = (float)mid_line[ref] - (float)Mid_Col;//(float)err_sum - (float)Mid_Col;
    float duty;
    // 2. 计算输出 (只有P项 + 二次项 + 陀螺仪补偿)
    float p_out = pid->kp * err;
    float kp2_out = pid->kp2 * err * fabs(err);
    
    // 总输出 (减去陀螺仪补偿)
    pid->out = p_out + kp2_out - pid->kg * imu660ra_gyro_z;
    
    // 3. 输出限幅
    pid->out = limit_float(pid->out, OUT_MIN, OUT_MAX);
    
    // 4. 计算舵机占空比
    duty = (float)SERVO_DUTY_MID - pid->out;
    duty = limit_float(duty, SERVO_DUTY_MIN, SERVO_DUTY_MAX);
    
    Out_servo = (uint16)duty;
    
    // 5. 更新误差历史
    pid->err1 = err;
}



