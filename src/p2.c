#include "motor_led/advance_one_timer/e_led.h"
#include "motor_led/advance_one_timer/e_motors.h"
#include "common.h"

void p2_run() {
    while (1) {
        wait(600000);

        e_set_led(2, 1);
        wait(600000);
        e_set_led(2, 0);
    }

    while (1) {
        e_set_steps_left(100);

        e_set_speed_left(100);

        e_set_steps_right(100);

        e_set_speed_right(100);

        //wait(300000);
    }
}
