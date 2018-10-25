#include "motor_led/advance_one_timer/e_led.h"
#include "motor_led/advance_one_timer/e_motors.h"
#include "motor_led/advance_one_timer/e_agenda.h"
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
        delay(300000);
    }
}
