#include "common.h"
#include "p1.h"
#include <motor_led/advance_one_timer/fast_agenda/e_motors.h>
#include <a_d/advance_ad_scan/e_prox.h>
#include <a_d/advance_ad_scan/e_acc.h>

#include <motor_led/advance_one_timer/fast_agenda/e_agenda_fast.h>
#include <e_uart_char.h>
#include <camera/fast_2_timer/e_poxxxx.h>
#include <motor_led/advance_one_timer/fast_agenda/e_led.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

void p4_led_on(){
    int i;
    for(i=0;i<8;i++){
        e_set_led(i,1);
    }
}

void p4_led_off(){
    int i;
    for(i=0;i<8;i++){
        e_set_led(i,0);
    }
}

typedef struct {
    /**
     * IR percentages, 100% clear, 0% blocked
     */
    int val[8];
    int front;
} p4_IR;

p4_IR p4_ir = {.val = {0}, .front = 0};


void p4_hug_ir(){
    int val[8] = {0};
    int i;
    for(i=0;i<8;i++){
        val[i] = max(100 - (min(e_get_calibrated_prox(i), 3000) / 30), 0);
    }
    for (i = 0; i < 8; i++) {
        if (val[i] < 80 && min(val[(i + 2) % 8] < 80, val[(i + 3) % 8] < 80)) {
            val[(i + 1) % 8] = 80;
            if (val[(i + 3) % 8] < val[(i + 2) % 8]) {
                val[(i + 2) % 8] = 80;
            }
        }
    }
    memcpy(p4_ir.val, val, sizeof val);
    p4_ir.front = min(p4_ir.val[0], p4_ir.val[7]);
}


typedef struct {
    int speed;
    int direction;
} p4_V;

p4_V p4_v = {.speed = 0, .direction = 0};


void p4_forward_one_step(){
    p4_v.speed = 600;
    p4_v.direction = 0;
}

void p4_back_one_step(){
    p4_v.speed = -600;
    p4_v.direction = 0;
}

void p4_stop(){
    p4_v.speed = 0;
    p4_v.direction = 0;
}

void p4_right_one_step(){
    p4_v.speed = 0;
    p4_v.direction = -600;
}
void p4_left_one_step(){
    p4_v.speed = 0;
    p4_v.direction = 600;
}


void moving(){
    int mode = 0;
    int steps = (abs(e_get_steps_right()) + abs(e_get_steps_left()));
    if(steps > 600){
        mode = 1;
    }
    if(steps > 635){
        mode = 2;
    }
    if(steps > 2550){
        mode = 3;
    }
    switch (mode){
        case 0:
            p4_left_one_step();
            break;
        case 1:
            p4_forward_one_step();
            break;
        case 2:
            p4_right_one_step();
            break;
        case 3: 
            p4_stop();
            break;
    }
    e_set_speed(p4_v.speed, p4_v.direction);
}

void p4_turning(){
    p4_v.speed = 0;
    p4_v.direction = 100;
    e_set_speed(p4_v.speed, p4_v.direction);
}

void p4_forward(){
    p4_v.speed = 300;
    p4_v.direction = 0;
    e_set_speed(p4_v.speed, p4_v.direction);
}

void p4_hug_wall_motor_set(){
    p4_hug_ir();
    int i;
    
    //if sides and front of e-puck are clear
    p4_forward();
    p4_led_off();
    
    
    for(i=1;i<7;i++){
        //if both the sides and front have an object in front of them
        if((p4_ir.val[i] < 100)){
            p4_turning();
            p4_led_off();
        } 
        //if the front is not clear but sides clear
        if((p4_ir.front < 100) & (p4_ir.val[i] < 100)){
            e_pause_agenda(p4_hug_wall_motor_set);
            e_activate_agenda(moving, 500);
            e_destroy_agenda(moving);
            e_restart_agenda(p4_hug_wall_motor_set);
            p4_led_on();
        }
    }
}

void p4_run() {
    
   
	//Capture image function
	
	//Print image to terminal
    
    e_activate_agenda(p4_hug_wall_motor_set, 500);
    
}
