#include "Interrupt_Control.h"
 
/* ת�� */
int16 Turn_PWM = 0;                   //����ת���PWM

/* ͣ����־λ */
uint8 Stop_Car_flag = 0;

int16 Turn_Out = 0;                 //ת�����
int16 Turn_Out_MAX = 0;             //���ת�����

/* �ٶȻ� */
int16 TargetSpeed = 200;           //Ŀ���ٶ�  190

int16 nowtargetSpeed;
int16 targetSpeed_min ;    //����ٶ�

int16 Speed_Left_Out;                    //�ٶȻ����
int16 Speed_Right_Out;

uint8 speed_mode = 0;        // 0=��� 1=ֱ�� 2=����

uint16 time = 0;
// �˵�����
uint8 menu_cursor = 0;         // 0=����1  1=����2
uint8 select_plan = 0;         // ��ǰʹ�÷���

// ����1
int16  TargetSpeed_1 = 180;
int16  Turn_KP_1    = 11;
int16  Turn_GKD_1   = 0.005;

// ����2
int16  TargetSpeed_2 = 250;
int16  Turn_KP_2    = 12.5;
int16  Turn_GKD_2   = 0.005;
float t;
int16 L = 20;
int16 K = 15;
float diff;
//-------------------------------------------------------------------------------------------------------------------
// �������     CCU60_CH0�ж�----�����ж�
// ����˵��
// ���ز���
// ʹ��ʾ��
// ��ע��Ϣ     �е�������ת�򻷡��ٶȻ����������
//------------------------------------------------------------------------------------------------------------------
void Interrupt_CCU60_CH0(void)
{
//	Test_Speed();
//	return;
	nowtargetSpeed = my_Speed;
	/* ʱ��������� */
	if(time <= 15000) 
	{
		time++;
	}
	else
	{
		COM_QY = 0;
	}
	/* ת�� ---------------------------------------------------------*/
	/* ���������ǻ��ֱ�־λ */
	use_gyro_flag();

	/* �ٶȻ� --------------------------------------------------------*/
	/* ��ȡ��������ֵ */
	Encoder_Left = Left_Encoder_Get();
	Encoder_Right = Right_Encoder_Get();

	/* ƽ��ֵ */
	Encoder_Average = (Encoder_Left + Encoder_Right) / 2;

	/* ���ڼ����������� */
	Count_Length();

	/* �������ֱ�־λ */
	use_encoder_flag();

//	/* ͣ��״̬�� */
	Stop_Car();

	/* ��ͣ��״̬ʱ�������ƣ���ͣ�� */
	if(COM_QY == 2)
	{
		/* �ٶȾ��� */
		Speed_DecisionMaking();
	}
	/* ת����� */
	Turn_Out = PID_Turn_Loc(Image_error);  //�е�����ӽ�ת��PID

	/* �õ������������ֵ */
	Turn_PWM = Turn_Out + SERVO_MOTOR_Mid; //4200~4900
	/* ת���޷� */
	if(Turn_PWM >= SERVO_MOTOR_L_MAX) Turn_PWM = SERVO_MOTOR_L_MAX;
	else if(Turn_PWM <= SERVO_MOTOR_R_MAX) Turn_PWM = SERVO_MOTOR_R_MAX;
	t = tan((Turn_PWM - SERVO_MOTOR_Mid) * 0.001176f);
    diff = ACKERMAN_COEFF * t;
	
	/* �ٶȻ���ת�� -------------------------------------------------- */
	/* �����ֱջ����  */
	if(COM_QY == 2)
	{
		Speed_Left_Out  = PID_Speed_Inc_L(nowtargetSpeed * (1.0f - diff), Encoder_Left);
        Speed_Right_Out = PID_Speed_Inc_R(nowtargetSpeed * (1.0f + diff), Encoder_Right);
    }
    else
    {
        Speed_Left_Out = 0;
        Speed_Right_Out = 0;
    }

	/* �ٶȻ���ת�� -------------------------------------------------- */
	Left_Out = Speed_Left_Out;
	Right_Out = Speed_Right_Out;

	/* �޷� */
	if(Left_Out >= Left_Out_Max) Left_Out = Left_Out_Max;
	else if(Left_Out <= -Left_Out_Max) Left_Out = -Left_Out_Max;
	if(Right_Out >= Right_Out_Max) Right_Out = Right_Out_Max;
	else if(Right_Out <= -Right_Out_Max) Right_Out = -Right_Out_Max;

////	/* �յ���ͣ�� */
//	End_Line_Stop_Car();

////	/* ���߱��� */
//	Lost_Line_Protect();
	
	/* ���� ------------------------------------------------------------*/
	/* ��� */
	if(COM_QY == 2)
	{
		pwm_set_duty(SERVO_MOTOR_FREQ,Turn_PWM);
	}
	
	/* ������� */
	Left_Motor_Speed(Left_Out);
	Right_Motor_Speed(Right_Out);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �ٶȾ���
// ����˵��
// ���ز���
// ʹ��ʾ��
// ��ע��Ϣ
//------------------------------------------------------------------------------------------------------------------
void Speed_DecisionMaking(void)
{
	// ���ԭ������������
	int16 mid_near = (Left_Line[110] + Right_Line[110]) / 2;
	int16 mid_far  = (Left_Line[reference_col_farthest + 5] + Right_Line[reference_col_farthest + 5]) / 2;

    if((Find_Left_FLAG >= Left_1) || (Find_Right_FLAG >= Right_1))
    {
        pid.Turn_KP = Ring_T_KP;//44 47
		pid.Turn_KP1 = T_KP1;
        nowtargetSpeed = my_Speed /10*9;
		pid.Turn_GKD = 0;
		pid.Turn_KD = Ring_T_KD;
		speed_mode = 2;   // ����
    }
    else if(White_Column_MID > 110 && abs(mid_near - mid_far) < 12)
    {
        pid.Turn_KP = W_T_KP;//20
		pid.Turn_KP1 = 0;
        nowtargetSpeed = my_Speed*11/10;
		pid.Turn_GKD = T_GKD;
		pid.Turn_KD = W_T_KD;
		speed_mode = 1;   // ֱ��
    }
    else
    {
        pid.Turn_KP = T_KP;      // 11.5 12.75 14
        pid.Turn_KP1 = T_KP1;
        nowtargetSpeed = my_Speed/10*9;
		pid.Turn_GKD = T_GKD/2;
		pid.Turn_KD = T_KD;
		speed_mode = 0;   // ���
    }
}

//==================== ͼ���·��˵� ====================
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