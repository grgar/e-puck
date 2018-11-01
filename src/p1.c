#include <stdlib.h>
#include <motor_led/advance_one_timer/e_led.h>
#include <motor_led/advance_one_timer/e_motors.h>
#include <motor_led/advance_one_timer/e_agenda.h>
#include <a_d/advance_ad_scan/e_prox.h>
#include <a_d/advance_ad_scan/e_ad_conv.h>
#include "common.h"

int ir_front = 2000;

void p1_read() {
    ir_front = max(e_get_calibrated_prox(0), e_get_calibrated_prox(7));
    if (ir_front < 3000) {
        e_set_led(3, 1);
    } else {
        e_set_led(3, 0);
    }
}

void p1_drive() {
    if (ir_front < 500) {
        e_set_speed(350, 0);
    } else {
        if (e_get_calibrated_prox(0) < e_get_calibrated_prox(7)) {
            // Turn left
            e_set_speed(0, -1000);
        } else {
            // Turn right
            e_set_speed(0, 1000);
        }
    }
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
