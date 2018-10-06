#include "xc.h"
#include "motor_led/e_init_port.h"
#include "motor_led/e_epuck_ports.h"
#include "motor_led/e_motors.h"

void wait(long time) {
    long i;
    for (i = 0; i < time; i++) {
        asm("nop");
    }
}

int main(void) {
    e_init_port();

    int led = 0;
    while (1) {
        LED0 = led = led^1;
        wait(300000);
    }

    return 0;
}
