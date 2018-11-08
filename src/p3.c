#include "motor_led/advance_one_timer/fast_agenda/e_led.h"
#include "motor_led/advance_one_timer/fast_agenda/e_motors.h"
#include "common.h"
#include "motor_led/advance_one_timer/fast_agenda/e_agenda_fast.h"
#include "a_d/advance_ad_scan/e_prox.h"

//Array for all of the Sensor Values
int p3_sensors[8] = {0};

//Gets the value of each sensor
void p3_sense() {
    int i;
    for (i = 0; i < 8; i++) {
        p3_sensors[i] = 5000 - (e_get_ambient_light(i));
    }
}

//Finds the average light reading for the sensors
int p3_average_light(int position) {
    int i;
    int sum = 0;
    for (i = 0; i < 8; i++) {
        if (i == position) {
            continue;
        }
        sum = sum + p3_sensors[i];
    }
    return sum / 7;
}

//Adds a small amount to average light for tolerance
int p3_tolerance(int position) {
    return p3_average_light(position) + 35;
}

//Checks to see if any sensor is over the threshold for sensed light
int p3_any_sensor_on() {
    int i;
    for (i = 0; i < 8; i++) {
        if (p3_sensors[i] > p3_tolerance(i)) {
            return 1;
        }
    }
    return 0;
}

//Main run function for p3
void p3_run() {
    e_activate_agenda(p3_sense, 1000);
    while (1) {
        
        //if no sensors on, don't move
        int i;
        if (p3_any_sensor_on() == 0) {
            e_set_speed(0, 0);
        } else {
            //if both front sensors 'on', move forward at speed 1000
            if ((p3_sensors[0] > p3_tolerance(0)) && (p3_sensors[7] > p3_tolerance(7))) {
                e_set_speed(1000, 0);
            }


            //Gives s3 priority over s2 and s2 over s1, s1 over s0
            if (p3_sensors[3] > p3_tolerance(3)) {
                e_set_speed(0, -1000);
            } else {
                if (p3_sensors[2] > p3_tolerance(2)) {
                    e_set_speed(0, -1000);
                } else {

                    if (p3_sensors[1] > p3_tolerance(1)) {
                        e_set_speed(800, -200);
                    } else {
                        if (p3_sensors[0] > p3_tolerance(0)) {
                            e_set_speed(1000, 0);
                        }
                    }
                }
            }


            //Gives s4 priority over s5 and s5 over s6, s6 over s7
            if (p3_sensors[4] > p3_tolerance(4)) {
                e_set_speed(0, 1000);
            } else {
                if (p3_sensors[5] > p3_tolerance(5)) {
                    e_set_speed(0, 1000);
                } else {
                    if (p3_sensors[6] > p3_tolerance(6)) {
                        e_set_speed(800, 200);
                    } else {
                        if (p3_sensors[7] > p3_tolerance(7)) {
                            e_set_speed(1000, 0);
                        }
                    }
                }
            }

            //Aligns LEDs with appropriate sensors
            for (i = 0; i < 8; i++) {
                switch (i) {
                    case 0:
                    case 1:
                    case 2:
                        e_set_led(i, p3_sensors[i] > p3_tolerance(i) ? 1 : 0);
                        break;
                    case 3:
                        e_set_led(i+1, p3_sensors[i] > p3_tolerance(i) ? 1 : 0);
                        break;
                    case 4:
                        e_set_led(i, p3_sensors[i] > p3_tolerance(i) ? 1 : 0);
                        break;
                    case 5:
                    case 6:
                        e_set_led(i+1, p3_sensors[i] > p3_tolerance(i) ? 1 : 0);
                        break;
                    case 7:
                        e_set_led(0, p3_sensors[i] > p3_tolerance(i) ? 1 : 0);
                        break;
                }
            }
        }



    }
}