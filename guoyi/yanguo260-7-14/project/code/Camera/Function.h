#ifndef __FUNCTION_H_
#define __FUNCTION_H_

#include "qy_headfile.h"

uint8 Right_ContinueLine(void);
uint8 Left_ContinueLine(void);
int Count_Left_Lost(int start, int end);
int Count_Right_Lost(int start, int end);
uint8 find_extreme_Value(uint8* arr, uint8 start, uint8 end, int mode);

#endif /* CODE_CAMERA_FUNCTION_FUNCTION_H_ */
