#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <motor_led/e_init_port.h>
#include <motor_led/e_epuck_ports.h>
#include <motor_led/advance_one_timer/fast_agenda/e_led.h>
#include <motor_led/advance_one_timer/fast_agenda/e_motors.h>
#include <motor_led/advance_one_timer/fast_agenda/e_agenda_fast.h>
#include <uart/e_uart_char.h>
#include <camera/fast_2_timer/e_poxxxx.h>
#include <a_d/advance_ad_scan/e_prox.h>
#include <e_uart_char.h>

#include "./common.h"
#include "./p6.h"

typedef int bool;
#define true 1
#define false 0

/**
 * IR percentages as integers between 0 (blocked) and 100 (clear)
 */
int p6_ir_val[8] = {0};

typedef struct {
    int speed;
    int turn;
} CurrentSpeed;

CurrentSpeed p6_current_speed = {.speed = 0, .turn = 0};

void p6_set_speed(int speed, int turn) {
    e_set_speed(speed, turn);
    p6_current_speed.speed = speed;
    p6_current_speed.turn = turn;
}

void p6_add_speed(int speedToAdd, int turnToAdd) {
    p6_set_speed(p6_current_speed.speed + speedToAdd, p6_current_speed.turn + turnToAdd);
}

// TODO: Move to common (this is George's code)
void p6_sense() {
    p1_sense_ir(p6_ir_val);
}

void p6_drive() {
    p6_set_speed(1000, 0);
}

void p6_travel_steps(int distanceLeft, int distanceRight, int speed) {
    //TODO
    // Check if steps right is negative
    // check the while condition - possibly infinite
    // Get screen outputting variable correctly
    
    int startStepsLeft = e_get_steps_left();
    int startStepsRight = e_get_steps_right() * -1; // Steps right is negative if it's gone forwards
    int currentStepsLeft = startStepsLeft;
    int currentStepsRight = startStepsRight;
    
    e_set_speed(speed, 0);
    
        char message3[20];
        sprintf(message3, "pere%i\n", 0);
        e_send_uart1_char(message3, strlen(message3));
    
    // Keep traveling until
    while (currentStepsLeft - startStepsLeft <= distanceLeft || currentStepsRight - startStepsRight <= distanceRight) {
        char message4[10];
        sprintf(message4, "test%i\n", 1);
        e_send_uart1_char(message4, strlen(message4));
        
        char message5[150];
        sprintf(message5, "CSL: %i, SSL: %i, DL: %i, CSR: %i, SSR: %i, DR: %i, \n", currentStepsLeft, startStepsLeft, distanceLeft, currentStepsRight, startStepsRight, distanceRight);
        e_send_uart1_char(message5, strlen(message5));
        
        currentStepsLeft = e_get_steps_left();
        currentStepsRight = e_get_steps_right() * -1; // Steps right is negative if it's gone forwards
    }
        
        char message2[10];
        sprintf(message2, "after%i\n", 3);
        e_send_uart1_char(message2, strlen(message2));
    
    e_set_speed(0, 0);
}

void p6_parallel_park_2() {
//    int FORWARD_SPEED = 1000;
//    int REVERSE_SPEED = -FORWARD_SPEED;

    
    p6_travel_steps(1000, 1000, 1000);
//    
//    
//    // TODO: Fine tune delay to travel half the distance of the ePuck
//    // 1000 steps = 12.8cm
//    delay(1000);
//    
//    e_set_speed(0, REVERSE_SPEED);
//    
//    // TODO: Fine tune delay so ePuck is angled at a 45 degree angle
//    delay(1000);
//    
//    e_set_speed(REVERSE_SPEED, REVERSE_SPEED);
//    
//    // TODO: Use IR sensor instead of delay. Stop reversing when ePuck is close to hitting wall
//    delay(1000);
//    
//    e_set_speed(REVERSE_SPEED, 0);
//    
//    // TODO: Fine tune delay so ePuck has corrected the 45 degree angle it just gave itself
//    delay(1000);
}

void p6_stop() {
    p6_set_speed(0, 0);
}

int p6_get_steps() {
    return abs(e_get_steps_left()) + abs(e_get_steps_right());
}

int p6_get_confident_ir_reading(int IRIndex, int confidenceRequired) {
    int totalIRValue = 0;
    int i = 0;

    for (i = 0; i < confidenceRequired; i++) {
        p6_sense();
        totalIRValue += p6_ir_val[IRIndex];
    }

    return totalIRValue / confidenceRequired;
}

