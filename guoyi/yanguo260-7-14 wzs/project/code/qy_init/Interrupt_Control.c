#include "Interrupt_Control.h"
 
/* ×ªï¿½ï¿½ */
int16 Turn_PWM = 0;                   //ï¿½ï¿½ï¿½ï¿½×ªï¿½ï¿½ï¿½PWM

/* Í£ï¿½ï¿½ï¿½ï¿½Ö¾Î» */
uint8 Stop_Car_flag = 0;

int16 Turn_Out = 0;                 //×ªï¿½ï¿½ï¿½ï¿½ï¿?
int16 Turn_Out_MAX = 0;             //ï¿½ï¿½ï¿½×ªï¿½ï¿½ï¿½ï¿½ï¿½

/* ï¿½Ù¶È»ï¿½ */
int16 TargetSpeed = 200;           //Ä¿ï¿½ï¿½ï¿½Ù¶ï¿½  190

int16 nowtargetSpeed;
int16 targetSpeed_min ;    //ï¿½ï¿½ï¿½ï¿½Ù¶ï¿?

int16 Speed_Left_Out;                    //ï¿½Ù¶È»ï¿½ï¿½ï¿½ï¿?
int16 Speed_Right_Out;

uint8 speed_mode = 0;        // 0=ÍäµÀ 1=Ö±µÀ 2=»·µº 3=´óÍä

int16 variance_max = 169;//169
int16 variance_max2 = 225;
uint16 time = 0;
// ï¿½Ëµï¿½ï¿½ï¿½ï¿½ï¿½
uint8 menu_cursor = 0;         // 0=ï¿½ï¿½ï¿½ï¿½1  1=ï¿½ï¿½ï¿½ï¿½2
uint8 select_plan = 0;         // ï¿½ï¿½Ç°Ê¹ï¿½Ã·ï¿½ï¿½ï¿½

// ï¿½ï¿½ï¿½ï¿½1
int16  TargetSpeed_1 = 180;
int16  Turn_KP_1    = 11;
int16  Turn_GKD_1   = 0.005;

