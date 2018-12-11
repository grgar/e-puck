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

// Maximum amount of actions in the queuing system
#define MAX 11


int WHEEL_DIAMETER_CM = 4;
int E_PUCK_DIAMETER_CM = 7;
int LEFT_IR_INDEX = 5;
int RIGHT_IR_INDEX = 2;

/**
 * IR percentages as integers between 0 (blocked) and 100 (clear)
 */
int p6_ir_val[8] = {0};

void p6_sense() {
    p1_sense_ir(p6_ir_val);
}

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


int startStepsLeft = 0;
int startStepsRight = 0;
int currentStepsLeft = 0;
int currentStepsRight = 0;
int distanceDeltaLeft = 0;
int distanceDeltaRight = 0;
bool isMovingForwards = true;

// Odd index stores left distance/speed, even stores right
int travelDistanceArray[MAX * 2] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
int travelSpeedArray[MAX * 2] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
float travelDelayArray[MAX] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
int currentTravelDistanceIndex = 0;

bool isFinishedTraveling = false;
bool isTraveling = false;

bool isTravelAgendaLocked = false;

/**
 * Locks the semaphore
 */
void p6_lock_semaphore() {
    isTravelAgendaLocked = true;
}

/**
 * Unlocks the semaphore
 */
void p6_unlock_semaphore() {
    isTravelAgendaLocked = false;
}

/**
 * Semaphore used to avoid race conditions in agendas
 * 
 * Checks if semaphore is locked.
 * If locked, returns true.
 * If free, locks and returns false.
 * 
 * @return 
 */
bool p6_is_semaphore_locked() {
    if (isTravelAgendaLocked) {
        return true;
    }
    
    p6_lock_semaphore();
    return false;
}

/**
 * Adds travel information to the next
 * available (-1) entry in the relevant queues.
 * 
 * @param distanceLeft Delta distance to travel left
 * @param distanceRight Delta distance to travel right
 * @param speedLeft Speed of left motor
 * @param speedRight Speed of left motor
 * @param delayAfter Delay in seconds to wait after movement is finished
 * @param shouldFlipDirection Perform action on left (false) or right (true)
 */
void p6_queue_travel(
    int distanceLeft,
    int distanceRight,
    int speedLeft,
    int speedRight,
    float delayAfter,
    bool shouldFlipDirection) {
    int i;
    
    // Find next non -1 entry and set it
    for (i = 0; i < MAX * 2; i += 2) {
        if (travelDistanceArray[i] == -1) {
            
            // Changes the direction of actions based on which IR sensor
            // detected the gap
            // shouldFlipDirection = false means perform as normal on the left side
            int leftIndex = shouldFlipDirection ? i + 1 : i;
            int rightIndex = shouldFlipDirection ? i : i + 1;
            
            travelDistanceArray[leftIndex] = distanceLeft;
            travelDistanceArray[rightIndex] = distanceRight;
            travelSpeedArray[leftIndex] = speedLeft;
            travelSpeedArray[rightIndex] = speedRight;
            travelDelayArray[i] = delayAfter;

            break;
        }
    }
}

/**
 * Continues traveling until the desired distance
 * is reached.
 */
void p6_travel_agenda() {
    if (p6_is_semaphore_locked()) {
        return;
    }
 
    if (isTraveling) {
        bool leftReachedDistance = abs(currentStepsLeft - startStepsLeft) >= distanceDeltaLeft;
        bool rightReachedDistance = abs(currentStepsRight - startStepsRight) >= distanceDeltaRight;

        if (leftReachedDistance && rightReachedDistance) {
            isFinishedTraveling = true;
        } else {
            // TODO: Potential to stop either motor when the goal is reached
            
            currentStepsLeft = abs(e_get_steps_left());
            currentStepsRight = abs(e_get_steps_right());
        }
    }
    
    p6_unlock_semaphore();
}

/**
 * Determines when and which travel action
 * to execute in the queue.
 */
