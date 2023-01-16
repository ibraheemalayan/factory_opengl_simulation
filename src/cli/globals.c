#include "local.h"
#ifndef __CLI_GLOBALS_H_
//..................user-defined-Read from input file.................
float g_time_for_taking_a_particular_box_to_the_STORAGE_AREAs_absent_for_the_storage_employee = 0;

// Manufacturing lines must be suspended depends on
int g_maximum_number_threshold_of_manufactured_chocolate_carton_boxes_in_the_storage_area = 0;
int g_minimum_number_threshold_of_manufactured_chocolate_carton_boxes_in_the_storage_area = 0;

int g_maximum_capacity_for_each_truck = 0;
int g_number_of_carton_boxs_truck_can_hold_from_typeA = 0;
int g_number_of_carton_boxs_truck_can_hold_from_typeB = 0;
int g_number_of_carton_boxs_truck_can_hold_from_typeC = 0;

float g_time_for_shipping_the_chocolate_cartons_for_truck1 = 0;
float g_time_for_shipping_the_chocolate_cartons_for_truck2 = 0;
float g_time_for_shipping_the_chocolate_cartons_for_truck3 = 0;

int g_number_of_carton_boxs_threshold_to_produce_to_end_simulation_for_typeA = 0;
int g_number_of_carton_boxs_threshold_to_produce_to_end_simulation_for_typeB = 0;
int g_number_of_carton_boxs_threshold_to_produce_to_end_simulation_for_typeC = 0;

int g_time_to_end_simulation_in_minutes = 0;

unsigned int id_counter = 100001;
unsigned int patch_id_A = 200000;
unsigned int patch_id_B = 300000;
unsigned int patch_id_C = 400000;


chocolateProduct *type_A_pile [C_MANUFACTURING_LINES_TYPEA][PILESIZE];


chocolateProduct *type_B_pile [C_MANUFACTURING_LINES_TYPEB][PILESIZE];


chocolateProduct *type_C_pile [C_MANUFACTURING_LINES_TYPEC][PILESIZE];

int type_A_patch = 0;
int type_B_patch = 0;
int type_C_patch = 0;



#define __CLI_GLOBALS_H_
#endif