#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <a_d/advance_ad_scan/e_prox.h>
#include <a_d/advance_ad_scan/e_acc.h>
#include <motor_led/advance_one_timer/fast_agenda/e_led.h>
#include <motor_led/advance_one_timer/fast_agenda/e_motors.h>
#include <motor_led/advance_one_timer/fast_agenda/e_agenda_fast.h>
#include "camera/fast_2_timer/e_poxxxx.h"
#include "common.h"

#include "p1.h"

#define WHEEL_GAP 5.3 //cm
#define WHEEL_DIAMETER 4.0 //cm
#define PI 3.141592

//Camera values
char camera[160]; //Camera information
int target_pixel[160]; //Is pixel target?
int target_visible = 0; //Is target visible?

//goal metrics
double goalxaxis; //x and y axis of goal
double goalyaxis;
double g_dist; //straight line distance from the e-puck to goal
float g_angle; //angle of e-puck orientation compared goal

//e-puck metrics
float xaxis = 0.0; //x and y axis of e-puck
float yaxis = 0.0;
int stepsL;//Motor steps from left and right motor
int stepsR;
float x_angle = 0.0; //angle of e-puck orientation compared to the x axis

int p5_speed = 500;

//Planning functions
//Calculate all current position metrics
void compute_metrics(){
    //Get motor steps
    int stepsLlatest = e_get_steps_left();
    int stepsRlatest = -(e_get_steps_right());
    //Get motor distance
    float distL = ((stepsLlatest - stepsL) / 1000.0) * (WHEEL_DIAMETER * PI);
    float distR = ((stepsRlatest - stepsR) / 1000.0) * (WHEEL_DIAMETER * PI);
    float total_dist = (distL + distR) / 2;
    float angle_adj = (distR - distL) / WHEEL_GAP;
    
    //Adjust the angle of the e-puck with respect to the x axis
    x_angle = x_angle + angle_adj;
    
    //Make sure the e-puck doesn't turn more than it needs to
    if(x_angle > PI) {
        x_angle = x_angle - (2 * PI);
    } else if(x_angle < -PI) {
        x_angle = x_angle + (2 * PI);
    }
    
    //Get the coordinates of the e-puck
    xaxis = xaxis + (total_dist * cos(x_angle));
    yaxis = yaxis + (total_dist * sin(x_angle));
    
    //Get the angle of the goal with respect to the e-puck
    g_angle = atanf((goalyaxis - yaxis)/ (goalxaxis - xaxis));
    
    //Calculate the straight line distance between e-puck and the goal
    g_dist = sqrt(((goalxaxis - xaxis)*(goalxaxis - xaxis)) + ((goalyaxis - yaxis)*(goalxaxis - xaxis)));
    
    //Re-calibrate steps
    stepsL = stepsLlatest;
    stepsR = stepsRlatest;
}

//Set goal coordinates
void p5_set_goal(int x, int y) {
    goalxaxis = x;
    goalyaxis = y;
}

//Move towards goal coordinates x and y
p1_V p5_move_towards_goal() {  
    compute_metrics();
    int angle = ((g_angle - x_angle) * 180) / PI;
    int turn = 0;
        
    if(angle > 5){
        turn = 20;
        if(angle > 30){
            turn = 50;
            if(angle > 60){
                turn = 75;
                if(angle > 90){
                    turn = 100;
                }
            }
        }
    }
    
    if(angle < -5){
        turn = -20;
        if(angle < -30){
            turn = -50;
            if(angle < -60){
                turn = -75;
                if(angle < -90){
                    turn = -100;
                }
            }
        }
    }
    
    if(g_dist < 5){
        int i;
        for(i=0;i<8;i++){
            e_set_led(i,1);
        }
        p5_speed = 0;
        turn = 0;
    }
    
    p1_V v = {.speed = 500, .direction = turn};
    return v;
}

void p5_move_towards_goal_run() {
    p1_V v = p5_move_towards_goal();
    e_set_speed(v.speed, v.direction);
}

void p5_run() {
    e_set_steps_left(0);
    e_set_steps_right(0);
       
    p5_set_goal(50, 50);
    e_activate_agenda(p5_move_towards_goal_run, 250);
    
    while(1) {
    }
}