void p6_travel_manager_agenda() {
    if (p6_is_semaphore_locked()) {
        return;
    }
    
    // New item is at the front of the array
    if (travelDistanceArray[currentTravelDistanceIndex] != -1) {
        int speedLeft = travelSpeedArray[currentTravelDistanceIndex];
        int speedRight = travelSpeedArray[currentTravelDistanceIndex + 1];
        int distanceLeft = travelDistanceArray[currentTravelDistanceIndex];
        int distanceRight = travelDistanceArray[currentTravelDistanceIndex + 1];
        
        if (!isTraveling) {
            // TODO: Potential here to combine left/right into same object
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

    p6_unlock_semaphore();
}

/**
 * Convert centimeters to steps
 * 
 * @param cm
 * @return steps
 */
float p6_cm_to_steps(int cm) {
    float PI = 3.141592;
    float steps = (cm * 1000) / PI / WHEEL_DIAMETER_CM;

    return steps;
}

/**
 * Obtains the length of the e-puck in steps
 * 
 * @return Length of e-puck in steps
 */
float p6_get_puck_length_in_steps() {
    int puckLengthInSteps = p6_cm_to_steps(E_PUCK_DIAMETER_CM);

    return puckLengthInSteps;
}

/**
 * Perform a parallel parking movement.
 * 
 * The movement will be different depending on the size
 * of the parking space.
 * 
 * @param gapSize The size of the gap in steps
 * @param IRIndex IR sensor that detected the gap, used to flip movement
 */
void p6_parallel_park(int gapSize, int IRIndex) {
    int FORWARD_SPEED = 1000;
    int REVERSE_SPEED = -FORWARD_SPEED;
    
    int puckLengthInSteps = p6_get_puck_length_in_steps();
    
    // Defaults to performing movements to the left
    bool shouldFlipDirection = IRIndex == RIGHT_IR_INDEX;

    // Always move forwards half an e-puck length
    p6_queue_travel(puckLengthInSteps * 0.5, puckLengthInSteps * 0.5, FORWARD_SPEED, FORWARD_SPEED, 0.5, shouldFlipDirection);

    if (gapSize <= puckLengthInSteps * 3) {
        // Smaller gap, make e-puck go straight in with no "frills"
        
        // Move backwards a whole e-puck length
        p6_queue_travel(puckLengthInSteps, puckLengthInSteps, REVERSE_SPEED, REVERSE_SPEED, 0, shouldFlipDirection);
        
        // Turn (anti-)clockwise 90 degrees
        p6_queue_travel(300, 300, FORWARD_SPEED, REVERSE_SPEED, 0, shouldFlipDirection);
        
        // Move forwards into space
        p6_queue_travel(puckLengthInSteps, puckLengthInSteps, REVERSE_SPEED, REVERSE_SPEED, 0, shouldFlipDirection);

        // Turn (anti-)clockwise 90 degrees
        p6_queue_travel(300, 300, REVERSE_SPEED, FORWARD_SPEED, 0, shouldFlipDirection);
    } else if (gapSize <= puckLengthInSteps * 4) {
        // Slightly bigger gap, perform smooth entry

        // Move backwards half an e-puck length
        p6_queue_travel(puckLengthInSteps, puckLengthInSteps, REVERSE_SPEED, REVERSE_SPEED, 0, shouldFlipDirection);

        // Curve in the parking space
        p6_queue_travel(800 * 0.20, 800, REVERSE_SPEED * 0.20, REVERSE_SPEED, 0, shouldFlipDirection);
        
        // Curve to straighten up
        p6_queue_travel(800, 800 * 0.20, REVERSE_SPEED, REVERSE_SPEED * 0.20, 0, shouldFlipDirection);
    } else {
        // Human-like entry

        // Move backwards an e-puck length
        p6_queue_travel(puckLengthInSteps, puckLengthInSteps, REVERSE_SPEED, REVERSE_SPEED, 0.5, shouldFlipDirection);

        // Turn 45 degrees left
        p6_queue_travel(0, 300, 0, REVERSE_SPEED, 1.5, shouldFlipDirection);

        // Travel backwards
        p6_queue_travel(850, 850, REVERSE_SPEED, REVERSE_SPEED, 0.5, shouldFlipDirection);

        // Turn 45 degrees left
        p6_queue_travel(300, 0, REVERSE_SPEED, 0, 1.5, shouldFlipDirection);

        // Travel closer to the wall again
        p6_queue_travel(250, 250, FORWARD_SPEED, FORWARD_SPEED, 1.5, shouldFlipDirection);

        // -- AND OUT AGAIN --

        // Turn 45 degrees left
        p6_queue_travel(300, 0, FORWARD_SPEED, 0, 1.5, shouldFlipDirection);

        // Travel backwards
        p6_queue_travel(850, 850, FORWARD_SPEED, FORWARD_SPEED, 0.5, shouldFlipDirection);

        // Turn 45 degrees left
        p6_queue_travel(0, 300, 0, FORWARD_SPEED, 1.5, shouldFlipDirection);

        // Move backwards an e-puck length
        p6_queue_travel(1000, 1000, FORWARD_SPEED, FORWARD_SPEED, 5, shouldFlipDirection);
    }
}

/**
 * Bring the e-puck to a halt
 */
void p6_stop() {
    p6_set_speed(0, 0);
}

/**
 * Gets the total steps of both motors,
 * with both motor step counters made positive.
 * 
 * @return The total number of steps from both motors
 */
int p6_get_steps() {
    return abs(e_get_steps_left()) + abs(e_get_steps_right());
}

/**
 * To avoid noise, we take multiple readings from the IR
 * sensor and average the result.
 * 
 * @param IRIndex The IR sensor to retrieve data from
 * @param confidenceRequired How many times to test
 * @return A confident IR reading
 */
int p6_get_confident_ir_reading(int IRIndex, int confidenceRequired) {
    int totalIRValue = 0;
    int i = 0;

    for (i = 0; i < confidenceRequired; i++) {
        p6_sense(); // Update all sensor values
        totalIRValue += p6_ir_val[IRIndex];
    }

    return totalIRValue / confidenceRequired;
}

/**
 * Turns on some number of red LEDs
 * 
 * @param ledsOn Number of LEDs to turn on
 */
void p6_switch_led(float ledsOn) {
    int i;

    for (i = 0; i < 8; i++) {
        e_set_led(i, i <= ledsOn ? 1 : 0);
    }
}

/**
 * Gets the size in steps since the
 * start of a while loop
 * 
 * @param gapStart When we started checking the gap size
 * @return The current size of the gap since the start
 */
int p6_get_gap_size(int gapStart) {
    int currentGapSize = p6_get_steps();
    int gapSize = currentGapSize - gapStart;

    return gapSize;
}

/**
 * Checks an IR sensor to see if there's a gap
 * 
 * @param IRIndex The IR sensor to check
 * @return boolean depending on if IR sensor is clear (true) or not (false)
 */
bool p6_is_gap(int IRIndex) {
    int CONFIDENCE_REQUIRED = 3; // Take multiple readings from IR sensor to avoid noise
    int DISTANCE_THRESHOLD = 95; // How far away we should consider "a gap"
    int IRReading = p6_get_confident_ir_reading(IRIndex, CONFIDENCE_REQUIRED);

    bool isBlocked = IRReading <= DISTANCE_THRESHOLD; // Blocked when IR closer to 0

    return !isBlocked;
}

/**
 * Determines which side of the e-puck the gap is on
 * 
 * @return IR sensor detecting gap, -1 if both blocked
 */
int p6_get_gap_side() {
    if (p6_is_gap(LEFT_IR_INDEX)) {
        return LEFT_IR_INDEX;
    } else if (p6_is_gap(RIGHT_IR_INDEX)) {
        return RIGHT_IR_INDEX;
    }
    
    return -1;
}

/**
 * Once an opening has been found, start a counter
 * to measure how large the gap is.
 * 
 * When the gap size reaches a certain threshold,
 * we can determine that it's a big enough gap.
 * 
 * Returns the size of the gap, where the gap
 * must be between the MINIMUM_GAP_SIZE
 * and the MAXIMUM_GAP_SIDE
 * 
 * @param IRIndex The IR sensor that detected the gap opening
 * @return The side of the gap
 */
int p6_is_big_gap(int IRIndex) {
    int gapStart = p6_get_steps();
    int puckLengthInSteps = p6_get_puck_length_in_steps();
    int MINIMUM_GAP_SIZE = puckLengthInSteps * 2; // "Big Gaps" need to be at least 2 ePucks in size
    int MAXIMUM_GAP_SIZE = puckLengthInSteps * 5; // Stop counting and return the gap size if we reach some maximum

    while (1) {
        bool isEndOfGap = !p6_is_gap(IRIndex);
        
        if (isEndOfGap) {
            return -1;
        }
        
        int gapSize = p6_get_gap_size(gapStart);

        // LEDs will slowly turn on as the gap size approaches the minimum
        p6_switch_led(((float) gapSize / (float) MINIMUM_GAP_SIZE) * 8);
        
        // It's now a big gap. Keep reading the gap size until we reach the MAXIMUM
        if (gapSize >= MINIMUM_GAP_SIZE) {
            
            // Keep going to the MAXIMUM_GAP_SIZE, since we can do a more fancy
            // parking movement based on how much room we've got
            while (p6_is_gap(IRIndex) && gapSize < MAXIMUM_GAP_SIZE) {
                gapSize = p6_get_gap_size(gapStart);
            }
            
            return gapSize;
        }
    }
}

// NOTE: MUST start e-puck away from any boxes while it calibrates the IR sensors

/**
 * Drives along a road and tries to find a parking
 * space to the right of left.
 * 
 * Depending on the size of the space, the ePuck will
 * perform different parking movements.
 */
void p6_run(void) {
    // TODO: Find wall, use straighten up function, then continue

    e_activate_agenda(p6_travel_agenda, 10);
    e_activate_agenda(p6_travel_manager_agenda, 10);

    p6_set_speed(1000, 0);

    p6_switch_led(0);

    while (1) {
        int IRIndex = p6_get_gap_side();

        // If it's the start of an opening
        if (IRIndex != -1) {
            int bigGapSize = p6_is_big_gap(IRIndex);

            // Stop if it's a big enough gap to fit the ePuck
            if (bigGapSize > -1) {
                p6_switch_led(8);
                p6_parallel_park(bigGapSize, IRIndex);
                break;
            }
        } else {
            // TODO: Straighten up here, currently assume user places robot straight;
        }

        int steps = p6_get_steps();

        // If this is taking too long, stop anyway
        if (steps > 8000) {
            p6_stop();
            break;
        }
    }
    
    while(1) {}
}

//
// Currently unused and untested straightening up logic
//

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