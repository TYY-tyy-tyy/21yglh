#include "Key.h"

/* 定义按键状态 */
int Key_1 = 0;
int Key_2 = 0;
int Key_3 = 0;
int Key_4 = 0;

/* 拨码开关状态 */
int Diswitch_Key_1 = 0;
int Diswitch_Key_2 = 0;

/* 参数选择 */
int Select = 1;

/* 调参模式选择-元素 */
int Mode_Select = 1;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     按键初始化
// 参数说明
//
// 返回参数
// 使用示例
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void Key_Init(void)
{
    gpio_init(KEY1, GPI, GPIO_HIGH, GPI_PULL_UP);           // 初始化 KEY1 输入 默认低电平 下拉输入
    gpio_init(KEY2, GPI, GPIO_HIGH, GPI_PULL_UP);           // 初始化 KEY2 输入 默认低电平 下拉输入
    gpio_init(KEY3, GPI, GPIO_HIGH, GPI_PULL_UP);           // 初始化 KEY1 输入 默认低电平 下拉输入
    gpio_init(KEY4, GPI, GPIO_HIGH, GPI_PULL_UP);           // 初始化 KEY2 输入 默认低电平 下拉输入
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
//    Key_5 = Get_Key_5();
//    Key_6 = Get_Key_6();
}
int Get_Key_1(void)
{
    uint8 KeyNum = 0; // unsigned char
    //当检测到按键按下时为低电平(接地)
    if(gpio_get_level(KEY1) == 0)
    {
        system_delay_ms(20);
        while(gpio_get_level(KEY1) == 0);
        system_delay_ms(20);
        KeyNum = 1;
    }

    return KeyNum;
}
int Get_Key_2(void)
{
    uint8 KeyNum = 0; // unsigned char
    //当检测到按键按下时为低电平(接地)
    if(gpio_get_level(KEY2) == 0)
    {
        system_delay_ms(20);
        while(gpio_get_level(KEY2) == 0);
        system_delay_ms(20);
        KeyNum = 1;
    }

    return KeyNum;
}
int Get_Key_3(void)
{
    uint8 KeyNum = 0; // unsigned char
    //当检测到按键按下时为低电平(接地)
    if(gpio_get_level(KEY3) == 0)
    {
        system_delay_ms(20);
        while(gpio_get_level(KEY3) == 0);
        system_delay_ms(20);
        KeyNum = 1;
    }

    return KeyNum;
}
int Get_Key_4(void)
{
    uint8 KeyNum = 0; // unsigned char
    //当检测到按键按下时为低电平(接地)
    if(gpio_get_level(KEY4) == 0)
    {
        system_delay_ms(20);
        while(gpio_get_level(KEY4) == 0);
        system_delay_ms(20);
        KeyNum = 1;
    }

    return KeyNum;
}

void Get_Diswitch_Key(void)
{
    Diswitch_Key_1 = gpio_get_level(DKEY1);
    Diswitch_Key_2 = gpio_get_level(DKEY2);
}
