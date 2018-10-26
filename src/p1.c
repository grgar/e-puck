#include <motor_led/advance_one_timer/e_led.h>
#include <motor_led/advance_one_timer/e_motors.h>
#include <motor_led/advance_one_timer/e_agenda.h>
#include <a_d/advance_ad_scan/e_prox.h>
#include "common.h"

void p1_run() {
    e_calibrate_ir();
    while (1) {
        int i;
        for (i = 0; i < 8; i++) {
            unsigned int light = e_get_prox(i);
            if (light < 1000) {
                e_set_led(i, 1);
            } else {
                e_set_led(i, 0);
            }
        }
        delay(300000);
        for (i = 0; i < 8; i++) {
            e_set_led(i, 0);
        }
        delay(300000);
    }
}
