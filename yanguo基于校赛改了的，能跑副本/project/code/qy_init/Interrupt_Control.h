#ifndef __INTERRUPT_CONTROL_H_
#define __INTERRUPT_CONTROL_H_

#include "qy_headfile.h"

#define Left_Out_Max  7000
#define Right_Out_Max 7000

#define Start_time    200
/*
调参指南
调参顺序:
  ① STEER_K = 1.0 起步，跑一圈感受转弯力度
     - 不够力 → 加大 STEER_K
     - 打滑   → 减小 STEER_K
  ② 大弯打滑 → 加大 ANTI_SLIP（最小 0.3，最大 0.6）
     - 0.35 偏激进，0.55 偏保守
  ③ 急弯内侧轮异响/倒转 → 减小 DIFF_MAX

典型组合:
  "既要猛又不滑" → STEER_K=1.2, ANTI_SLIP=0.4, DIFF_MAX=0.7
  "保守求稳"     → STEER_K=0.8, ANTI_SLIP=0.5, DIFF_MAX=0.6
  "激进抢弯"     → STEER_K=1.4, ANTI_SLIP=0.35, DIFF_MAX=0.8
*/
// 阿克曼转向差速模型参数
// STEER_K     转向灵敏度，越大越猛（建议 0.8~1.5）
// ANTI_SLIP   内侧轮最低转速比（建议 0.35~0.55），防打滑
// DIFF_MAX    最大差速比（建议 0.65~0.85），防内侧轮倒转
#define STEER_K      1.0f
#define ANTI_SLIP    0.55f
#define DIFF_MAX     0.6f

/* 转向环 */
#define MID_ERROR_MAX       10     //当前权重下的最大终点误差(用于速度决策)

/* 速度模式 */
extern uint8 Speed_Mode;     //速度模式--1为线性，2为匀速

/* 中点误差 */
extern int16 Image_error;

/* 停车标志位 */
extern uint8 Stop_Car_flag;

/* 目标速度 */
extern int16 TargetSpeed;           //目标速度  190
extern int16 nowtargetSpeed;
extern int16 targetSpeed_min;

/* 速度环输出 */
extern int16 Speed_Left_Out;
extern int16 Speed_Right_Out;

/* 转向环输出 */
extern int16 Turn_Out;

/* 时间 */
extern uint16 time;

void Interrupt_CCU60_CH0(void);
void Speed_DecisionMaking(void);
void show_menu(void);
#endif /* CODE_INTERRUPT_CONTROL_INTERRUPT_CONTROL_H_ */
