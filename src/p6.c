#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <motor_led/e_init_port.h>
#include <motor_led/e_epuck_ports.h>
#include <motor_led/advance_one_timer/fast_agenda/e_led.h>
#include <motor_led/advance_one_timer/fast_agenda/e_motors.h>
#include <motor_led/advance_one_timer/fast_agenda/e_agenda_fast.h>
#include <camera/fast_2_timer/e_poxxxx.h>
#include <a_d/advance_ad_scan/e_prox.h>

#include "./common.h"
#include "./p1.h"
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

int startStepsLeft = 0;
int startStepsRight = 0;
int currentStepsLeft = 0;
int currentStepsRight = 0;
int distanceDeltaLeft = 0;
int distanceDeltaRight = 0;
bool isMovingForwards = true;

// Odd index stores left distance/speed, even stores right
int travelDistanceArray[MAX * 2] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
int travelSpeedArray[MAX * 2] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
float travelDelayArray[MAX] = { -1, -1, -1, -1, -1, -1 };
int currentTravelDistanceIndex = 0;

bool isFinishedTraveling = false;
bool isTraveling = false;

void p6_queue_travel(int distanceLeft, int distanceRight, int speedLeft, int speedRight, float delayAfter) {
    int i;
    
    // Find next non -1 entry and set it
    for (i = 0; i < MAX * 2; i += 2) {
        if (travelDistanceArray[i] == -1) {
            travelDistanceArray[i] = distanceLeft;
            travelDistanceArray[i + 1] = distanceRight;
            travelSpeedArray[i] = speedLeft;
            travelSpeedArray[i + 1] = speedRight;
            travelDelayArray[i] = delayAfter;
            break;
        }
    }
}

bool isTravelAgendaLocked = false;

void p6_travel_agenda() {
    if (isTravelAgendaLocked) {
        return;
    }
    
    isTravelAgendaLocked = true;
 
    if (isTraveling) {
        bool leftReachedDistance = abs(currentStepsLeft - startStepsLeft) >= distanceDeltaLeft;
        bool rightReachedDistance = abs(currentStepsRight - startStepsRight) >= distanceDeltaRight;

        if (leftReachedDistance && rightReachedDistance) {
            isFinishedTraveling = true;
        } else {
            // TODO: Potential to stop either motor when it reaches distance here?
            
            currentStepsLeft = abs(e_get_steps_left());
            currentStepsRight = abs(e_get_steps_right());
        }
    }
    
    isTravelAgendaLocked = false;
}

void p6_travel_manager_agenda() {
    if (isTravelAgendaLocked) {
        return;
    }
    
    isTravelAgendaLocked = true;
    
    // New item is at the front of the array
    if (travelDistanceArray[currentTravelDistanceIndex] != -1) {
        int speedLeft = travelSpeedArray[currentTravelDistanceIndex];
        int speedRight = travelSpeedArray[currentTravelDistanceIndex + 1];
        int distanceLeft = travelDistanceArray[currentTravelDistanceIndex];
        int distanceRight = travelDistanceArray[currentTravelDistanceIndex + 1];
        
        if (!isTraveling) {
            // TODO: Combine left/right into same?
            startStepsLeft = abs(e_get_steps_left());
            startStepsRight = abs(e_get_steps_right());
            currentStepsLeft = startStepsLeft;
            currentStepsRight = startStepsRight;
            distanceDeltaLeft = abs(distanceLeft);
            distanceDeltaRight = abs(distanceRight);
            
            e_set_speed_left(speedLeft);
            e_set_speed_right(speedRight);

            isTraveling = true;
        } else if (isFinishedTraveling) {
            currentTravelDistanceIndex += 2;
            e_set_speed(0, 0);
            
            float delaySeconds = travelDelayArray[currentTravelDistanceIndex];
            
            delay((long) (1000000 * delaySeconds));
            
            isTraveling = false;
            isFinishedTraveling = false;
        }
    }

    isTravelAgendaLocked = false;
}

float p6_cm_to_steps(int cm) {
    float PI = 3.141592;
    int WHEEL_DIAMETER_CM = 4;
    float steps = (cm * 1000) / PI / WHEEL_DIAMETER_CM;

    return steps;
}

float p6_get_puck_length_in_steps() {
    int E_PUCK_DIAMETER_CM = 7;
    int puckLengthInSteps = p6_cm_to_steps(E_PUCK_DIAMETER_CM);

    return puckLengthInSteps;
}