// ï¿½ï¿½ï¿½ï¿½2
int16  TargetSpeed_2 = 250;
int16  Turn_KP_2    = 12.5;
int16  Turn_GKD_2   = 0.005;
float t;
int16 L = 20;
int16 K = 15;
float diff;
//-------------------------------------------------------------------------------------------------------------------
// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?    CCU60_CH0ï¿½Ð¶ï¿½----ï¿½ï¿½ï¿½ï¿½ï¿½Ð¶ï¿½
// ï¿½ï¿½ï¿½ï¿½Ëµï¿½ï¿½
// ï¿½ï¿½ï¿½Ø²ï¿½ï¿½ï¿½
// Ê¹ï¿½ï¿½Ê¾ï¿½ï¿½
// ï¿½ï¿½×¢ï¿½ï¿½Ï¢     ï¿½Ðµï¿½ï¿½ï¿½î´¦ï¿½ï¿½ï¿½ï¿½×ªï¿½ò»·¡ï¿½ï¿½Ù¶È»ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
//------------------------------------------------------------------------------------------------------------------
void Interrupt_CCU60_CH0(void)
{
//	Test_Speed();
//	return;
	/* Ê±ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?*/
//	if(time <= 15000) 
//	{
//		time++;
//	}
//	else
//	{
//		COM_QY = 0;
//	}
	/* ×ªï¿½ï¿½ ---------------------------------------------------------*/
	/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ç»ï¿½ï¿½Ö±ï¿½Ö¾Î» */
	use_gyro_flag();

	/* ï¿½Ù¶È»ï¿½ --------------------------------------------------------*/
	/* ï¿½ï¿½È¡ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Öµ */
	Encoder_Left = Left_Encoder_Get();
	Encoder_Right = Right_Encoder_Get();

	/* Æ½ï¿½ï¿½Öµ */
	Encoder_Average = (Encoder_Left + Encoder_Right) / 2;

	/* ï¿½ï¿½ï¿½Ú¼ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ */
	Count_Length();

	/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö±ï¿½Ö¾Î» */
	use_encoder_flag();

//	/* Í£ï¿½ï¿½×´Ì¬ï¿½ï¿½ */
	Stop_Car();

	/* ï¿½ï¿½Í£ï¿½ï¿½×´Ì¬Ê±ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æ£ï¿½ï¿½ï¿½Í£ï¿½ï¿½ */
	if(COM_QY == 2 || COM_QY == 3)
	{
		/* ï¿½Ù¶È¾ï¿½ï¿½ï¿½ */
		Speed_DecisionMaking();
	}
	/* ×ªï¿½ï¿½ï¿½ï¿½ï¿?*/
	Turn_Out = PID_Turn_Loc(Image_error);  //ï¿½Ðµï¿½ï¿½ï¿½ï¿½ï¿½Ó½ï¿½×ªï¿½ï¿½PID

	/* ï¿½Ãµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö?*/
	Turn_PWM = Turn_Out + SERVO_MOTOR_Mid; //4200~4900
	/* ×ªï¿½ï¿½ï¿½Þ·ï¿½ */
	if(Turn_PWM >= SERVO_MOTOR_L_MAX) Turn_PWM = SERVO_MOTOR_L_MAX;
	else if(Turn_PWM <= SERVO_MOTOR_R_MAX) Turn_PWM = SERVO_MOTOR_R_MAX;
	t = tan((Turn_PWM - SERVO_MOTOR_Mid) * 0.001176f);
    diff = ACKERMAN_COEFF * t;
	
	/* ï¿½Ù¶È»ï¿½ï¿½ï¿½×ªï¿½ï¿½ -------------------------------------------------- */
	/* ï¿½ï¿½ï¿½ï¿½ï¿½Ö±Õ»ï¿½ï¿½ï¿½ï¿? */
	if(COM_QY == 2)
	{
		Speed_Left_Out  = PID_Speed_Inc_L(nowtargetSpeed * (1.0f - diff), Encoder_Left);
        Speed_Right_Out = PID_Speed_Inc_R(nowtargetSpeed * (1.0f + diff), Encoder_Right);
    }
	else if(COM_QY == 3)
	{
       Speed_Left_Out  = PID_Speed_Inc_L(nowtargetSpeed * (1.0f - diff), Encoder_Left);
       Speed_Right_Out = PID_Speed_Inc_R(nowtargetSpeed * (1.0f + diff), Encoder_Right);
	}
	else if(COM_QY == 4)
	{
		COM_QY = 0;
	}
    else
    {
        Speed_Left_Out = 0;
        Speed_Right_Out = 0;
    }

	/* ï¿½Ù¶È»ï¿½ï¿½ï¿½×ªï¿½ï¿½ -------------------------------------------------- */
	Left_Out = Speed_Left_Out;
	Right_Out = Speed_Right_Out;

	/* ï¿½Þ·ï¿½ */
	if(Left_Out >= Left_Out_Max) Left_Out = Left_Out_Max;
	else if(Left_Out <= -Left_Out_Max) Left_Out = -Left_Out_Max;
	if(Right_Out >= Right_Out_Max) Right_Out = Right_Out_Max;
	else if(Right_Out <= -Right_Out_Max) Right_Out = -Right_Out_Max;

////	/* ï¿½Õµï¿½ï¿½ï¿½Í£ï¿½ï¿½ */
//	End_Line_Stop_Car();

////	/* ï¿½ï¿½ï¿½ß±ï¿½ï¿½ï¿½ */
//	Lost_Line_Protect();
	
	/* ï¿½ï¿½ï¿½ï¿½ ------------------------------------------------------------*/
	/* ï¿½ï¿½ï¿*/
	pwm_set_duty(SERVO_MOTOR_FREQ,Turn_PWM);
	
	/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?*/
	Left_Motor_Speed(Left_Out);
	Right_Motor_Speed(Right_Out);
}

