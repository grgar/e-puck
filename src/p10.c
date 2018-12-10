#include <motor_led/e_init_port.h>
#include <motor_led/advance_one_timer/fast_agenda/e_led.h>
#include <motor_led/advance_one_timer/fast_agenda/e_motors.h>
#include <motor_led/advance_one_timer/fast_agenda/e_agenda_fast.h>
#include <e_uart_char.h>
#include <stdio.h>
#include "common.h"
#include "p1.h"
#include "p5.h"

char msg[30];

void p10_goal_obstacle() {
    p1_V v = p5_move_towards_goal();
    p1_v = p1_obstacle(v);
    sprintf(msg, "v: %i,%i, p1_v: %i,%i\r\n", v.speed, v.direction, p1_v.speed, p1_v.direction);
    e_send_uart1_char(msg, strlen(msg));
    e_set_speed(p1_v.speed, p1_v.direction);
}

void p10_run() {
    e_init_uart1();
    e_set_steps_left(0);
    e_set_steps_right(0);

    sprintf(msg, "v: %i,%i, p1_v: %i,%i\r\n", v.speed, v.direction, p1_v.speed, p1_v.direction);
    e_send_uart1_char(msg, strlen(msg));
    p5_set_goal(50, 50);

    e_activate_agenda(p1_sense, 500);
    //e_activate_agenda(p1_drive, 500);

    e_activate_agenda(p10_goal_obstacle, 500);
    while (1) {
    }
}
