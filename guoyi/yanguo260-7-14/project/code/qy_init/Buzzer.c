#include "Buzzer.h"

void Buzzer_Init(void)
{
    gpio_init(Buzzer, GPO, GPIO_LOW, GPO_PUSH_PULL);
//    gpio_toggle_level(Buzzer);
}

void Buzzer_ON(void)
{
    gpio_set_level(Buzzer,GPIO_HIGH);
}

void Buzzer_OFF(void)
{
    gpio_set_level(Buzzer,GPIO_LOW);
}
