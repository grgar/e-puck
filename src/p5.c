#include <a_d/advance_ad_scan/e_prox.h>
#include <a_d/advance_ad_scan/e_acc.h>
#include <motor_led/advance_one_timer/fast_agenda/e_led.h>
#include <motor_led/advance_one_timer/fast_agenda/e_motors.h>
#include <motor_led/advance_one_timer/fast_agenda/e_agenda_fast.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "common.h"

#include "p1.h"

#define WHEEL_GAP 5.3 //cm
#define WHEEL_DIAMETER 4.0 //cm
#define PI 3.141592

//goal metrics
double goalxaxis = 0; //x and y axis of goal
double goalyaxis = 0;
double g_dist; //straight line distance from the e-puck to goal
float g_angle; //angle of e-puck orientation compared goal

//e-puck metrics
float xaxis = 0.0; //x and y axis of e-puck
float yaxis = 0.0;
int stepsL; //Motor steps from left and right motor
int stepsR;
float x_angle = 0.0; //angle of e-puck orientation compared to the x axis

//Planning functions
//Calculate all current position metrics
void p5_compute_metrics() {
    //Get motor steps
    int stepsLlatest = e_get_steps_left();
    int stepsRlatest = -(e_get_steps_right());
    //Get motor distance
    float distL = ((stepsLlatest - stepsL) / 1000.0) * (WHEEL_DIAMETER * PI);
    float distR = ((stepsRlatest - stepsR) / 1000.0) * (WHEEL_DIAMETER * PI);
    float total_dist = (distL + distR) / 2;
    float angle_adj = (distR - distL) / WHEEL_GAP;
    
    //Re-calibrate steps
    stepsL = stepsLlatest;
    stepsR = stepsRlatest;

    //Adjust the angle of the e-puck with respect to the x axis
    x_angle = x_angle + angle_adj;

    //Make sure the e-puck doesn't turn more than it needs to
    if (x_angle > PI) {
        x_angle = x_angle - (2 * PI);
    } else if (x_angle < -PI) {
        x_angle = x_angle + (2 * PI);
    }

    //Get the coordinates of the e-puck
    xaxis += (total_dist * cos(x_angle));
    yaxis += (total_dist * sin(x_angle));
    
    //The x and y difference of the e-puck to the goal
    float xdiff = goalxaxis - xaxis;
    float ydiff = goalyaxis - yaxis;
   
    //Get the angle of the goal with respect to the e-puck
    if(xdiff == 0){
        g_angle = PI/2;
    }else{ //Must not divide by zero as arctan(infinite) should be 90 degrees
        g_angle = atanf(ydiff/ xdiff);
    }

    //Incase goal is in bottom left quadrant
    if(xdiff < 0 && ydiff < 0){
        g_angle -= PI;
    }else if(xdiff < 0){ //Incase goal is in top left quadrant
        g_angle += PI;
    }
    
    //Calculate the straight line distance between e-puck and the goal
    g_dist = sqrt((xdiff * xdiff) + (ydiff * ydiff));
}

//Set goal coordinates
void p5_set_goal(int x, int y) {
    goalxaxis = x;
    goalyaxis = y;
}

//Move towards goal coordinates x and y
p1_V p5_move_towards_goal() {
    //Convert radians to degrees
    p1_V v = {.speed = 500, .direction = ((g_angle - x_angle) * 180) / PI};

    //Stop if the goal is reached
    if (g_dist < 5) {
        int i;
        for (i = 0; i < 8; i++) {
            e_set_led(i, 1);
        }
        v.speed = 0;
        v.direction = 0;
    }
    
    return v;
}

p1_V p5_move_towards_goal_smooth(p1_V v) {
    if (v.speed == 0 && v.direction == 0) {
        return v;
    }

    int angle = v.direction;

    //Turn left if goal is to the left
    if (angle > 5) {
        v.direction = 25;
        if (angle > 25) { //Harsher turn if angle is very different
            v.direction = 350;
            v.speed = 0;
        }
    }

    //Turn right if goal is to the right
    if (angle < -5) {
        v.direction = -25;
        if (angle < -25) { //Harsher turn if angle is very different
            v.direction = -350;
            v.speed = 0;
        }
    }

    return v;
}

void p5_move_towards_goal_run() {
    p5_compute_metrics();
    p1_V v = p5_move_towards_goal();
    v = p5_move_towards_goal_smooth(v);
    e_set_speed(v.speed, v.direction);
}

void p5_run() {
    e_set_steps_left(0);
    e_set_steps_right(0);

    p5_set_goal(25, 25);
    e_activate_agenda(p5_move_towards_goal_run, 500);

    while (1) {
    }
}