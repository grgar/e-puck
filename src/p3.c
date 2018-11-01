#include "motor_led/advance_one_timer/fast_agenda/e_led.h"
#include "motor_led/advance_one_timer/fast_agenda/e_motors.h"
#include "common.h"
#include "motor_led/advance_one_timer/fast_agenda/e_agenda_fast.h"
#include <p30F6014A.h>
#include <motor_led/e_init_port.h>

void p3_run() {
    e_init_port();
    e_init_motors();

    int led = 0;
    e_set_speed_left(1000);
    e_set_speed_right(1000);
    e_start_agendas_processing();
    while (1) {
        led = led^1;
        e_set_body_led(led);
        e_set_led(0, led);

        delay(300000);
    }
}