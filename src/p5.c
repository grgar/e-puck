#include <stdlib.h>
#include <math.h>
#include <a_d/advance_ad_scan/e_prox.h>
#include <a_d/advance_ad_scan/e_acc.h>
#include <motor_led/advance_one_timer/fast_agenda/e_led.h>
#include <motor_led/advance_one_timer/fast_agenda/e_motors.h>
#include <motor_led/advance_one_timer/fast_agenda/e_agenda_fast.h>
#include "camera/fast_2_timer/e_poxxxx.h"
#include "common.h"

#define AXLE_SIZE 5.0
#define WHEEL_DIAMETER 4.0
#define PI 3.141592
#define TURN_STRENGTH 100

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
double xaxis = 0.0; //x and y axis of e-puck
double yaxis = 0.0;
int stepsL;//Motor steps from left and right motor
int stepsR;
float x_angle = 0.0; //angle of e-puck orientation compared to the x axis

//Camera functions
//capture the image
void get_image(){
	e_poxxxx_launch_capture((char *)camera);
    while(!e_poxxxx_is_img_ready()){};
}

//Test each pixel for target
void analyse_image(){
    int red;
    int green;
    int blue;
    target_visible = 0;
    int i;
	for(i = 0; i < 160; i++){
        //convert colour into integer
		red = (camera[i] & 0xF8);
		green = (((camera[i] & 0x07) << 5) | ((camera[i+1] & 0xE0) >> 3));
		blue = ((camera[i+1] & 0x1F) << 3);
        
		if(red - 25 > ((green + blue)/2)){
			target_pixel[i] = 1;
            target_visible = 1;
		}else{
			target_pixel[i] = 0;
		}
	}
}

void target_not_visible() {
    //perform p1 or some wondering function
}

//turn based on if the target is to the left or right
void p5_follow_target(){

    get_image();
	analyse_image();

    if(target_visible == 1){ //If the target is not visible do some plan oriented behaviour 
        int left = 0;
        int right = 0;
        int i;
        for(i = 0; i < 80; i++){
            if(target_pixel[i] == 1){
                left++;
            }
            if(target_pixel[i + 80] == 1){
                right++;
            }
        }
        if(left > right){
            e_set_speed(500, 100); //Turn left
        }
        else if(right > left){
            e_set_speed(500, -100); //Turn right
        }
        else if(right == left){
            e_set_speed(500, 0); //Move forward
        }
    } else {
        e_activate_agenda(target_not_visible, 500); //this is wrong
    }
}

//Planning functions
//Calculate all current position metrics
void compute_metrics(){
    //Get motor steps
    int stepsLlatest = e_get_steps_left();
    int stepsRlatest = e_get_steps_right();
    //Get motor distance
    double distL = (stepsLlatest / 1000.0) * (WHEEL_DIAMETER * PI);
    double distR = (stepsRlatest / 1000.0) * (WHEEL_DIAMETER * PI);
    double total_dist = (distL + distR) / 2;
    float angle_adj = (distR - distL)*2 / AXLE_SIZE;
    
    //Adjust the angle of the e-puck with respect to the x axis
    x_angle = x_angle + angle_adj;
    
    //Get the coordinates of the e-puck
    xaxis = xaxis + (total_dist * cos(x_angle));
    yaxis = yaxis + (total_dist * sin(x_angle));
    
    //Get the angle of the goal with respect to the e-puck
    g_angle = atanf((goalyaxis - yaxis)/(goalxaxis - xaxis));
    
    //Calculate the straight line distance between e-puck and the goal
    g_dist = sqrt((goalxaxis - xaxis)*(goalxaxis - xaxis) + (goalyaxis - yaxis)*(goalxaxis - xaxis));
    
    e_set_steps_left(0);
    e_set_steps_right(0);
}

//Set goal coordinates
void p5_set_goal(int x, int y) {
    goalxaxis = x;
    goalyaxis = y;
}

//Move towards goal coordinates x and y
void p5_move_towards_goal() {
    compute_metrics(); //update position data
    
    float turning = ((g_angle - x_angle)/ 2*PI) * TURN_STRENGTH;
    e_set_speed(0, turning);
}

void p5_run() {
    //Camera Set Up
    //e_poxxxx_init_cam();
	//e_poxxxx_config_cam(0,(ARRAY_HEIGHT - 4)/2,640,4,8,4,RGB_565_MODE);
	//e_poxxxx_write_cam_registers();
        
    p5_set_goal(20, 20);
    e_activate_agenda(p5_move_towards_goal, 500);
    while(1) {
    }
}