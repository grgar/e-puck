#include "motor_led/advance_one_timer/fast_agenda/e_led.h"
#include "motor_led/advance_one_timer/fast_agenda/e_motors.h"
#include "common.h"
#include "motor_led/advance_one_timer/fast_agenda/e_agenda_fast.h"
#include "a_d/advance_ad_scan/e_prox.h"
#include "stdlib.h"

typedef int bool;
#define true 1
#define false 0

//Initialise values
int p8_sensors[8] = {0};
int currentSteps = 0;
int failureNo;
void p8_sense();
void p8_patrol();
void p8_chase();
int p8_any_sensor_on();
int p8_tolerance(int position);

//Scanning for light
void p8_lights() {
    e_pause_agenda(p8_lights);
    e_pause_agenda(p8_patrol);
    e_set_body_led(0);
    int k;
    for (k=0; k<=7; k++){
        e_set_led(k,0);
    }

    //Display the light pattern
    int i;
    int j;
    for (i = 0; i<5; i++) {
        delay(300000);
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
                delay(600000);
                for (j = 0; j<5; j++) {
                    delay(200000);
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
                            delay(600000);
                            break;
                    }
                }
                break;
        }
    }
    
    //Check the sensors, if one is on, initiate the chase
    p8_sense();
    e_set_steps_left(0);
    e_set_steps_right(0);
    if (p8_any_sensor_on() == 1){
        e_restart_agenda(p8_chase);
    } else {
        e_restart_agenda(p8_patrol);
    }
}

//Patrol the area (go forward and about turn)
void p8_patrol() {
    e_set_speed(1000,0);
    if (e_get_steps_left() >= 1000 || e_get_steps_right() <= -1000) {
        e_set_speed(0,-1000);
        if (e_get_steps_left() >= 1700 || e_get_steps_right() <= -1700) {
            e_set_speed(0,0);
            e_restart_agenda(p8_lights);
        }    
    }   
}

//Chase the light for a certain amount of time until it's lost the scent
void p8_chase() {

    //Turn all the lights on and check sensors
    e_set_led(8,1);
    e_set_body_led(1);
    p8_sense();
    
    //If there's a sensor on and it hasn't failed too many times, continue
    if (p8_any_sensor_on() == 1 && failureNo <= 60) {
        
        //Count the steps (functions as a while loop)
        if (abs(currentSteps) <= 10) {
            currentSteps = abs(e_get_steps_left()) + abs(e_get_steps_right());
        
            //Gives s3 priority over s2 and s2 over s1, s1 over s0
            if (p8_sensors[3] > p8_tolerance(3)) {
                e_set_speed(0, -1000);
            } else {
                if (p8_sensors[2] > p8_tolerance(2)) {
                    e_set_speed(0, -1000);
                } else {
                    if (p8_sensors[1] > p8_tolerance(1)) {
                        e_set_speed(800, -200);
                    } else {
                        if (p8_sensors[0] > p8_tolerance(0)) {
                            e_set_speed(1000, 0);
                        }
                    }
                }
            }
            
            //Gives s4 priority over s5 and s5 over s6, s6 over s7
            if (p8_sensors[4] > p8_tolerance(4)) {
                e_set_speed(0, 1000);
            } else {
                if (p8_sensors[5] > p8_tolerance(5)) {
                    e_set_speed(0, 1000);
                } else {
                    if (p8_sensors[6] > p8_tolerance(6)) {
                        e_set_speed(800, 200);
                    } else {
                        if (p8_sensors[7] > p8_tolerance(7)) {
                            e_set_speed(1000, 0);
                        }
                    }
                }
            }

        //Reset everything once it passes a set amount of steps
        } else {
            e_set_steps_left(0);
            e_set_steps_right(0);
            currentSteps = 0;
            e_set_speed(0,0);
        }
        
    //If no sensors are on and it hasn't failed too many times, increment failureNo
    } else if (p8_any_sensor_on() == 0 && failureNo <= 60) {
        failureNo = failureNo + 1;
        e_set_led(8,0);
        
    //If no sensors are on and it's failed too many times, restart the patrol
    } else {
        failureNo = 0;
        e_set_body_led(0);
        e_pause_agenda(p8_chase);
        e_restart_agenda(p8_lights);
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

//Main run function for p8
void p8_run() {
    e_activate_agenda(p8_patrol, 500);
    e_activate_agenda(p8_lights, 500);
    e_activate_agenda(p8_chase,500);
    e_pause_agenda(p8_chase);
    while (1) {
    }
}
