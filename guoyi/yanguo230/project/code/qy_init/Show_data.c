/*
 * Show_data.c
 *
 *  Created on: 2024年12月21日
 *      Author: 19131
 */

#include "zf_common_headfile.h"

/* 发车模式下的模式切换 */
int Show_Flag = 1;

/* 发车显示 */
void Show_data_1(void)
{
    /*-----------------------------第一行------------------------------ */
    /* 编码器数值 */
    tft180_show_string(5,60,"ENC:");
    tft180_show_int32(40,60,Encoder_Left,3);
    tft180_show_int32(70,60,Encoder_Right,3);

    /*-----------------------------第二行------------------------------ */
    /* 左右丢线数值 */
    tft180_show_string(5,80,"lost:");
    tft180_show_int32(45,80,Left_Lost_Line_count,3);
    tft180_show_int32(75,80,Right_Lost_Line_count,3);

    /*-----------------------------第三行100--------------------------------*/
    /* 中点误差 */
    tft180_show_string(5,100,"Err:");
    tft180_show_int32(40,100,Image_error,3);
    tft180_show_int32(70,100,gyro_angle_dif,3);

    /*-----------------------------第四行120--------------------------------*/
    /* 调试位置 */
    tft180_show_float(5,120,imu.gyro.angle[Z],4,2);
    tft180_show_int32(65,120,White_Column_MID,2);
    tft180_show_int32(95,120,White_Nums,2);

    /*-----------------------------第五行--------------------------------*/
    tft180_show_int32(5,140,Left_dowm_Patch,2);
    tft180_show_int32(35,140,Right_dowm_Patch,2);
    tft180_show_int32(65,140,Left_local_LostNums,2);
    tft180_show_int32(95,140,Right_local_LostNums,2);


//    tft180_show_float(5,140,Track_length,3,1);Left_dowm_PatchLeft_local_LostNums

//    tft180_show_float(45,140,pid.Speed_KI_L,3,1);
//    tft180_show_int32(65,140,nowtargetSpeed,3);

//    tft180_show_int32(95,140,Right_dowm_Patch,2);

//    tft180_show_int32(65,140,White_Column_MID,2);
//    tft180_show_int32(95,140,White_Nums,2);
//    tft180_show_int32(5,140,nowtargetSpeed,3);
//    tft180_show_int32(45,140,Find_Right_FLAG,2);
//    tft180_show_int32(85,140,Right_local_LostNums,3);

    /*-----------------------------右1---------------------------------*/
    /* 保护情况 */
    tft180_show_int32(100,5,Protection,1);
    tft180_show_int32(110,5,Stop_Car_flag,1);
//    tft180_show_int32(100,5,lost_line_project,1);
//    tft180_show_int32(108,5,End_line_Flag,1);

    /*-----------------------------右2---------------------------------*/
    tft180_show_int32(95,25,exp_time,3);
//    tft180_show_int32(100,25,Mutation_count,1);
//    tft180_show_int32(100,25,Diswitch_Key_1,1);
//    tft180_show_int32(110,25,Diswitch_Key_2,1);

    /*-----------------------------右3---------------------------------*/
    tft180_show_int32(100,45,Mutation_count,1);
}

/* 发车显示2 */
void Show_data_2(void)
{
    /*-----------------------------第一行------------------------------ */
    tft180_show_string(5,60,"Speed: ");
    tft180_show_int32(60,60,TargetSpeed,3);
    tft180_show_string(85,60,"/");
    tft180_show_int32(95,60,targetSpeed_min,3);

    /*-----------------------------第二行------------------------------ */
    tft180_show_string(5,80,"S_PI: ");
    tft180_show_int32(50,80,pid.Speed_KP_L,3);
    tft180_show_string(75,80,"/");
    tft180_show_float (85,80,pid.Speed_KI_L,1,2);

    /*-----------------------------第三行100--------------------------------*/
    tft180_show_string(5,100,"T_PD: ");
    tft180_show_int32(50,100,pid.Turn_KP,3);
    tft180_show_string(75,100,"/");
    tft180_show_float(85,100,pid.Turn_GKD,1,2);

    /*-----------------------------第四行------------------------------ */
//    tft180_show_string(5,120,"T_GKD: ");
//    tft180_show_float (60,120,pid.Turn_GKD,1,2);
}

