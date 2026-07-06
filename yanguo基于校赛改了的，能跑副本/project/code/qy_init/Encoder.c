#include "Encoder.h"

/* 左右编码器的值 */
int16 Encoder_Left = 0;
int16 Encoder_Right = 0;

/* 积分标志位 */
int16 Encoder_jifen_flag = 0;

/* 左右编码器的平均值 */
int16 Encoder_Average = 0;

/* 卡尔曼 */
KFP Speed_Left_KFP = {0.001,0,0,0,0.04,0.2};
KFP Speed_Right_KFP = {0.001,0,0,0,0.04,0.2};

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     编码电机初始化
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void Encoder_Init(void)
{
    /* 左电机 */
    encoder_quad_init(LEFT_ENCODER_QUADDEC, LEFT_ENCODER_QUADDEC_A, LEFT_ENCODER_QUADDEC_B);
    /* 右电机 */
    encoder_quad_init(RIGHT_ENCODER_QUADDEC, RIGHT_ENCODER_QUADDEC_A, RIGHT_ENCODER_QUADDEC_B);
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     读取左编码器的值
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
int16 Left_Encoder_Get(void)
{
    int16 Left_Encoder_Get;
    Left_Encoder_Get = encoder_get_count(LEFT_ENCODER_QUADDEC);
    encoder_clear_count(LEFT_ENCODER_QUADDEC);

//    Left_Encoder_Get = kalmanFilter(&Speed_Left_KFP,Left_Encoder_Get);

    return Left_Encoder_Get;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     读取右编码器的值
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
int16 Right_Encoder_Get(void)
{
    int16 Right_Encoder_Get;
    Right_Encoder_Get = -encoder_get_count(RIGHT_ENCODER_QUADDEC);
    encoder_clear_count(RIGHT_ENCODER_QUADDEC);

//    Right_Encoder_Get = kalmanFilter(&Speed_Right_KFP,Right_Encoder_Get);

    return Right_Encoder_Get;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     处理积分的标志位
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void use_encoder_flag(void)
{
    /* 若积分标志位置1，则积分 */
    if(Encoder_jifen_flag == 1)
    {
        //左右轮开始积分
        Encoder_jifen_L += Encoder_Left;
        Encoder_jifen_R += Encoder_Right;
    }
    else
    {
        Encoder_jifen_L = 0;
        Encoder_jifen_R = 0;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     卡尔曼滤波
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
float kalmanFilter(KFP *kfp,float input)
{
    //预测协方差方程：k时刻系统估算协方差 = k-1时刻的系统协方差 + 过程噪声协方差
    kfp->Now_P = kfp->LastP + kfp->Q;
    //卡尔曼增益方程：卡尔曼增益 = k时刻系统估算协方差 / （k时刻系统估算协方差 + 观测噪声协方差）
    kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
    //更新最优值方程：k时刻状态变量的最优值 = 状态变量的预测值 + 卡尔曼增益 * （测量值 - 状态变量的预测值）
    kfp->out = kfp->out + kfp->Kg * (input -kfp->out);//因为这一次的预测值就是上一次的输出值
    //更新协方差方程: 本次的系统协方差付给 kfp->LastP 威下一次运算准备。
    kfp->LastP = (1-kfp->Kg) * kfp->Now_P;
    return kfp->out;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     用于统计赛道长度
// 参数说明
// 返回参数
// 使用示例
// 备注信息     单位m
//------------------------------------------------------------------------------------------------------------------
/* 赛道长度 */
int16 Track_length[2] = {0};

/* 当前积分值 */
int16 Enc_value = 0;

/* 统计n个一米的值 */
int16 Enc_n = 0;

/* 当前编码器一米的积分 */
#define     EncLength       950


void Count_Length(void)
{
    Enc_value += Encoder_Average;
    if(Enc_value >= EncLength)
    {
        Enc_value = 0;
        Track_length[0] += 0.1;
		if(Track_length[0] > 10000)
		{
			Track_length[1] ++;
			Track_length[0] = Track_length[0] -10000;
		}
    }
}