void p6_parallel_park(int gapSize) {
    // TODO: Implement
    int FORWARD_SPEED = 1000;
    int REVERSE_SPEED = -FORWARD_SPEED;
    int puckLengthInSteps = p6_get_puck_length_in_steps();
    int halfPuckLengthInSteps = puckLengthInSteps / 2;

    p6_queue_travel(halfPuckLengthInSteps, halfPuckLengthInSteps, FORWARD_SPEED, FORWARD_SPEED, 1);

    if (gapSize <= puckLengthInSteps * 3) {
        // Smaller gap, make e-puck go straight in with no "frills"
        p6_queue_travel(puckLengthInSteps, puckLengthInSteps, REVERSE_SPEED, REVERSE_SPEED, 0);
        p6_queue_travel(300, 300, FORWARD_SPEED, REVERSE_SPEED, 0);
        p6_queue_travel(puckLengthInSteps, puckLengthInSteps, REVERSE_SPEED, REVERSE_SPEED, 0);
        p6_queue_travel(300, 300, REVERSE_SPEED, FORWARD_SPEED, 0);
    } else if (gapSize <= puckLengthInSteps * 4) {
        // Slightly bigger gap, perform smooth entry
        p6_queue_travel(halfPuckLengthInSteps, halfPuckLengthInSteps, REVERSE_SPEED, REVERSE_SPEED, 0);
        p6_queue_travel(800 * 0.20, 800, REVERSE_SPEED * 0.20, REVERSE_SPEED, 0);
        p6_queue_travel(800, 800 * 0.20, REVERSE_SPEED, REVERSE_SPEED * 0.20, 0);
    } else {
        // Take the piss

        // Turn 45 degrees left
        p6_queue_travel(0, 300, 0, REVERSE_SPEED, 1.5);

        // Travel backwards
        p6_queue_travel(850, 850, REVERSE_SPEED, REVERSE_SPEED, 0.5);

        // Turn 45 degrees left
        p6_queue_travel(300, 0, REVERSE_SPEED, 0, 1.5);

        // Travel closer to the wall again
        p6_queue_travel(500, 500, FORWARD_SPEED, FORWARD_SPEED, 0);
    }
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
    int DISTANCE_THRESHOLD = 95; // How far away we should consider "a gap"
    int IRReading = p6_get_confident_ir_reading(IRIndex, CONFIDENCE_REQUIRED);

    bool isBlocked = IRReading <= DISTANCE_THRESHOLD; // Blocked when IR closer to 0

    return !isBlocked;
}

void p6_switch_led(float ledsOn) {
    int i;

    for (i = 0; i < 8; i++) {
        e_set_led(i, i <= ledsOn ? 1 : 0);
    }
}

int p6_get_gap_size(int gapStart) {
    int currentGapSize = p6_get_steps();
    int gapSize = currentGapSize - gapStart;

    return gapSize;
}

int p6_is_big_gap() {
    // Potential problem here, attempting to get steps while in a while loop
    // Is there a function we can call to set the steps moved?
    int gapStart = p6_get_steps();
    int puckLengthInSteps = p6_get_puck_length_in_steps();
    int MINIMUM_GAP_SIZE = puckLengthInSteps * 2;
    int MAXIMUM_GAP_SIZE = puckLengthInSteps * 5;

    while (1) {
        bool isEndOfGap = !p6_is_gap();
        
        if (isEndOfGap) {
            return -1;
        }
        
        int gapSize = p6_get_gap_size(gapStart);

        p6_switch_led(((float) gapSize / (float) MINIMUM_GAP_SIZE) * 8);
        
        if (gapSize >= MINIMUM_GAP_SIZE) {
            
            // Keep going to the MAXIMUM_GAP_SIZE, since we can do a more fancy
            // parking movement based on how much room we've got
            while (p6_is_gap() && gapSize < MAXIMUM_GAP_SIZE) {
                gapSize = p6_get_gap_size(gapStart);
            }
            
            return gapSize;
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

// NOTE: MUST start e-puck away from any boxes while it calibrates the IR sensors
void p6_run(void) {
    // TODO: Find wall, use straighten up function, then continue

    e_activate_agenda(p6_travel_agenda, 10);
    e_activate_agenda(p6_travel_manager_agenda, 10);

    // Get the initial distance to the wall, use p6_straighten_up to keep it
    // int lastKnownDistanceToWall = p6_get_confident_ir_reading(5, 3);

    p6_drive();

    p6_switch_led(0);

    while (1) {
        bool isGap = p6_is_gap();

        // If it's the start of an opening
        if (isGap) {
            int bigGapSize = p6_is_big_gap();

            // Stop if it's a big enough gap to fit the ePuck
            if (bigGapSize > -1) {
                p6_switch_led(8);
                p6_parallel_park(bigGapSize);
                break;
            }
        } else {
            // TODO: Straighten up here, currently assume user places robot straight;
            // lastKnownDistanceToWall = p6_straighten_up(lastKnownDistanceToWall);
        }

        int steps = p6_get_steps();

        // If this is taking too long, stop anyway
        if (steps > 8000) {
            p6_stop();
            break;
        }
    }
}

