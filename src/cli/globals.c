#include "local.h"
#ifndef __CLI_GLOBALS_H_
//..................user-defined-Read from input file.................

int type_A_patch = 0;
int type_B_patch = 0;
int type_C_patch = 0;

float g_time_for_taking_a_particular_box_to_the_storage_area_as_absent_for_the_storage_employee = 0;

// Manufacturing lines must be suspended depends on
int g_maximum_number_threshold_of_manufactured_chocolate_carton_boxes_in_the_storage_area = 0;
int g_minimum_number_threshold_of_manufactured_chocolate_carton_boxes_in_the_storage_area = 0;

int storage_area_box_count = 0;

int g_maximum_capacity_for_each_truck = 0;
int g_number_of_carton_boxs_truck_can_hold_from_typeA = 0;
int g_number_of_carton_boxs_truck_can_hold_from_typeB = 0;
int g_number_of_carton_boxs_truck_can_hold_from_typeC = 0;

float g_time_for_shipping_the_chocolate_cartons_for_truck1 = 0;
float g_time_for_shipping_the_chocolate_cartons_for_truck2 = 0;
float g_time_for_shipping_the_chocolate_cartons_for_truck3 = 0;

int G_BOXES_TO_END_OF_TYPE_A = 0;
int G_BOXES_TO_END_OF_TYPE_B = 0;
int G_BOXES_TO_END_OF_TYPE_C = 0;

int G_MINUTES_TO_END = 0;

int id_counter = 1;
long cnt = 0;

int patch_id_A = 200000;
int patch_id_B = 300000;
int patch_id_C = 400000;

int arr_A[10];
int arr_B[10];
int arr_C[10];

#define __CLI_GLOBALS_H_
#endif