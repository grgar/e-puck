#include <motor_led/advance_one_timer/fast_agenda/e_agenda_fast.h>
#include "common.h"
#include "p3.h"


//Curiousity: The robot is intrigued by the light, and will turn to face it and
//follow it to investigate. Once the light source disappears, the robot
//will move in the direction it last saw the light.

void p2_run() {
    e_activate_agenda(p3_sense, 1000);
    while (1) {
        //Calls the moving function without the code that tells it not to
        //move if there's no light, so it will go in the direction the
        //light was last seen to investigate
        p3_move_if_light();
    }
}
