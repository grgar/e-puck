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

/**
 * Perform final escape from obstacle when intended velocity based on goal path
 * is towards the corner of the obstacle
 * @param v_goal intended velocity to go to goal
 * @param v_obst intended velocity to avoid obstacle
 * @return new velocity to avoid obstacle based on velocity to goal
 */
p1_V p10_escape(p1_V v_goal, p1_V v_obst) {
    if (v_goal.direction > 0) {
        if (p1_ir.val[6] < 85) {
            v_obst.direction = -10 * (100 - p1_ir.val[6]);
        } else if (p1_ir.val[5] < 85 || p1_ir.val[4] < 85) {
            v_obst.speed = 450;
            v_obst.direction = -100;
        }
    } else if (v_goal.direction < 0) {
        if (p1_ir.val[1] < 85) {
            v_obst.direction = -10 * (100 - p1_ir.val[1]);
        } else if (p1_ir.val[2] < 85 || p1_ir.val[3] < 85) {
            v_obst.speed = 450;
            v_obst.direction = 100;
        }
    }

    return v_obst;
}

/**
 * Velocity to goal
 */
p1_V p10_gv = {.speed = 0, .direction = 0};
/**
 * Velocity to goal with smoothed direction
 */
p1_V p10_gvs = {.speed = 0, .direction = 0};
/**
 * Velocity to avoid obstacle
 */
p1_V p10_ov = {.speed = 0, .direction = 0};

/**
 * Calculate velocities based on goal seeking
 */
void p10_goal() {
    p5_compute_metrics();
    p10_gv = p5_move_towards_goal();
    p10_gvs = p5_move_towards_goal_smooth(p10_gv);
}

/**
 * Calculate velocity based on obstacle avoidance
 */
void p10_obstacle() {
    p1_V v = {.speed = 1000, .direction = 0};
    p10_ov = p1_obstacle(v);
}

/**
 * Combine velocities to avoid obstacle while obstacle present while navigating
 * towards goal
 */
void p10_goal_obstacle() {
    if (p10_ov.direction == 0) {
        e_set_speed(p10_gvs.speed, p10_gvs.direction);
    } else {
        e_set_speed(p10_ov.speed, p10_ov.direction);
    }
}

/**
 * Obstacle avoidance and goal seeking
 */
void p10_run() {
    e_set_steps_left(0);
    e_set_steps_right(0);

    p5_set_goal(50, 50);

    e_activate_agenda(p1_sense, 1000);
    e_activate_agenda(p10_goal, 1000);
    e_activate_agenda(p10_obstacle, 1000);
    e_activate_agenda(p10_goal_obstacle, 1000);

    while (1) {
    }
}
