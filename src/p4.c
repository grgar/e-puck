#include <common.h>
#include "p1.h"
#include <motor_led/advance_one_timer/fast_agenda/e_motors.h>
#include <motor_led/advance_one_timer/fast_agenda/e_agenda_fast.h>
#include <e_uart_char.h>
#include <camera/fast_2_timer/e_poxxxx.h>
#include <motor_led/advance_one_timer/fast_agenda/e_led.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char fbwbuffer[160];
int numbuffer[80];
long isRedVisible;

char hexbuffer[1*2];

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
//custom cam picture load
void getImage(){
	e_poxxxx_launch_capture((char *)fbwbuffer);
    while(!e_poxxxx_is_img_ready()){};
}
// Image processing removes useless information
void Image(){	
	long i;
	int green, red, vis;
	for(i=0; i<80; i++){
		//RGB turned into an integer value for comparison
		red = (fbwbuffer[2*i] & 0xF8);
		green = (((fbwbuffer[2*i] & 0x07) << 5) | ((fbwbuffer[2*i+1] & 0xE0) >> 3));
		//blue = ((fbwbuffer[2*i+1] & 0x1F) << 3); we don't need blue for looking for red.
		if(red > green + 5){ // green will be less then red when red is strong.
			numbuffer[i] = 1;
			vis++;
		}else{
			numbuffer[i] = 0;
		}
		//If red is visable then isRedVisable turns to true
		if(vis>0){
			isRedVisible = 1;
            p4_led_off();
		}else{
			isRedVisible = 0;
            p4_led_on();
		}
	}	
}

//Decide which way to turn.
int turnDirection(){
	int sumL = 0;
	int sumR = 0;
	int i;
	for(i=0;i<40;i++){
		sumL += numbuffer[i];
		sumR += numbuffer[i+40];
	}
	if(sumL<sumR){ 
		return 1;
	}else{
		return 0;
	}
}
//Function to deal with turning.
void turn(void) {
	if(turnDirection()){
		e_set_speed_left (500);
		e_set_speed_right(-500);
	}else{
		e_set_speed_left (-500);
		e_set_speed_right(500);
	}
}

void p4_run() {
    
    // camera set up to take a 1 pixel image in the centre of the camera
	e_poxxxx_init_cam();
	//This gets a 4x4 image at the center of the camera and samples it to a single pixel
	e_poxxxx_config_cam((ARRAY_WIDTH - 4)/2,(ARRAY_HEIGHT - 4)/2,4,4,4,4,RGB_565_MODE);
	e_poxxxx_set_mirror(1,1);
	e_poxxxx_write_cam_registers(); 
	//Capture image function
	getImage();
	//Print image to terminal
	p4_led_on();
    
    while(1){}
    
}
