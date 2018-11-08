#include <stdlib.h>
#include <motor_led/e_epuck_ports.h>

void delay(long time) {
    long i;
    for (i = 0; i < time; i++) {
        NOP();
    }
}

int between(int n, int low, int high) {
    return max(min(n, high), low);
}

int scale(int n, int from, int to) {
    return n * to / from;
}
