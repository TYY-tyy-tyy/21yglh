#ifndef __KEY_H_
#define __KEY_H_

#include "qy_headfile.h"

#define KEY1                    (IO_PB2)
#define KEY2                    (IO_PB3)
#define KEY3                    (IO_PB4)
#define KEY4                    (IO_P32)
#define DKEY1                   (IO_PB0)
#define DKEY2                   (IO_PB1)

/* 按键状态 */
extern int Key_1;
extern int Key_2;
extern int Key_3;
extern int Key_4;

/* 拨码开关状态 */
extern int Diswitch_Key_1;
extern int Diswitch_Key_2;

/* 参数选择 */
extern int Select;

/* 调参模式选择-元素 */
extern int Mode_Select;

void Key_Init(void);
void Get_Key(void);
int Get_Key_1(void);
int Get_Key_2(void);
int Get_Key_3(void);
int Get_Key_4(void);
void Key_DecisionMaking(void);

#endif /* CODE_KEY_KEY_H_ */
