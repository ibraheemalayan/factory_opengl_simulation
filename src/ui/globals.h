#ifndef __UI_GLOBALS_H_
#define __UI_GLOBALS_H_

#include "../include.h"
#include "./structs.h"
#include "./hash_table.h"

Queue *queue_A1,
    *queue_A2,
    *queue_B1,
    *queue_B2,
    *metal_detector,
    *inner_grouping_area,
    *teller_R,
    *teller_I,
    *teller_B,
    *teller_T,
    *teller_R_Q,
    *teller_I_Q,
    *teller_B_Q,
    *teller_T_Q;

// Person *people[500];

int people_count = 10;

int simulation_finished = 0;

double male_rolling_gate_rotation = 0;
double female_rolling_gate_rotation = 0;

#endif