/* 控制调参界面 */
void Show_data_3(void)
{
    /*-----------------------------第一行------------------------------ */
    tft180_show_string(15,1,"Speed: ");
    tft180_show_int32(70,1,TargetSpeed,3);

    /*-----------------------------第二行------------------------------ */
    tft180_show_string(15,20,"Speed_M: ");
    tft180_show_int32(85,20,targetSpeed_min,3);

    /*-----------------------------第三行------------------------------ */
    tft180_show_string(15,40,"S_p: ");
    tft180_show_int32(50,40,pid.Speed_KP_L,3);

    /*-----------------------------第四行------------------------------ */
    tft180_show_string(15,60,"S_i: ");
    tft180_show_float (50,60,pid.Speed_KI_L,1,2);

    /*-----------------------------第五行------------------------------ */
    tft180_show_string(15,80,"T_p: ");
    tft180_show_int32(50,80,pid.Turn_KP,3);

    /*-----------------------------第六行------------------------------ */
    tft180_show_string(15,100,"T_GKD: ");
    tft180_show_float(70,100,pid.Turn_GKD,1,2);

    /*-----------------------------第七行------------------------------ */
    tft180_show_string(15,120,"out_Mode: ");
    tft180_show_int32(60,120,out_Mode,1);
}

/* 元素调参界面-左圆环 */
void Show_data_4(void)
{
    /*-----------------------------第一行------------------------------ */
    tft180_show_string(15,1,"L_time: ");
    tft180_show_int32(80,1,Left_Ring_num,2);

    /*-----------------------------第二行------------------------------ */
    tft180_show_string(15,20,"L_G_In: ");
    tft180_show_int32(80,20,Gyro_In_Left,3);

    /*-----------------------------第三行------------------------------ */
    tft180_show_string(15,40,"L_G_Out: ");
    tft180_show_int32(80,40,Gyro_Out_Left,3);

    /*-----------------------------第四行------------------------------ */
    tft180_show_string(15,60,"L_E_In: ");
    tft180_show_int32 (80,60,Left_Enc_In,5);

    /*-----------------------------第五行------------------------------ */
    tft180_show_string(15,80,"L_E_Out: ");
    tft180_show_int32(80,80,Left_Enc_Out,5);
}

/* 元素调参界面-右圆环 */
void Show_data_5(void)
{
    /*-----------------------------第一行------------------------------ */
    tft180_show_string(15,1,"R_time: ");
    tft180_show_int32(80,1,Right_Ring_num,2);

    /*-----------------------------第二行------------------------------ */
    tft180_show_string(15,20,"R_G_In: ");
    tft180_show_int32(80,20,Gyro_In_Right,3);

    /*-----------------------------第三行------------------------------ */
    tft180_show_string(15,40,"R_G_Out: ");
    tft180_show_int32(80,40,Gyro_Out_Right,3);

    /*-----------------------------第四行------------------------------ */
    tft180_show_string(15,60,"R_E_In: ");
    tft180_show_int32 (80,60,Right_Enc_In,5);

    /*-----------------------------第五行------------------------------ */
    tft180_show_string(15,80,"R_E_Out: ");
    tft180_show_int32(80,80,Right_Enc_Out,5);
}

/* 附加页，无刷与速度模式 */
void Show_data_6(void)
{
    /*-----------------------------第一行------------------------------ */
    tft180_show_string(15,1,"Blds: ");
    tft180_show_int32(80,1,Blds_PWM_MAX,4);

    /*-----------------------------第二行------------------------------ */
    tft180_show_string(15,20,"S_Mode: ");
    tft180_show_int32(80,20,Speed_Mode,1);
}
