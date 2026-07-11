#ifndef __KEY_H_
#define __KEY_H_

#include "qy_headfile.h"

#define KEY1_PIN                    (IO_P96)
#define KEY2_PIN                    (IO_P67)
#define KEY3_PIN                    (IO_PB4)
#define KEY4_PIN                    (IO_PB3)
#define KEY5_PIN                    (IO_P47)
#define DKEY1_PIN                   (IO_P15)
#define DKEY2_PIN                   (IO_P14)
#define DKEY3_PIN                   (IO_P11)

/* 按键状态 */
extern uint8 Key_1;
extern uint8 Key_2;
extern uint8 Key_3;
extern uint8 Key_4;
extern uint8 Key_5;
extern uint8 Key_6;

/* 拨码开关状态 */
extern uint8 Diswitch_Key_1;
extern uint8 Diswitch_Key_2;
extern uint8 Diswitch_Key_3;

/* 参数选择 */
extern int Select;

/* 调参模式选择-元素 */
extern int Mode_Select;

void Key_Init(void);
void Get_Key(void);
uint8 Get_Key_1(void);
uint8 Get_Key_2(void);
uint8 Get_Key_3(void);
uint8 Get_Key_4(void);
uint8 Get_Key_5(void);
void Key_DecisionMaking(void);

#endif /* CODE_KEY_KEY_H_ */
