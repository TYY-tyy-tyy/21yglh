#include "laser.h"

uint16 laser_time = 0;
uint8 laser_cul = 0;

void laser_init(void)
{
	gpio_init(LASER_PIN_1, GPO, 0, GPO_PUSH_PULL);
	gpio_init(LASER_PIN_2, GPO, 0, GPO_PUSH_PULL);
	gpio_init(LASER_PIN_3, GPO, 0, GPO_PUSH_PULL);
	gpio_init(LASER_PIN_4, GPO, 0, GPO_PUSH_PULL);
	gpio_init(LASER_PIN_5, GPO, 0, GPO_PUSH_PULL);
}


void laser_on(gpio_pin_enum LASER_PIN)
{
	gpio_set_level(LASER_PIN, 1);
}

void laser_off(gpio_pin_enum LASER_PIN)
{
	gpio_set_level(LASER_PIN, 0);
}

void all_off(void)
{
	gpio_set_level(LASER_PIN_1, 0);
	gpio_set_level(LASER_PIN_2, 0);
	gpio_set_level(LASER_PIN_3, 0);
	gpio_set_level(LASER_PIN_4, 0);
	gpio_set_level(LASER_PIN_5, 0);
}

void all_on(void)
{
	gpio_set_level(LASER_PIN_1, 1);
	gpio_set_level(LASER_PIN_2, 1);
	gpio_set_level(LASER_PIN_3, 1);
	gpio_set_level(LASER_PIN_4, 1);
	gpio_set_level(LASER_PIN_5, 1);
}