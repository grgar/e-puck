#include "motor_led/advance_one_timer/fast_agenda/e_led.h"
#include "motor_led/advance_one_timer/fast_agenda/e_motors.h"
#include "common.h"
#include "motor_led/advance_one_timer/fast_agenda/e_agenda_fast.h"
#include "a_d/advance_ad_scan/e_prox.h"
#include "motor_led/advance_one_timer/e_led.h"
int S0 = 0;
void p3_sense(){
    S0= 5000-(e_get_ambient_light(0));
}
void p3_run() {
    e_activate_agenda(p3_sense,1000);
    while (1) {
        if (S0 < 0) {
            e_set_front_led(1);
        } else {
            e_set_front_led(0);
        }
        //Ambient Light ~1000
        if (S0 > 1000) {
            e_set_led(0, 1);
        } else {
            e_set_led(0, 0);
        }
         //Sees from around 15cm
        if (S0 > 1000) {
            e_set_led(1, 1);
        } else {
            e_set_led(1, 0);
        }
        if (S0 > 1100) {
            e_set_led(2, 1);
        } else {
            e_set_led(2, 0);
        }
        if (S0 > 1110) {
            e_set_led(3, 1);
        } else {
            e_set_led(3, 0);
        }
        //Value for it to follow torch
        if (S0 > 1120) {
            e_set_led(4, 1);
        } else {
            e_set_led(4, 0);
        }
        if (S0 > 1130) {
            e_set_led(5, 1);
        } else {
            e_set_led(5, 0);
        }
        if (S0 > 1140) {
            e_set_led(6, 1);
        } else {
            e_set_led(6, 0);
        }
        if (S0 > 1150) {
            e_set_led(7, 1);
        } else {
            e_set_led(7, 0);
        }


    }
}