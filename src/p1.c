#include <stdlib.h>
#include <a_d/advance_ad_scan/e_prox.h>
#include <motor_led/advance_one_timer/fast_agenda/e_led.h>
#include <motor_led/advance_one_timer/fast_agenda/e_motors.h>
#include <motor_led/advance_one_timer/fast_agenda/e_agenda_fast.h>
#include "common.h"

typedef struct {
    /**
     * IR percentages, 100% clear, 0% blocked
     */
    int val[8];
    int front;
} p1_IR;

p1_IR p1_ir = {.val =
    { 0}, .front = 0};

void p1_sense() {
    int i;
    for (i = 0; i < 8; i++) {
        p1_ir.val[i] = max(100 - (min(e_get_calibrated_prox(i), 3000) / 30), 0);
    }
    p1_ir.front = min(p1_ir.val[0], p1_ir.val[7]);
}

typedef struct {
    int speed;
    int direction;
} p1_V;

p1_V p1_v = {.speed = 0, .direction = 0};

void p1_drive() {
    e_set_speed(p1_v.speed, p1_v.direction);
}

p1_V p1_obstacle_uncertainty(p1_V v) {
    if (v.speed >= 800) {
        e_set_body_led(1);
        int i;
        for (i = 0; i < 8; i++) {
            if (p1_ir.val[i] < 100) {
                v.speed = min(v.speed, 800);
                return v;
            }
        }
    } else {
        e_set_body_led(0);
    }
    return v;
}

p1_V p1_obstacle_adjust(p1_V v) {
    // Only adjust for side obstacles if:
    // robot not stationary, and
    // robot seeing obstacle on the side
    if (v.speed <= 0 ||
            min(
            min(p1_ir.val[1], p1_ir.val[2]),
            min(p1_ir.val[5], p1_ir.val[6])
            ) > 80) {
        return v;
    }
    int ir_right = 100 - p1_ir.val[1] + (100 - p1_ir.val[2]) * 0.5;
    int ir_left = 100 - p1_ir.val[6] + (100 - p1_ir.val[5]) * 0.5;
    v.direction = ir_right - ir_left;
    v.speed = between(abs(v.direction) * -1 + 200, 350, 600);
    return v;
}

p1_V p1_obstacle_avoid(p1_V v) {
    if (p1_ir.front > 90) {
        e_set_led(4, 1);
        return v;
    }
    e_set_led(4, 0);

    v.speed = 0;
    v.direction =
            p1_ir.val[0] > p1_ir.val[7] ?
            // Turn left
            -1000 :
            // Turn right
            1000;

    return v;
}

p1_V p1_obstacle_surrounded(p1_V v) {
    int i;
    for (i = 0; i < 8; i++) {
        if (p1_ir.val[i] > 0) {
            return v;
        }
    }
    v.speed = 0;
    v.direction = 0;
    return v;
}

int p1_startup_complete = 0;
p1_V p1_startup_torque(p1_V v) {
    if (p1_startup_complete == 1) {
        return v;
    }
    int steps = abs(e_get_steps_left() + e_get_steps_right());
    if (steps < 500) {
        v.speed = max(v.speed, (steps + 10) * 5);
    } else {
        p1_startup_complete = 1;
    }
    return v;
}

void p1_obstacle() {
    p1_V v;
    v.speed = 1000;
    v.direction = 0;

    // Cap speed if detected uncertainty
    v = p1_obstacle_uncertainty(v);

    // Avoid front obstacles
    v = p1_obstacle_avoid(v);

    // Avoid side obstacles
    v = p1_obstacle_adjust(v);

    // Avoid panic
    //v = p1_obstacle_surrounded(v);
    
    // Reduce instant torque on startup
    v = p1_startup_torque(v);
    
    p1_v = v;
}

void p1_run() {
    e_activate_agenda(p1_sense, 500);
    e_activate_agenda(p1_drive, 500);
    e_activate_agenda(p1_obstacle, 500);
    while (1) {
    }
}
