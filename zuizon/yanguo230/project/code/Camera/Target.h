#ifndef __TARGET_H_
#define __TARGET_H_

#include "qy_headfile.h"

extern uint8 Target_EER;

void Find_Target1(void);
void Find_Target2(int16 p1,int16 p2);
void LowerCameraExposure(void);
uint8 verify_hollow_target_by_column(uint8 col);

#endif
