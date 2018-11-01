#include <motor_led/advance_one_timer/e_led.h>
#include <motor_led/advance_one_timer/e_motors.h>
#include <motor_led/advance_one_timer/e_agenda.h>
#include <a_d/advance_ad_scan/e_prox.h>
#include <a_d/advance_ad_scan/e_ad_conv.h>
#include "common.h"

int ir_front = 2000;

void p1_read() {
    ir_front = (e_get_calibrated_prox(0) + e_get_calibrated_prox(7)) / 2;
    if (ir_front < 3000) {
        e_set_led(3, 1);
    } else {
        e_set_led(3, 0);
    }
}

void p1_drive() {
    int ir_drive = 3000;
    if (ir_front < 3000) {
        ir_drive = ir_front;
    }
    ir_drive = 1000 - (ir_drive / 3);
    if (ir_drive < 350) {
        ir_drive = 0;
    }
    e_set_speed(ir_drive, 0);
}

void p1_run() {
    e_init_ad_scan(0);
    e_calibrate_ir();
    e_activate_agenda(p1_read, 1000);
    e_activate_agenda(p1_drive, 1000);
    e_start_agendas_processing();
    while (1) {
    }
}
