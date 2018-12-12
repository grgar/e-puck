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
#include <e_uart_char.h>
#include <camera/fast_2_timer/e_poxxxx.h>

#include "p1.h"
#include "p2.h"
#include "p3.h"
#include "p4.h"
#include "p5.h"
#include "p6.h"
#include "p7.h"
#include "p8.h"
#include "p9.h"
#include "p10.h"

/**
 * Run correct function in c file based on selector position
 */
void controller() {
    switch (getselector()) {
        case 0:
            break;

        case 1:
            p1_run();
            break;

        case 2:
            p2_run();
            break;

        case 3:
            p3_run();
            break;

        case 4:
            p4_run();
            break;

        case 5:
            p5_run();
            break;

        case 6:
            p6_run();
            break;

        case 7:
            p7_run();
            break;

        case 8:
            p8_run();
            break;

        case 9:
            p9_run();
            break;
        
        case 10:
            p10_run();
            break;

        default:
            break;
    }
}

/**
 * Always light body LED on position 0
 * to work out where position 0 is on selector
 */
void zero_position() {
    if (getselector() == 0) {
        e_set_body_led(1);
    } else {
        e_set_body_led(0);
    }
}

/**
 * Main functionality of robot
 */
int main(void) {
    // Begin by setting ports for other operations to access e-puck
    e_init_port();
    // Show that execution has begun by turning on body LED
    // If body LED illuminates again after this initial setup process,
    // an error has occurred
    e_set_body_led(1);

    // Analog to digital setup, for IR
    e_init_ad_scan(ALL_ADC);
    // When e-puck starts, calibration is performed immediately
    // Calibration fails if attempted after agendas are prepared,
    // so unfortunately it's not possible to lateinit
    e_calibrate_ir();

    // Camera set up
	e_poxxxx_init_cam();
    // 1 pixel image in the centre of the camera
	// Captured 4x4 image, sampled to a single pixel
	e_poxxxx_config_cam((ARRAY_WIDTH - 4)/2,(ARRAY_HEIGHT - 4)/2,4,4,4,4,RGB_565_MODE);
	e_poxxxx_set_mirror(1,1);
	e_poxxxx_write_cam_registers();
    
    // Configure timer 1, 2, 3 on e-puck
    e_configure_timer(0);
    e_start_timer_processing(0);
    
    // No agenda functions should be added to the queue yet
    e_start_agendas_processing();

    // Initialising the motors must be done after agendas are ready,
    // due to implementation calling `e_activate_agenda` if __AGENDA_FAST_H__
    // is defined, which is as fast_agenda is used
    e_init_motors();

    // At any time, selecting position 0 turns on body LED, indicating 0 has
    // been reached and reset can be pressed to halt execution
    e_activate_agenda(zero_position, 1000);
    // Preliminary configuration complete, turn off body LED
    e_set_body_led(0);

    // Main execution of px.c files based on selector position begins
    controller();

    // If px_run() function returns, body LED turns on to show this
    while (1) {
        e_set_body_led(1);
    }
    return 0;
}
