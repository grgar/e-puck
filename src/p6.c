#include <motor_led/e_init_port.h>
#include <motor_led/e_epuck_ports.h>
#include <motor_led/advance_one_timer/fast_agenda/e_led.h>
#include <motor_led/advance_one_timer/fast_agenda/e_motors.h>
#include <motor_led/advance_one_timer/fast_agenda/e_agenda_fast.h>
#include <uart/e_uart_char.h>
#include <camera/fast_2_timer/e_poxxxx.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "./p6.h"
#include <e_uart_char.h>

typedef int bool;
#define true 1
#define false 0

// TODO: Move to common (this is George's code)
typedef struct {
    /**
     * IR percentages, 100% clear, 0% blocked
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

    memcpy(p1_ir.val, val, sizeof val);
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
    int DISTANCE_THRESHOLD = 0.5; // How far away we should consider "a gap"
    
    return p6_IR[5] >= DISTANCE_THRESHOLD;
}

bool p6_is_big_gap() {
    int gapStart = p6_get_steps();
    int MINIMUM_GAP_SIZE = 1000;
    
    while (1) {
        bool isEndOfGap = !p6_is_gap();
        
        if (isEndOfGap) {
            return false;
        }
        
        int currentGapSize = p6_get_steps();
        int gapSize = currentGapSize - gapStart;
        
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

