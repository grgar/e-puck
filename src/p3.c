#include "motor_led/advance_one_timer/fast_agenda/e_led.h"
#include "motor_led/advance_one_timer/fast_agenda/e_motors.h"
#include "common.h"
#include "motor_led/advance_one_timer/fast_agenda/e_agenda_fast.h"
#include "a_d/advance_ad_scan/e_prox.h"

int sensors[8] = {0};

void p3_sense() {
    int i;
    for (i = 0; i < 8; i++) {
        sensors[i] = 5000 - (e_get_ambient_light(i));
    }
}

int p3_any_sensor_on(){
    int i;
    for (i=0; i < 8; i++){
        if (sensors[i] > 1120){
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
        if(p3_any_sensor_on() == 0){
            e_set_speed(0,0);
        }
        else{
            if ((sensors[0] > 1120) || (sensors[7] > 1120)){
                e_set_speed(800,0);
            }
            if(sensors[1] > 1120){
                e_set_speed(800,-200);
            }
            if(sensors[2] > 1120){
                e_set_speed(800, -350);
            }
            if(sensors[3] > 1120){
                e_set_speed(800, -650);
            }
            if(sensors[4] > 1120){
                e_set_speed(800, 650);
            }
            if(sensors[5] > 1120){
                e_set_speed(800, 350);
            }
            if(sensors[6] > 1120){
                e_set_speed(800, 200);
            }
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
}