//-------------------------------------------------------------------------------------------------------------------
// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?    ï¿½Ù¶È¾ï¿½ï¿½ï¿½
// ï¿½ï¿½ï¿½ï¿½Ëµï¿½ï¿½
// ï¿½ï¿½ï¿½Ø²ï¿½ï¿½ï¿½
// Ê¹ï¿½ï¿½Ê¾ï¿½ï¿½
// ï¿½ï¿½×¢ï¿½ï¿½Ï¢
//------------------------------------------------------------------------------------------------------------------
void Speed_DecisionMaking(void)
{
	static uint8 mode_confirm_cnt = 0;
	static uint8 candidate_mode = 0;
	int16 i;
    int16 sum = 0, sum_sq = 0;
    int16 mean, variance;
    uint8 n = 0;
	// ÔÝ´æºòÑ¡ PID ²ÎÊý
    int16 tmp_KP, tmp_KP1, tmp_GKD, tmp_KD, tmp_Speed;
    uint8 tmp_mode;

    // Ã¿¸ô5ÐÐ²ÉÑù Road_Wide£¬ÓÃ·½²îÅÐ¶ÏÖÐ¼äÏß±ä»¯
    for(i = MT9V03X_H - 10; i >= reference_col_farthest + 5; i -= 5)
    {
        if(Left_Line[i] > 8 && Right_Line[i] < (MT9V03X_W - 8))
        {
            sum += Road_Wide[i];
            sum_sq += Road_Wide[i] * Road_Wide[i];
            n++;
        }
    }

    if((Find_Left_FLAG >= Left_1) || (Find_Right_FLAG >= Right_1))
    {
        tmp_KP    = Ring_T_KP;//44 47
		tmp_KP1   = T_KP1-1;
        tmp_Speed = my_Speed /10*85/10;
		tmp_GKD   = 0;
		tmp_KD    = Ring_T_KD;
		tmp_mode  = 2;   // »·µº
    }
	else if(n >= 4 && White_Column_MID > 110)
    {
		mean = sum / (int16)n;
        variance = sum_sq / (int16)n - mean * mean;
		
		if(variance < variance_max || variance > 400)
		{
			tmp_KP    = W_T_KP;//20
			tmp_KP1   = 0;
			tmp_Speed = my_Speed*11/10;
			tmp_GKD   = T_GKD;
			tmp_KD    = W_T_KD;
			tmp_mode  = 1;   // Ö±µÀ
		}
		else if(variance_max< variance < variance_max2)
		{
			tmp_KP    = (T_KP+W_T_KP)/2 ;//20
			tmp_KP1   = 1;
			tmp_Speed = my_Speed;
			tmp_GKD   = T_GKD;
			tmp_KD    = (T_KD+W_T_KD)/2;
			tmp_mode  = 0;   //ÆÕÍ¨ÍäµÀ
		}
		else
		{
			tmp_KP    = T_KP;      // 11.5 12.75 14
			tmp_KP1   = T_KP1;
			tmp_Speed = my_Speed/10*85/10;
			tmp_GKD   = T_GKD*2/3;
			tmp_KD    = T_KD;
			tmp_mode  = 3;   // ´óÍäµÀ
		}
	}
	else
	{
		tmp_KP    = T_KP;      // 11.5 12.75 14
		tmp_KP1   = T_KP1;
		tmp_Speed = my_Speed/10*88/10;
		tmp_GKD   = T_GKD*2/3;
		tmp_KD    = T_KD;
		tmp_mode  = 3;   // ´óÍäµÀ
	}
	
	/* ---- È·ÈÏÂß¼­£ºÁ¬Ðø3Ö¡Í¬Ä£Ê½²ÅÕæÕýÇÐ»» ---- */
    if(tmp_mode != candidate_mode)
    {
        candidate_mode = tmp_mode;
        mode_confirm_cnt = 1;   // µÚÒ»´Î³öÏÖ£¬¼ÆÊýÆ÷´Ó1¿ªÊ¼
    }
    else
    {
        mode_confirm_cnt++;
    }

    if(mode_confirm_cnt >= 3)
    {
        /* È·ÈÏÇÐ»»£¬¸üÐÂÊµ¼Ê²ÎÊý */
        speed_mode = candidate_mode;
        pid.Turn_KP   = tmp_KP;
        pid.Turn_KP1  = tmp_KP1;
        nowtargetSpeed = tmp_Speed;
        pid.Turn_GKD  = tmp_GKD;
        pid.Turn_KD   = tmp_KD;
    }
    // Î´Âú3Ö¡ ¡ú ±£³ÖÉÏÒ»Ä£Ê½µÄ²ÎÊý²»±ä
}

//==================== Í¼ï¿½ï¿½ï¿½Â·ï¿½ï¿½Ëµï¿½ ====================
void show_menu(void)
{
    tft180_show_string(0, 65, "Plan:");
    
    if(menu_cursor == 0)
        tft180_show_string(40, 65, ">1");
    else
        tft180_show_string(40, 65, " 1");
        
    if(menu_cursor == 1)
        tft180_show_string(60, 65, ">2");
    else
        tft180_show_string(60, 65, " 2");

    tft180_show_string(0, 80, "Speed:");
    tft180_show_string(0, 95, "KP:");
    tft180_show_string(MT9V03X_W / 2, 0, "GKD:");

    if(select_plan == 1)
    {
        tft180_show_int16(45, 80, TargetSpeed_1);
        tft180_show_float(24, 95, Turn_KP_1, 2, 2);
		tft180_show_float(MT9V03X_W / 2, 16, Turn_GKD_1, 1, 3);
    }
    else
    {
        tft180_show_int16(45, 80, TargetSpeed_2);
        tft180_show_float(24, 95, Turn_KP_2, 2, 2);
        tft180_show_float(MT9V03X_W / 2, 16, Turn_GKD_2, 1, 3);
    }

    if(COM_QY == 1)
        tft180_show_string(0, 110, "RUN");
    else
        tft180_show_string(0, 110, "STOP");
}