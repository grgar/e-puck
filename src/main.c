#include "xc.h"
#include "motor_led/e_init_port.h"
#include "motor_led/e_epuck_ports.h"
#include "motor_led/e_led.h"
#include "motor_led/e_motors.h"

void wait(long time) {
    long i;
    for (i = 0; i < time; i++) {
        asm("nop");
    }
}

int main(void) {
    e_init_port();

    int led = 0;
    while (1) {
        LED0 = led = led^1;
        wait(300000);
    }

    e_init_motors();
    while (1) {
        e_set_body_led(led);
        LED0 = led = led^1;
        e_set_speed_left(1000);
        e_set_steps_left(1000);
        e_set_speed_right(1000);
        e_set_steps_right(1000);
        wait(300000);
    }
    return 0;
}
