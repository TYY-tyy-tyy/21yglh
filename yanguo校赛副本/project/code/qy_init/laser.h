#ifndef __LASER_H_
#define __LASER_H_

#include "qy_headfile.h"


#define LASER_PIN_1 		IO_P00
#define LASER_PIN_2 		IO_PB2
#define LASER_PIN_3 		IO_P03
#define LASER_PIN_4 		IO_P04
#define LASER_PIN_5 		IO_P01

extern uint16 laser_time;
extern uint8 laser_cul;

void laser_init(void);
void laser_on(gpio_pin_enum LASER_PIN);
void laser_off(gpio_pin_enum LASER_PIN);
void all_off(void);

#endif
