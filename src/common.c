#include <stdlib.h>
#include <motor_led/e_epuck_ports.h>

void delay(long time) {
    long i;
    for (i = 0; i < time; i++) {
        NOP();
    }
}

int between(int in, int low, int high) {
    return max(min(in, high), low);
}
