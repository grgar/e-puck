#include <stdlib.h>
#include <motor_led/advance_one_timer/e_led.h>
#include <motor_led/advance_one_timer/e_motors.h>
#include <motor_led/advance_one_timer/e_agenda.h>
#include <a_d/advance_ad_scan/e_prox.h>
#include <a_d/advance_ad_scan/e_ad_conv.h>
#include "common.h"

// IR percentages, 100% clear, 0% blocked
int ir[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int ir_front = 0;

void p1_ir() {
    int i;
    for (i = 0; i < 8; i++) {
        ir[i] = 100 - (min(e_get_calibrated_prox(i), 3000) / 30);
    }
    ir_front = max(ir[0], ir[7]);
}

int direction_adjust = 0;

void p1_obstacle() {
    if (ir_front > 85) {
        return;
    }
    
    /*if (!(ir[1] > 85 || ir[6] > 85)) {
        // Trapped, reverse
        e_set_speed(-450, (ir[1] > ir[6]) ? 100 : -100);
        return;
    }*/

    if (ir[0] > ir[7]) {
        // Turn left
        e_set_speed(0, -1000);
        return;
    } else {
        // Turn right
        e_set_speed(0, 1000);
        return;
    }
}

void p1_drive() {
    if (ir_front > 85) {
        e_set_speed(350, direction_adjust);
    }
}

void p1_direction() {
    if (!(ir[1] < 85 || ir[2] < 85 || ir[5] < 85 || ir[6] < 85)) {
        direction_adjust = 0;
        return;
    }
    
    // Front-side
    if (ir[1] < 85) {
        direction_adjust = 100;
    }
    if (ir[6] < 85) {
        direction_adjust = -100;
    }
    if (ir[1] < 85 || ir[6] < 85) {
        return;
    }

    // Side
    if (ir[2] < 85) {
        direction_adjust = 50;
    }
    if (ir[5] < 85) {
        direction_adjust = -50;
    }
}

void p1_run() {
    e_init_ad_scan(0);
    e_calibrate_ir();
    e_activate_agenda(p1_ir, 500);
    delay(50000);
    e_activate_agenda(p1_drive, 1000);
    e_activate_agenda(p1_obstacle, 1000);
    e_activate_agenda(p1_direction, 1000);
    e_start_agendas_processing();
    while (1) {
    }
}
