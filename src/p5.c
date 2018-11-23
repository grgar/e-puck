#include <stdlib.h>
#include <math.h>
#include <a_d/advance_ad_scan/e_prox.h>
#include <a_d/advance_ad_scan/e_acc.h>
#include <motor_led/advance_one_timer/fast_agenda/e_led.h>
#include <motor_led/advance_one_timer/fast_agenda/e_motors.h>
#include <motor_led/advance_one_timer/fast_agenda/e_agenda_fast.h>
#include "camera/fast_2_timer/e_poxxxx.h"
#include "common.h"

char camera[160]; //Camera information
int target_pixel[160]; //Is pixel target?
int target_visible = 0; //Is target visible?
double goalxaxis; //x and y axis of goal
double goalyaxis;
double xaxis = 0; //x and y axis of e-puck
double yaxis = 0;
double g_euc_dist; //straight line distance from the e-puck to goal
float x_angle; //angle of e-puck orientation compared to the x axis
float g_angle; //angle of e-puck orientation compared goal

//move forward
void p5_forward(void){
    e_set_speed(500, 0);
}

//turn left
void p5_left(void){
    e_set_speed(500, 100);
}

//turn right
void p5_right(void){
    e_set_speed(500, -100);
}

//capture the image
void p5_get_image(){
	e_poxxxx_launch_capture((char *)camera);
    while(!e_poxxxx_is_img_ready()){};
}

//Test each pixel for target
void p5_analyse_image(){
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

    //p5_get_image();
	//p5_analyse_image();

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
            p5_left;
        }
        else if(right > left){
            p5_right;
        }
        else if(right == left){
            p5_forward;
        }
    } else {
        e_activate_agenda(target_not_visible, 500);
    }
}

void euc_distance_from_goal() {
    g_euc_dist = sqrt(( goalxaxis - xaxis)*(goalxaxis - xaxis) + (goalyaxis - yaxis)*(goalyaxis - yaxis));
}

void goal_angle() {
    
}

void count_xaxis() {
    
}

void count_yaxis() {
    
}

void p5_run() {
    //Camera Set Up
    //e_poxxxx_init_cam();
	//e_poxxxx_config_cam(0,(ARRAY_HEIGHT - 4)/2,640,4,8,4,RGB_565_MODE);
	//e_poxxxx_write_cam_registers();
        
    e_activate_agenda(e_display_angle, 500);
    while(1) {
    }
}