#include "motor_led/e_init_port.h"
#include "motor_led/e_epuck_ports.h"
#include "motor_led/advance_one_timer/e_motors.h"
#include "motor_led/advance_one_timer/e_agenda.h"
#include "uart/e_uart_char.h"
#include "motor_led/advance_one_timer/e_led.h"
#include "camera/fast_2_timer/e_poxxxx.h"

#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "./p6.h"

typedef int bool;
#define true 1
#define false 0

char buffer[2*40*40];

void load_image() {
	e_poxxxx_launch_capture(buffer);

    while(!e_poxxxx_is_img_ready()) {};
}

// Load the image, then return an array of black and white pixels
int * get_black_and_white_image() {
    load_image();
    
	long i;
    int pixelCount = 2*40*40; // 2 bytes per pixel, 40x40 pixels
    int blackAndWhiteImage[pixelCount];

	for (i = 0; i < pixelCount; i++) {
        int green, red, blue, total;
    
    	red = (buffer[2 * i] & 0xF8);
		green = (((buffer[2 * i] & 0x07) << 5) | ((buffer[2 * i + 1] & 0xE0) >> 3));
		blue = ((buffer[2 * i + 1] & 0x1F) << 3);
		
        total = red + green + blue;
        blackAndWhiteImage[i] = total / 3; // a value between 0 and 1
	}
    
    return blackAndWhiteImage;
}

int is_mostly_black(int * blackAndWhiteImage) {
    long i;
    long pixelCount = sizeof(blackAndWhiteImage);
    long totalBrightness = 0;
    
    for (i = 0; i < pixelCount; i++) {
        totalBrightness += blackAndWhiteImage[i];
    }
    
    if (totalBrightness / pixelCount > 0.5) {
        return 1;
    }
    
    return 0;
}

void p6_run(void) {
	while (1) {
        int * blackAndWhiteImage = get_black_and_white_image();
        int ledValue = is_mostly_black(blackAndWhiteImage);
        
        e_set_body_led(ledValue);
    }
}

