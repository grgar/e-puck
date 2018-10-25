void delay(long time) {
    long i;
    for (i = 0; i < time; i++) {
        asm("nop");
    }
}
