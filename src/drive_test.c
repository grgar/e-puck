#include "xc.h"
#include "motor_led/e_init_port.h"
#include "motor_led/e_epuck_ports.h"
#include "motor_led/e_motors.h"

void wait(int time) {
    int i;
    for (i = 0; i < time; i++) {
        asm("nop");
    }
}

int main(void) {
    e_init_motors();
    e_set_speed_left(200);
    e_set_speed_right(200);
    wait(300000);
    e_set_speed_left(0);
    e_set_speed_right(0);
    return 0;
}
