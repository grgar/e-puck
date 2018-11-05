#include <stdlib.h>
#include <a_d/advance_ad_scan/e_prox.h>
#include <motor_led/advance_one_timer/fast_agenda/e_led.h>
#include <motor_led/advance_one_timer/fast_agenda/e_motors.h>
#include <motor_led/advance_one_timer/fast_agenda/e_agenda_fast.h>
#include "common.h"

int p4_irv[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int p4_ir_front = 0;

void p4_ir() {
    int i;
    for (i = 0; i < 8; i++) {
        p4_irv[i] = 100 - (min(e_get_calibrated_prox(i), 3000) / 30);
    }
    p4_ir_front = max(p4_irv[0], p4_irv[7]);
    if (p4_ir_front > 90) {
        e_set_led(3, 1);
    } else {
        e_set_led(3, 0);
    }
}

void p4_led_display(){
    p4_ir();
    int i;
    for(i=0; i<8;i++){
        e_set_led(i,0);
        if(p4_irv[i] > 200){
            e_set_led(i,1);
        };
    }
}

void p4_led_test(){
    int i;
    for(i=0;i<8;i++){
        //sets each led to on value
        e_start_timer_processing(10000);
        
        e_end_agendas_processing(10000);
        e_set_led(i,1);
    }
}

void p4_led_identify(){
    e_set_led(4,1);
}


void p4_run() {
    //p4_led_test();
    //while(1);
    p4_led_test();
    //e_activate_agenda(p4_led_identify, 10000);
   
}
