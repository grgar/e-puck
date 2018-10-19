#include "motor_led/advance_one_timer/e_led.h"
#include "motor_led/advance_one_timer/e_motors.h"
#include "common.h"

void p2_run() {
    e_set_speed_left(100);
    e_set_speed_right(100);
    e_start_agendas_processing();
    while (1) {
    }

    int led = 0;
    while (1) {
        led = led^1;
        e_set_front_led(led);
        wait(300000);
    }
}
