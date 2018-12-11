#ifndef P5_H
#define	P5_H

void p5_run();
void p5_set_goal(int x, int y);
void p5_compute_metrics();
p1_V p5_move_towards_goal();
p1_V p5_move_towards_goal_smooth(p1_V v);

#endif