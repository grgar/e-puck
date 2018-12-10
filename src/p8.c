#include "motor_led/advance_one_timer/fast_agenda/e_led.h"
#include "motor_led/advance_one_timer/fast_agenda/e_motors.h"
#include "common.h"
#include "motor_led/advance_one_timer/fast_agenda/e_agenda_fast.h"
#include "a_d/advance_ad_scan/e_prox.h"
#include "stdlib.h"

//Array for all of the Sensor Values
int p8_sensors[8] = {0};
void p8_sense();
void p8_check();
void p8_drive();
int p8_any_sensor_on();

void p8_lights() {
    int sensorOn;
    e_pause_agenda(p8_lights);
    e_pause_agenda(p8_drive);
    e_set_body_led(0);
//Love
    int i;
    int j;
    for (i = 0; i<5; i++) {
        delay(400000);
        switch (i) {
            case 0:
                e_set_led(0,1);
                break;
            case 1:
                e_set_led(1,1);
                e_set_led(7,1);
                break;
            case 2:
                e_set_led(2,1);
                e_set_led(6,1);
                break;
            case 3:
                e_set_led(3,1);
                e_set_led(5,1);
                break;
            case 4:
                e_set_led(4,1);
                delay(700000);
                for (j = 0; j<5; j++) {
                    delay(300000);
                    switch (j) {
                        case 0:
                            e_set_led(4,0);
                            break;
                        case 1:
                            e_set_led(3,0);
                            e_set_led(5,0);
                            break;
                        case 2:
                            e_set_led(2,0);
                            e_set_led(6,0);
                            break;
                        case 3:
                            e_set_led(1,0);
                            e_set_led(7,0);
                            break;
                        case 4:
                            delay(700000);
                            break;
                    }
                }
                break;
        }
    }
    p8_sense();
    sensorOn = p8_any_sensor_on();
    e_set_steps_left(0);
    e_set_steps_right(0);
    if (sensorOn == 1){
        p8_check();
    } else {
        e_restart_agenda(p8_drive);
    }
}

void p8_ree() {
    int i;
    e_set_body_led(1);
    for (i = 0; i<8; i++){
        e_set_led(i,1);
    }
}
void p8_reee() {
    int i;
    e_set_body_led(0);
    for (i = 0; i<8; i++){
        e_set_led(i,0);
    }
}

void p8_drive() {
    e_set_speed(1000,0);
    
    if (e_get_steps_left() >= 1000 || e_get_steps_right() <= -1000) {
        e_set_speed(0,-1000);
        if (e_get_steps_left() >= 1700 || e_get_steps_right() <= -1700) {
            e_set_speed(0,0);
            e_restart_agenda(p8_lights);
        }    
    }   
}

//Gets the value of each sensor
void p8_sense() {
    int i;
    for (i = 0; i < 8; i++) {
        p8_sensors[i] = 5000 - (e_get_ambient_light(i));
    }
}

//Finds the average light reading for the sensors
int p8_average_light(int position) {
    int i;
    int sum = 0;
    for (i = 0; i < 8; i++) {
        if (i == position) {
            continue;
        }
        sum = sum + p8_sensors[i];
    }
    return sum / 7;
}

//Adds a small amount to average light for tolerance
int p8_tolerance(int position) {
    return p8_average_light(position) + 35;
}

//Checks to see if any sensor is over the threshold for sensed light
int p8_any_sensor_on() {
    int i;
    for (i = 0; i < 8; i++) {
        if (p8_sensors[i] > p8_tolerance(i)) {
            return 1;
        }
    }
    return 0;
}

void p8_check() {     //CHANGE TO BOOL
    e_set_body_led(1);
    int t;
    for (t=0;t<=8;t++){
        e_set_led(t,1);
    }
    e_set_steps_left(0);
    e_set_steps_right(0);
    //(e_get_steps_left() + e_get_steps_right()) <= 1000
    
    while (1){
        //if no sensors on, don't move
        if (p8_any_sensor_on() == 0) {
            e_set_speed(0, 0);
        } else {

            //if both front sensors 'on', move forward at speed 1000
            if ((p8_sensors[0] > p8_tolerance(0)) && (p8_sensors[7] > p8_tolerance(7))) {
                    e_activate_agenda(p8_ree, 9000);
                    e_activate_agenda(p8_reee, 9000);
                    e_set_speed(1000, 0);
                }

            //if(){
                //EVERYTHING IS "NORMAL" START PACING
            //}


            //Gives s3 priority over s2 and s2 over s1, s1 over s0
            if (p8_sensors[3] > p8_tolerance(3)) {
                e_set_speed(0, -1000);
                e_set_led(1,0);
            } else {
                if (p8_sensors[2] > p8_tolerance(2)) {
                    e_set_speed(0, -1000);
                    e_set_led(2,0);
                } else {

                    if (p8_sensors[1] > p8_tolerance(1)) {
                        e_set_speed(800, -200);
                        e_set_led(3,0);
                    } else {
                        if (p8_sensors[0] > p8_tolerance(0)) {
                            e_set_speed(1000, 0);
                            e_set_led(4,0);
                        }
                    }
                }
            }


            //Gives s4 priority over s5 and s5 over s6, s6 over s7
            if (p8_sensors[4] > p8_tolerance(4)) {
                e_set_speed(0, 1000);
                e_set_led(5,0);
            } else {
                if (p8_sensors[5] > p8_tolerance(5)) {
                    e_set_speed(0, 1000);
                    e_set_led(6,0);
                } else {
                    if (p8_sensors[6] > p8_tolerance(6)) {
                        e_set_speed(800, 200);
                        e_set_led(7,0);
                    } else {
                        if (p8_sensors[7] > p8_tolerance(7)) {
                            e_set_speed(1000, 0);
                            e_set_body_led(0);
                        }
                    }
                }
            }
        }
    }
    
    //p8_sense();
    //p8_check();
    
    
   
}


//Main run function for p8
void p8_run() {
    //e_activate_agenda(p8_check, 500);
    e_activate_agenda(p8_drive, 500);
    e_activate_agenda(p8_lights, 500);
    e_activate_agenda(p8_sense, 500);
    while (1) {
    }
}
