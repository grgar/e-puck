#include "motor_led/e_init_port.h"
#include "motor_led/e_epuck_ports.h"
#include "motor_led/advance_one_timer/e_motors.h"
#include "motor_led/advance_one_timer/e_agenda.h"
#include "uart/e_uart_char.h"
#include "motor_led/advance_one_timer/e_led.h"
#include "camera/fast_2_timer/e_poxxxx.h"

#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "./p6.h"
#include <e_uart_char.h>

typedef int bool;
#define true 1
#define false 0

void p6_drive() {
    e_set_speed(1000, 0);
}

void p6_stop() {
    e_set_speed(0, 0);
}

void p6_consider_stopping() {
    int steps = abs(e_get_steps_left() + e_get_steps_right());
   
    e_init_uart1();
    char data[4] = "In\n";
    e_send_uart1_char(data, 4);
        
    e_init_uart1();
    char message[50];
    sprintf(message, "This is a test: %d\n", steps);
    e_send_uart1_char(message, strlen(message));
    
    if (steps > 1000) {
        e_init_uart1();
        char data[7] = "hello\n";
        e_send_uart1_char(data, 7);

        p6_stop();
    }
}

void p6_run(void) {
    p6_drive();
    
    e_activate_agenda(p6_consider_stopping, 500);
    
	while (1) {
    }
}

