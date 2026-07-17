#include "laser.h"

uint16 laser_time = 0;
uint8 laser_cul = 0;

void laser_init(void)
{
	gpio_init(LASER_PIN_3, GPO, 0, GPO_PUSH_PULL);
}


void laser_on(gpio_pin_enum LASER_PIN)
{
	gpio_set_level(LASER_PIN, 1);
}

void all_off(void)
{
	gpio_set_level(LASER_PIN_3, 0);
}
