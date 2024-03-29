#include <motor_led/advance_one_timer/fast_agenda/e_led.h>
#include <motor_led/advance_one_timer/fast_agenda/e_motors.h>
#include <motor_led/advance_one_timer/fast_agenda/e_agenda_fast.h>
#include <a_d/advance_ad_scan/e_prox.h>
#include "common.h"
#include "p3.h"

//Fear: The robot will take sensor inputs from all directions. When
//one of these values is above average, it knows there is a light
//present in that direction. It will consequently move away from
//the light as quickly as possible.

/**
 * Adds a small amount to average light for tolerance
 * @param position to check
 * @return new light value for position
 */
int p9_tolerance(int position) {
    return p3_average_light(position) + 45;
}

/**
 * Checks to see if any sensor is over the threshold for sensed light
 * @return 1 if any sensor on, otherwise 0
 */
int p9_any_sensor_on() {
    int i;
    for (i = 0; i < 8; i++) {
        if (p3_sensors[i] > p9_tolerance(i)) {
            return 1;
        }
    }
    return 0;
}

/**
 * Main run function for p3
 */
void p9_run() {
    e_activate_agenda(p3_sense, 1000);
    while (1) {

        //if no sensors on, don't move
        int i;
        if (p9_any_sensor_on() == 0) {
            e_set_speed(0, 0);
        } else {
            //if both front sensors 'on', move backwards at speed 1000
            if ((p3_sensors[0] > p9_tolerance(0)) && (p3_sensors[7] > p9_tolerance(7))) {
                e_set_speed(-1000, 0);
            }

            //Gives s3 priority over s2 and s2 over s1, s1 over s0
            if (p3_sensors[3] > p9_tolerance(3)) {
                e_set_speed(1000, 0);
            } else {
                if (p3_sensors[2] > p9_tolerance(2)) {
                    e_set_speed(0, 1000);
                } else {

                    if (p3_sensors[1] > p9_tolerance(1)) {
                        e_set_speed(800, 200);
                    } else {
                        if (p3_sensors[0] > p9_tolerance(0)) {
                            e_set_speed(-1000, 0);
                        }
                    }
                }
            }

            //Gives s4 priority over s5 and s5 over s6, s6 over s7
            if (p3_sensors[4] > p9_tolerance(4)) {
                e_set_speed(0, -1000);
            } else {
                if (p3_sensors[5] > p9_tolerance(5)) {
                    e_set_speed(0, -1000);
                } else {
                    if (p3_sensors[6] > p9_tolerance(6)) {
                        e_set_speed(-800, -200);
                    } else {
                        if (p3_sensors[7] > p9_tolerance(7)) {
                            e_set_speed(-1000, 0);
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
                        e_set_led(i, p3_sensors[i] > p9_tolerance(i) ? 1 : 0);
                        break;
                    case 3:
                        e_set_led(i + 1, p3_sensors[i] > p9_tolerance(i) ? 1 : 0);
                        break;
                    case 4:
                        e_set_led(i, p3_sensors[i] > p9_tolerance(i) ? 1 : 0);
                        break;
                    case 5:
                    case 6:
                        e_set_led(i + 1, p3_sensors[i] > p9_tolerance(i) ? 1 : 0);
                        break;
                    case 7:
                        e_set_led(0, p3_sensors[i] > p9_tolerance(i) ? 1 : 0);
                        break;
                }
            }
        }
    }
}