#include <motor_led/e_epuck_ports.h>

void delay(long time) {
    long i;
    for (i = 0; i < time; i++) {
        NOP();
    }
}