bool p6_is_gap() {
    int IRIndex = 5;
    int CONFIDENCE_REQUIRED = 3;
    int DISTANCE_THRESHOLD = 50; // How far away we should consider "a gap"
    int IRReading = p6_get_confident_ir_reading(IRIndex, CONFIDENCE_REQUIRED);

    bool isBlocked = IRReading <= DISTANCE_THRESHOLD; // Blocked when IR closer to 0

    return !isBlocked;
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
        
        if (gapSize >= MINIMUM_GAP_SIZE) {
            return true;
        }
    }
}

void p6_parallel_park() {
    int FORWARD_SPEED = 1000;
    int REVERSE_SPEED = -FORWARD_SPEED;

    p6_set_speed(REVERSE_SPEED, REVERSE_SPEED);
    
    // TODO: Fine tune delay to travel half the distance of the ePuck
    // 1000 steps = 12.8cm
    delay(1000);
    
    p6_set_speed(0, REVERSE_SPEED);
    
    // TODO: Fine tune delay so ePuck is angled at a 45 degree angle
    delay(1000);
    
    p6_set_speed(REVERSE_SPEED, REVERSE_SPEED);
    
    // TODO: Use IR sensor instead of delay. Stop reversing when ePuck is close to hitting wall
    delay(1000);
    
    p6_set_speed(REVERSE_SPEED, 0);
    
    // TODO: Fine tune delay so ePuck has corrected the 45 degree angle it just gave itself
    delay(1000);
}

bool p6_is_straightened_up(int lastKnownDistanceToWall, int straightenedUpThreshold) {
    int distanceToWall = p6_get_confident_ir_reading(5, 3);
    bool isStraightenedUp = abs(lastKnownDistanceToWall - distanceToWall) <= straightenedUpThreshold;
    
    // Considered to be "straight" if the current distance and the distance
    // to the wall is no longer increasing/decreasing
    return isStraightenedUp;
}

void p6_straighten_up(int lastKnownDistanceToWall) {
    int STRAIGHTENED_UP_THRESHOLD = 10;
    
    bool isStraightenedUp = p6_is_straightened_up(lastKnownDistanceToWall, STRAIGHTENED_UP_THRESHOLD);

    if (isStraightenedUp) {
        return;
    }

    int SPEED_INCREASE_DECREASE_AMOUNT = 10;
    int previousDistanceToWall = p6_get_confident_ir_reading(5, 3);
    
    while (!isStraightenedUp) {
        int distanceToWall = p6_get_confident_ir_reading(5, 3);
        int isDistanceFromWallDecreasing = previousDistanceToWall > distanceToWall;

        if (isDistanceFromWallDecreasing) {
            // The ePuck has drifted away, turn left
            p6_add_speed(0, SPEED_INCREASE_DECREASE_AMOUNT);
        } else {
            // The ePuck is getting closer, turn right
            p6_add_speed(0, -SPEED_INCREASE_DECREASE_AMOUNT);
        }
        
        isStraightenedUp = p6_is_straightened_up(lastKnownDistanceToWall, STRAIGHTENED_UP_THRESHOLD);
        
        if (!isStraightenedUp) {
            previousDistanceToWall = p6_get_confident_ir_reading(5, 3);
        }
    }
}

void p6_run(void) {
    e_activate_agenda(p6_sense, 500);

    // TODO: Find wall, use straighten up function, then continue

    p6_drive();

//    e_activate_agenda(p6_sense, 500);
    
    // TODO: Find wall, use straighten up function, then continue
    
    p6_parallel_park_2();
    
    // Get the initial distance to the wall, use p6_straighten_up to keep it
    int lastKnownDistanceToWall = p6_get_confident_ir_reading(5, 3);

    while (1) {
        bool isGap = p6_is_gap();

        // If it's the start of an opening
        if (isGap) {
            bool isBigGap = p6_is_big_gap();

            // Stop if it's a big enough gap to fit the ePuck
            if (isBigGap) {
                // TODO: Parallel park here, currently just stop.
                // p6_parallel_park();
                p6_stop();
                break;
            }
        } else {
            // TODO: Straighten up here, currently assume user places robot straight;
            // lastKnownDistanceToWall = p6_straighten_up(lastKnownDistanceToWall);
        }

        int steps = p6_get_steps();

        // If this is taking too long, stop anyway
        if (steps > 5000) {
            p6_stop();
            break;
        }
    }
}

