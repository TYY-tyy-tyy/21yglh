#ifndef __LASER_H_
#define __LASER_H_

#include "qy_headfile.h"

//#define LASER_PIN_1 		
//#define LASER_PIN_2 		
#define LASER_PIN_3 		IO_P67
//#define LASER_PIN_4 		
//#define LASER_PIN_5 		

extern uint16 laser_time;
extern uint8 laser_cul;

void laser_init(void);
void laser_on(gpio_pin_enum LASER_PIN);
void all_off(void);

#endif