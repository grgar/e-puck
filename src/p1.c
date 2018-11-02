#include <stdlib.h>
#include <a_d/advance_ad_scan/e_prox.h>
#include <motor_led/advance_one_timer/fast_agenda/e_led.h>
#include <motor_led/advance_one_timer/fast_agenda/e_motors.h>
#include <motor_led/advance_one_timer/fast_agenda/e_agenda_fast.h>
#include "common.h"

// IR percentages, 100% clear, 0% blocked
int p1_irv[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int p1_ir_front = 0;

void p1_ir() {
    int i;
    for (i = 0; i < 8; i++) {
        p1_irv[i] = 100 - (min(e_get_calibrated_prox(i), 3000) / 30);
    }
    p1_ir_front = max(p1_irv[0], p1_irv[7]);
    if (p1_ir_front > 90) {
        e_set_led(3, 1);
    } else {
        e_set_led(3, 0);
    }
}

int p1_direction_adjust = 0;

void p1_obstacle() {
    if (p1_ir_front > 90) {
        return;
    }

    /*if (!(ir[1] > 85 || ir[6] > 85)) {
        // Trapped, reverse
        e_set_speed(-450, (ir[1] > ir[6]) ? 100 : -100);
        return;
    }*/

    if (p1_irv[0] > p1_irv[7]) {
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
    if (p1_ir_front > 90) {
        e_set_speed(350, p1_direction_adjust);
    }
}

void p1_direction() {
    if (!(p1_irv[1] < 85 || p1_irv[2] < 85 || p1_irv[5] < 85 || p1_irv[6] < 85)) {
        p1_direction_adjust = 0;
        return;
    }

    // Front-side
    if (p1_irv[1] < 85) {
        p1_direction_adjust = 100;
    }
    if (p1_irv[6] < 85) {
        p1_direction_adjust = -100;
    }
    if (p1_irv[1] < 85 || p1_irv[6] < 85) {
        return;
    }

    // Side
    if (p1_irv[2] < 85) {
        p1_direction_adjust = 50;
    }
    if (p1_irv[5] < 85) {
        p1_direction_adjust = -50;
    }
}

void p1_run() {
    e_activate_agenda(p1_ir, 500);
    e_activate_agenda(p1_drive, 1000);
    e_activate_agenda(p1_obstacle, 1000);
    e_activate_agenda(p1_direction, 1000);
    while (1) {
    }
}
