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

#define MAX 6

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

int startSteps = 0;
int currentSteps = 0;
int distanceRequired = 0;
bool isMovingForwards = true;

int travelDistanceArray[6] = { -1, -1, -1, -1, -1, -1 };
int currentTravelDistanceIndex = 0;

bool isFinishedTraveling = false;
bool isTraveling = false;

void insert(int data) {
    int i;
    
    // Find next non -1 entry and set it
    for (i = 0; i < MAX; i++) {
        if (travelDistanceArray[i] == -1) {
            travelDistanceArray[i] = data;
            break;
        }
    }
}

void p6_travel_agenda() {
    if (isTraveling) {
        if (abs(currentSteps - startSteps) <= distanceRequired) {
            currentSteps = abs(e_get_steps_left()) + abs(e_get_steps_right());
        } else {
            isFinishedTraveling = true;
        }
    }
}

void p6_travel_manager_agenda() {
    // New item is at the front of the array
    if (travelDistanceArray[currentTravelDistanceIndex] != -1) {
        int distance = travelDistanceArray[currentTravelDistanceIndex];
        
        if (!isTraveling) {
            startSteps = abs(e_get_steps_left()) + abs(e_get_steps_right());
            currentSteps = startSteps;
            distanceRequired = abs(distance);
            
            e_set_speed(distance, 0); // TODO: Update dynamically
            isTraveling = true;
        } else if (isFinishedTraveling) {
            currentTravelDistanceIndex++;
            e_set_speed(0, 0);
            isTraveling = false;
            isFinishedTraveling = false;
        }
    }
}

void p6_travel_steps(int distance) {
    insert(distance);
}
void p6_parallel_park() {
    // TODO: Implement
    p6_travel_steps(-500);
    p6_travel_steps(1000);
    p6_travel_steps(-1000);
    p6_travel_steps(500);
    p6_travel_steps(500);
    p6_travel_steps(-100);
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
    
    e_activate_agenda(p6_travel_agenda, 500);
    e_activate_agenda(p6_travel_manager_agenda, 500);

    p6_parallel_park();
    
    while(1) {}
    
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

