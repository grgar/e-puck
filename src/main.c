#include <p30F6014A.h>

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <motor_led/e_init_port.h>
#include <motor_led/advance_one_timer/e_led.h>
#include <motor_led/advance_one_timer/e_motors.h>
#include <motor_led/advance_one_timer/e_agenda.h>
#include <motor_led/e_epuck_ports.h>

#include "p1.h"

void wait(long time) {
    long i;
    for (i = 0; i < time; i++) {
        asm("nop");
    }
}

void controller() {
    int pos = SELECTOR0 + 2 * SELECTOR1 + 4 * SELECTOR2 + 8 * SELECTOR3;
    switch (pos) {
        case 0:
            break;
        case 1:
            p1_run();
            break;
        default:
            break;
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
