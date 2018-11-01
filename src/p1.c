#include <motor_led/advance_one_timer/e_led.h>
#include <motor_led/advance_one_timer/e_motors.h>
#include <motor_led/advance_one_timer/e_agenda.h>
#include <a_d/advance_ad_scan/e_prox.h>
#include <a_d/advance_ad_scan/e_ad_conv.h>
#include "common.h"

int ir_front = 2000;

void p1_read() {
    ir_front = (e_get_calibrated_prox(0) + e_get_calibrated_prox(7)) / 2;
    if (ir_front < 2000) {
        e_set_led(0, 1);
    } else {
        e_set_led(0, 1);
    }
}

void p1_drive() {
    if (ir_front > 1000) {
        e_set_speed(50, 0);
    } else {
        e_set_speed(0, 0);
    }
}

void p1_run() {
    e_init_ad_scan(0);
    e_calibrate_ir();
    e_activate_agenda(p1_read, 10);
    e_activate_agenda(p1_drive, 10);
    e_start_agendas_processing();
    while (1) {
    }
}
