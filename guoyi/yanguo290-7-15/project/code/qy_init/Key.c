#include "Key.h"

uint8 Key_1 = 0;
uint8 Key_2 = 0;
uint8 Key_3 = 0;
uint8 Key_4 = 0;
uint8 Key_5 = 0;
uint8 Key_6 = 0;

// 开关状态变量
uint8 key1_status = 1;
uint8 key2_status = 1;
uint8 key3_status = 1;
uint8 key4_status = 1;
uint8 key5_status = 1;
uint8 key6_status = 1;

// 上一次开关状态变量
uint8 key1_last_status;
uint8 key2_last_status;
uint8 key3_last_status;
uint8 key4_last_status;
uint8 key5_last_status;
uint8 key6_last_status;

// 开关标志位
uint8 key1_flag = 0;
uint8 key2_flag = 0;
uint8 key3_flag = 0;
uint8 key4_flag = 0;
uint8 key5_flag = 0;
uint8 key6_flag = 0;

/* 拨码开关状态 */
uint8 Diswitch_Key_1 = 0;
uint8 Diswitch_Key_2 = 0;
uint8 Diswitch_Key_3 = 0;

void Key_Init(void)
{
    gpio_init(KEY1_PIN, GPI, GPIO_HIGH, GPI_PULL_UP);           // 初始化 KEY1 输入 默认低电平 下拉输入
    gpio_init(KEY2_PIN, GPI, GPIO_HIGH, GPI_PULL_UP);           // 初始化 KEY2 输入 默认低电平 下拉输入
    gpio_init(KEY3_PIN, GPI, GPIO_HIGH, GPI_PULL_UP);           // 初始化 KEY1 输入 默认低电平 下拉输入
    gpio_init(KEY4_PIN, GPI, GPIO_HIGH, GPI_PULL_UP);           // 初始化 KEY2 输入 默认低电平 下拉输入
	gpio_init(KEY5_PIN, GPI, GPIO_HIGH, GPI_PULL_UP);           // 初始化 KEY1 输入 默认低电平 下拉输入
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取所有按键状态
// 参数说明
//
// 返回参数
// 使用示例
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void Get_Key(void)
{
    Key_1 = Get_Key_1();
    Key_2 = Get_Key_2();
    Key_3 = Get_Key_3();
    Key_4 = Get_Key_4();
	Key_5 = Get_Key_5();
}

uint8 Get_Key_1(void)
{
	uint8 KeyNum = 0; // unsigned char
	//当检测到按键按下时为低电平(接地)
	key1_last_status = key1_status;
	key1_status = gpio_get_level(KEY1_PIN);
	if(!key1_status && key1_last_status)	
	{
		KeyNum = 1;	
		tft180_clear(RGB565_WHITE);
	}
	return KeyNum;
}

uint8 Get_Key_2(void)
{
   uint8 KeyNum = 0; // unsigned char
	//当检测到按键按下时为低电平(接地)
	key2_last_status = key2_status;
	key2_status = gpio_get_level(KEY2_PIN);
	if(!key2_status && key2_last_status)	
	{
		KeyNum = 1;	
		tft180_clear(RGB565_WHITE);
	}
	return KeyNum;
}

uint8 Get_Key_3(void)
{
   uint8 KeyNum = 0; // unsigned char
	//当检测到按键按下时为低电平(接地)
	key3_last_status = key3_status;
	key3_status = gpio_get_level(KEY3_PIN);
	if(!key3_status && key3_last_status)	
	{
		KeyNum = 1;	
		tft180_clear(RGB565_WHITE);
	}
	return KeyNum;
}

uint8 Get_Key_4(void)
{
   uint8 KeyNum = 0; // unsigned char
	//当检测到按键按下时为低电平(接地)
	key4_last_status = key4_status;
	key4_status = gpio_get_level(KEY4_PIN);
	if(!key4_status && key4_last_status)	
	{
		KeyNum = 1;	
		tft180_clear(RGB565_WHITE);
	}
	return KeyNum;
}

uint8 Get_Key_5(void)
{
   uint8 KeyNum = 0; // unsigned char
	//当检测到按键按下时为低电平(接地)
	key5_last_status = key5_status;
	key5_status = gpio_get_level(KEY5_PIN);
	if(!key5_status && key5_last_status)	
	{
		KeyNum = 1;	
		tft180_clear(RGB565_WHITE);
	}
	return KeyNum;
}


void Get_Diswitch_Key(void)
{
    Diswitch_Key_1 = gpio_get_level(DKEY1_PIN);
    Diswitch_Key_2 = gpio_get_level(DKEY2_PIN);
	Diswitch_Key_3 = gpio_get_level(DKEY3_PIN);
}