#include <motor_led/e_init_port.h>
#include <motor_led/e_epuck_ports.h>
#include <motor_led/advance_one_timer/fast_agenda/e_led.h>
#include <motor_led/advance_one_timer/fast_agenda/e_motors.h>
#include <motor_led/advance_one_timer/fast_agenda/e_agenda_fast.h>
#include <uart/e_uart_char.h>
#include <camera/fast_2_timer/e_poxxxx.h>
#include <a_d/advance_ad_scan/e_prox.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "./p6.h"
#include <e_uart_char.h>

typedef int bool;
#define true 1
#define false 0

typedef struct {
    /**
     * IR percentages as integers between 0 (blocked) and 100 (clear)
     */
    int val[8];
} p6_IR;

p6_IR p6_ir = {.val =
    { 0}};

// TODO: Move to common (this is George's code)
void p6_sense() {
    int i;
    int val[8] = {0};

    for (i = 0; i < 8; i++) {
        val[i] = max(100 - (min(e_get_calibrated_prox(i), 3000) / 30), 0); // Normalise between 0 and 100
    }
    
    memcpy(p6_ir.val, val, sizeof val);
}

void p6_drive() {
    e_set_speed(1000, 0);
}

void p6_stop() {
    e_set_speed(0, 0);
}

int p6_get_steps() {
    return abs(e_get_steps_left()) + abs(e_get_steps_right());
}

bool p6_is_gap() {
    int DISTANCE_THRESHOLD = 80; // How far away we should consider "a gap"
    bool isGap = p6_ir.val[5] >= DISTANCE_THRESHOLD;
    
    char message5[15];
    sprintf(message5, "checking!\n\r");
    e_send_uart1_char(message5, strlen(message5));
//    
//    int val = p6_ir.val[5];
    char message4[30];
    sprintf(message4, "%d\n\r", e_get_calibrated_prox(5));
    e_send_uart1_char(message4, strlen(message4));
    
    return isGap;
}

bool p6_is_big_gap() {
    int gapStart = p6_get_steps();
    int MINIMUM_GAP_SIZE = 2000;
    
    while (1) {
        bool isEndOfGap = !p6_is_gap();
        
        if (isEndOfGap) {
            return false;
        }
        
        int currentGapSize = p6_get_steps();
        int gapSize = currentGapSize - gapStart;
        
        char message3[100];
        sprintf(message3, "Gap Start: %d, Current Gap Size: %d, FINAL GAP SIZE: %d\n\r", gapStart, currentGapSize, gapSize);
        e_send_uart1_char(message3, strlen(message3));
        
        if (gapSize >= MINIMUM_GAP_SIZE) {
            return true;
        }
    }
}

void p6_run(void) {
    e_init_uart1();
    e_activate_agenda(p6_sense, 500);
    p6_drive();
    
	while (1) {
        bool isGap = p6_is_gap();

        // If it's the start of an opening
        if (isGap) {
            bool isBigGap = p6_is_big_gap();

            // Stop if it's a big enough gap to fit the ePuck
            if (isBigGap) {
                p6_stop();
                break;
            }
        }

        int steps = p6_get_steps();

        // If this is taking too long, stop anyway
        if (steps > 5000) {
            p6_stop();
            break;
        }
    }
}

