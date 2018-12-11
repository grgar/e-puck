#include <motor_led/e_init_port.h>
#include <motor_led/advance_one_timer/e_led.h>
#include <motor_led/advance_one_timer/e_motors.h>
#include <motor_led/advance_one_timer/e_agenda.h>
#include <e_uart_char.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "p1.h"
#include "p5.h"

char msg[30];

void p10_goal_obstacle() {
    p1_V v_goal = p5_move_towards_goal();
    p1_V v_glsm = p5_move_towards_goal_smooth(v_goal);
    p1_V v_obst = p1_obstacle(v_glsm);

    //sprintf(msg, "v_goal: %i,%i, v_obst: %i,%i; ",
    //v_goal.speed, v_goal.direction, v_obst.speed, v_obst.direction);
    //e_send_uart1_char(msg, strlen(msg));

    if (v_goal.direction < 0) {

        sprintf(msg, "L ");
        e_send_uart1_char(msg, strlen(msg));

        if (p1_ir.val[6] < 85) {
            v_obst.direction = -(100 - p1_ir.val[6]);

            sprintf(msg, "FL: %i", v_obst.direction);
            e_send_uart1_char(msg, strlen(msg));

        } else if (p1_ir.val[5] < 85) {
            v_obst.speed = 450;
            v_obst.direction = 100;

            sprintf(msg, "LL %i", v_obst.direction);
            e_send_uart1_char(msg, strlen(msg));
        }

        sprintf(msg, "\r\n");
        e_send_uart1_char(msg, strlen(msg));
    } else if (v_goal.direction > 0) {
        sprintf(msg, "R ");
        e_send_uart1_char(msg, strlen(msg));


        if (p1_ir.val[1] < 85) {
            v_obst.direction = 100 - p1_ir.val[1];

            sprintf(msg, "FR: %i", v_obst.direction);
            e_send_uart1_char(msg, strlen(msg));

        } else if (p1_ir.val[2] < 85) {
            v_obst.speed = 450;
            v_obst.direction = -100;

            sprintf(msg, "RR: %i", v_obst.direction);
            e_send_uart1_char(msg, strlen(msg));
        }

        sprintf(msg, "\r\n");
        e_send_uart1_char(msg, strlen(msg));
    }

    p1_v = v_obst;

    e_set_speed(p1_v.speed, p1_v.direction);
}

void p10_run() {
    e_init_uart1();
    e_set_steps_left(0);
    e_set_steps_right(0);

    p5_set_goal(50, 50);

    e_activate_agenda(p1_sense, 500);
    //e_activate_agenda(p1_drive, 500);

    e_activate_agenda(p10_goal_obstacle, 500);
    while (1) {
    }
}
