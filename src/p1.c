#include "motor_led/advance_one_timer/e_led.h"
#include "common.h"

void p1_run() {
    int led = 0;
    while (1) {
        led = led^1;
        e_set_led(1, led);
        wait(300000);
    }
}
