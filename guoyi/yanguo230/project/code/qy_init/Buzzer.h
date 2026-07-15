#ifndef __BUZZER_H_
#define __BUZZER_H_

#include "qy_headfile.h"

#define     Buzzer      (IO_P65)

void Buzzer_Init(void);
void Buzzer_ON(void);
void Buzzer_OFF(void);

#endif /* CODE_BUZZER_BUZZER_H_ */
