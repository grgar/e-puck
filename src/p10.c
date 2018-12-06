#include <motor_led/e_init_port.h>
#include <motor_led/advance_one_timer/fast_agenda/e_led.h>
#include <motor_led/advance_one_timer/fast_agenda/e_motors.h>
#include <motor_led/advance_one_timer/fast_agenda/e_agenda_fast.h>
#include "common.h"
#include "p1.h"
#include "p5.h"

void p10_goal_obstacle() {
    p1_v = p1_obstacle(p5_move_towards_goal());
}

void p10_run() {
    e_set_steps_left(0);
    e_set_steps_right(0);

    p5_set_goal(50, 50);

    e_activate_agenda(p1_sense, 500);
    e_activate_agenda(p1_drive, 500);
    e_activate_agenda(p10_goal_obstacle, 500);
    while (1) {
    }
}
