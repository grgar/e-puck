#include "motor_led/advance_one_timer/fast_agenda/e_led.h"
#include "motor_led/advance_one_timer/fast_agenda/e_motors.h"
#include "common.h"
#include "motor_led/advance_one_timer/fast_agenda/e_agenda_fast.h"
#include "a_d/advance_ad_scan/e_prox.h"
#include "motor_led/advance_one_timer/e_led.h"

int sensors[8] = {0};

void p3_sense() {
    int i;
    for (i = 0; i < 8; i++) {
        sensors[i] = 5000 - (e_get_ambient_light(i));
    }
}

void p3_run() {
    e_activate_agenda(p3_sense, 1000);
    while (1) {

        //Ambient Light ~1000
        //Sees from around 15cm
        //Value for it to follow torch

        int i;
        for (i = 0; i < 8; i++) {
            if (sensors[i] > 1120) {
                e_set_led(i, 1);
            } else {
                e_set_led(i, 0);
            }
            if (sensors[i] < 0) {
                e_set_front_led(1);
            } else {
                e_set_front_led(0);
            }
        }


    }
}