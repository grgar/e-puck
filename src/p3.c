#include "motor_led/advance_one_timer/fast_agenda/e_led.h"
#include "motor_led/advance_one_timer/fast_agenda/e_motors.h"
#include "common.h"
#include "motor_led/advance_one_timer/fast_agenda/e_agenda_fast.h"
#include "a_d/advance_ad_scan/e_prox.h"

int p3_sensors[8] = {0};
int p3_light = 1100;

void p3_sense() {
    int i;
    for (i = 0; i < 8; i++) {
        p3_sensors[i] = 5000 - (e_get_ambient_light(i));
    }
}

int p3_any_sensor_on() {
    int i;
    for (i = 0; i < 8; i++) {
        if (p3_sensors[i] > p3_light) {
            return 1;
        }
    }
    return 0;
}

void p3_run() {
    e_activate_agenda(p3_sense, 1000);
    while (1) {

        //Ambient Light ~1000
        //Sees from around 15cm
        //Value for it to follow torch
        //Turns around to face the light as it loves it

        int i;
        if (p3_any_sensor_on() == 0) {
            e_set_speed(0, 0);
        } else {
            if ((p3_sensors[0] > p3_light) || (p3_sensors[7] > p3_light)) {
                e_set_speed(800, 0);
            }
            if (p3_sensors[1] > p3_light) {
                e_set_speed(800, -200);
            }

            if (p3_sensors[3] > p3_light) {
                e_set_speed(0, -900);
            } else {
                if (p3_sensors[2] > p3_light) {
                    e_set_speed(0, -500);
                }
            }
            if (p3_sensors[4] > p3_light) {
                e_set_speed(0, 900);
            } else {
                if (p3_sensors[5] > p3_light) {
                    e_set_speed(0, 500);
                }
            }
            if (p3_sensors[6] > p3_light) {
                e_set_speed(800, 200);
            }
            for (i = 0; i < 8; i++) {
                if (p3_sensors[i] > p3_light) {
                    e_set_led(i, 1);
                } else {
                    e_set_led(i, 0);
                }
                if (p3_sensors[i] < 0) {
                    e_set_front_led(1);
                } else {
                    e_set_front_led(0);
                }
            }
        }



    }
}