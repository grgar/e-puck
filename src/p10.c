#include <motor_led/e_init_port.h>
#include <motor_led/advance_one_timer/fast_agenda/e_led.h>
#include <motor_led/advance_one_timer/fast_agenda/e_motors.h>
#include <motor_led/advance_one_timer/fast_agenda/e_agenda_fast.h>
#include <e_uart_char.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "p1.h"
#include "p5.h"

char msg[30];

p1_V p10_escape(p1_V v_goal, p1_V v_obst) {
    if (v_goal.direction > 0) {
        sprintf(msg, "L ");
        e_send_uart1_char(msg, strlen(msg));

        if (p1_ir.val[6] < 85) {
            v_obst.direction = -10 * (100 - p1_ir.val[6]);

            sprintf(msg, "FL: %i", v_obst.direction);
            e_send_uart1_char(msg, strlen(msg));

        } else if (p1_ir.val[5] < 85 || p1_ir.val[4] < 85) {
            v_obst.speed = 450;
            v_obst.direction = -100;

            sprintf(msg, "LL %i", v_obst.direction);
            e_send_uart1_char(msg, strlen(msg));
        }

        sprintf(msg, "\r\n");
        e_send_uart1_char(msg, strlen(msg));

    } else if (v_goal.direction < 0) {
        sprintf(msg, "R ");
        e_send_uart1_char(msg, strlen(msg));

        if (p1_ir.val[1] < 85) {
            v_obst.direction = -10 * (100 - p1_ir.val[1]);

            sprintf(msg, "FR: %i", v_obst.direction);
            e_send_uart1_char(msg, strlen(msg));

        } else if (p1_ir.val[2] < 85 || p1_ir.val[3] < 85) {
            v_obst.speed = 450;
            v_obst.direction = 100;

            sprintf(msg, "RR: %i", v_obst.direction);
            e_send_uart1_char(msg, strlen(msg));
        }

        sprintf(msg, "\r\n");
        e_send_uart1_char(msg, strlen(msg));
    }

    return v_obst;
}

/*void p10_goal() {
    p5_compute_metrics();
    p10_gv = p5_move_towards_goal();
    p10_gvs = p5_move_towards_goal_smooth(p10_gv);
}

void p10_obstacle() {
    //p1_obstacle_run();
    //p1_v = p10_escape(p10_gv, p1_v);
    //if (p1_v.direction == 0) {
    //e_set_speed(p10_gvs.direction, p10_gvs.speed);
    //} else {
    p1_V v;
    v.speed = 1000;
    v.direction = 0;
    p1_v = p1_obstacle(v);

    e_set_speed(p1_v.direction, p1_v.speed);
    //}

    //p5_compute_metrics();
    //sprintf(msg, "%i %i \r\n", stepsL, stepsR);
    //e_send_uart1_char(msg, strlen(msg));
}

void p10_run() {
    //e_init_uart1();
    //e_set_steps_left(0);
    //e_set_steps_right(0);

    //p5_set_goal(50, 50);

    e_activate_agenda(p1_sense, 500);
    //e_activate_agenda(p10_goal, 500);
    //e_activate_agenda(p10_obstacle, 500);
    while (1) {
        p1_V v;
        v.speed = 1000;
        v.direction = 0;
        v = p1_obstacle(v);

        e_set_speed(v.direction, v.speed);

    }
}
 */

p1_V p10_gv = {.speed = 0, .direction = 0};
p1_V p10_gvs = {.speed = 0, .direction = 0};
p1_V p10_ov = {.speed = 0, .direction = 0};

void p10_goal() {
    p5_compute_metrics();
    p10_gv = p5_move_towards_goal();
    p10_gvs = p5_move_towards_goal_smooth(p10_gv);
}

void p10_obstacle() {
    p1_V v = {.speed = 1000, .direction = 0};
    p10_ov = p1_obstacle(v);
}

void p10_goal_obstacle() {
    if (p10_ov.direction == 0) {
        e_set_speed(p10_gvs.speed, p10_gvs.direction);
    } else {
        e_set_speed(p10_ov.speed, p10_ov.direction);
    }
}

void p10_run() {
    e_set_steps_left(0);
    e_set_steps_right(0);

    p5_set_goal(50, 50);

    e_activate_agenda(p1_sense, 1000);
    //e_activate_agenda(p1_drive, 1000);
    e_activate_agenda(p10_goal, 1000);
    e_activate_agenda(p10_obstacle, 1000);
    e_activate_agenda(p10_goal_obstacle, 1000);

    while (1) {
    }
}
