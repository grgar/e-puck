#include "motor_led/e_init_port.h"
#include "motor_led/e_epuck_ports.h"
#include "motor_led/advance_one_timer/e_motors.h"
#include "motor_led/advance_one_timer/e_agenda.h"
#include "uart/e_uart_char.h"
#include "motor_led/advance_one_timer/e_led.h"

#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include <motor_led/advance_one_timer/fast_agenda/e_led.h>
#include <motor_led/advance_one_timer/fast_agenda/e_motors.h>


typedef int bool;
#define true 1
#define false 0

//void p6_run(void){
//	//basic set up for camera
//	e_poxxxx_init_cam();
//	e_poxxxx_config_cam(0,(ARRAY_HEIGHT - 4)/2,640,4,8,4,RGB_565_MODE);
//	e_poxxxx_write_cam_registers(); 
//
//	e_start_agendas_processing();
//	int centreValue;
//
//	while(1){
//		getImage();
//		Image();
//		e_led_clear();
//
//		//Take a section of the center, this means if there is an error with one it won't effect it as a whole.
//		centreValue = numbuffer[38] + numbuffer[39] + numbuffer[40] + numbuffer[41] + numbuffer[42] + numbuffer[43]; // removes stray 
//		if(centreValue > 3){ //If red is in the middle then stay still			
//			e_destroy_agenda(turn);
//			e_set_speed_left (0);
//			e_set_speed_right(0);
//			e_set_led(0,1);
//		}else if(isRedVisable){//If red isn't in the center but is visable then picks a direction to turn to face it
//			e_activate_agenda(turn, 650);
//			e_set_led(1,1);
//		}else{// if red isn't visible and no true values it will turn left
//			e_destroy_agenda(turn);
//			e_set_speed_left (0);
//			e_set_speed_right(0);
//			e_set_led(2,1);
//		}
//	}
//}

void p6_cha_cha() {
    
}

void p6_criss_cross() {
    //e_set_led(0);
    e_set_body_led(1);
    e_set_speed(0, -500); // Spin Right
//    long i;
//    for(i=0; i<4000000; i++) {asm("nop");}
    
//    delay(1000); // Wait
    e_set_speed(0, 0); // Stop Spin
//    delay(5000);
    e_set_body_led(1);
//    e_set_speed(0, -1000); // Spin Right
    delay(5);
    
}

void p6_slide_to_the(bool direction) {
    
}

void p6_run() {
//    e_activate_agenda(p6_criss_cross, 2500);
    p6_criss_cross();

    while (1) {
    }
}

