#ifndef P1_H
#define	P1_H

void p1_run();

typedef struct {
    int speed;
    int direction;
} p1_V;

extern p1_V p1_v;
p1_V p1_obstacle(p1_V);

void p1_sense();
void p1_drive();
void p1_sense_ir(int val[8]);

#endif
