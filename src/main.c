#include <p30F6014A.h>

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <motor_led/e_init_port.h>
#include <motor_led/advance_one_timer/e_led.h>
#include <motor_led/advance_one_timer/e_motors.h>
#include <motor_led/advance_one_timer/e_agenda.h>
#include <motor_led/e_epuck_ports.h>

#include "p1.h"
#include "p2.h"
#include "p3.h"
#include "p4.h"
#include "p5.h"
#include "p6.h"
#include "p7.h"
#include "p8.h"
#include "p9.h"

void controller() {
    int pos = SELECTOR0 + 2 * SELECTOR1 + 4 * SELECTOR2 + 8 * SELECTOR3;
    switch (pos) {
        case 0:
            break;

        case 1:
            // Flash LED 1
            p1_run();
            break;

        case 2:
            // Drive forward
            p2_run();
            break;
        
        case 3:
            // Flash LED 2 and scream
            p3_run();
            break;
        
        default:
            break;
    }
}

int main(void) {
    e_init_port();
    e_init_motors();
    e_set_body_led(0);

    controller();

    while (1) {
        // Error
        e_set_body_led(1);
    }
    return 0;
}
