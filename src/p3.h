#ifndef P3_H
#define	P3_H

extern int p3_sensors[8];
void p3_run();
void p3_sense();
void p3_move_if_light();
int p3_average_light(int position);

#endif