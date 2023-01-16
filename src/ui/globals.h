#ifndef __UI_GLOBALS_H_
#define __UI_GLOBALS_H_

#include "../include.h"
#include "./structs.h"
#include "./hash_table.h"

LocationObject *locations_ptrs[24];

LocationObject
    // manufacturing lines
    *manufacturing_line_a1_location_obj,
    *manufacturing_line_a2_location_obj,
    *manufacturing_line_a3_location_obj,

    *manufacturing_line_b1_location_obj,
    *manufacturing_line_b2_location_obj,

    *manufacturing_line_c1_location_obj,
    *manufacturing_line_c2_location_obj,

    // patching
    *patching_a_location_obj,
    *patching_b_location_obj,
    *patching_c_location_obj,

    // printing
    *printer_location_obj,

    // containers
    *container_a_location_obj,
    *container_b_location_obj,
    *container_c_location_obj,

    // carton boxes
    *carton_box_a_location_obj,
    *carton_box_b_location_obj,
    *carton_box_c_location_obj,

    // storage
    *storage_area_a_location_obj,
    *storage_area_b_location_obj,
    *storage_area_c_location_obj,

    // trucks
    *truck_1_location_obj,
    *truck_2_location_obj,
    *truck_3_location_obj;

// Person *people[500];

int people_count = 10;

int simulation_finished = 0;

double male_rolling_gate_rotation = 0;
double female_rolling_gate_rotation = 0;

#endif
