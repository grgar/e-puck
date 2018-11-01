#include <p30F6014A.h>

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <utility/utility.h>
#include <motor_led/e_init_port.h>
#include <motor_led/advance_one_timer/fast_agenda/e_led.h>
#include <motor_led/advance_one_timer/fast_agenda/e_motors.h>
#include <motor_led/advance_one_timer/fast_agenda/e_agenda_fast.h>
#include <motor_led/e_epuck_ports.h>
#include <a_d/advance_ad_scan/e_prox.h>
#include <a_d/advance_ad_scan/e_ad_conv.h>

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
    switch (getselector()) {
        case 0:
            break;

        case 1:
            // Obstacle avoidance
            p1_run();
            break;

        case 2:
            p2_run();
            break;

        case 3:
            p3_run();
            break;

        default:
            break;
    }
}

void zero_position() {
    if (getselector() == 0) {
        e_set_front_led(1);
    } else {
        e_set_front_led(0);
    }
}

int main(void) {
    e_init_port();
    e_set_body_led(1);

    e_init_ad_scan(ALL_ADC);
    e_calibrate_ir();

    e_configure_timer(0);
    e_start_timer_processing(0);
    e_start_agendas_processing();

    e_init_motors();

    e_activate_agenda(zero_position, 1000);
    e_set_body_led(0);

    controller();

    while (1) {
        // Error
        e_set_body_led(1);
    }
    return 0;
}
