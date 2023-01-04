#ifndef __LOCAL_H_
#define __LOCAL_H_

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define g_number_of_manufacturing_lines_to_produce_the_chocolate_products 7

#define g_number_of_manufacturing_lines_to_produce_the_chocolate_typeA 3
#define g_number_of_manufacturing_lines_to_produce_the_chocolate_typeB 2
#define g_number_of_manufacturing_lines_to_produce_the_chocolate_typeC 2

#define g_number_of_employees_per_manufacturing_line_each_executing_a_step_in_the_chocolate_manufacturing_process_typeA 8
#define g_number_of_employees_per_manufacturing_line_each_executing_a_step_in_the_chocolate_manufacturing_process_typeB 6
#define g_number_of_employees_per_manufacturing_line_each_executing_a_step_in_the_chocolate_manufacturing_process_typeC 5


#define g_min_time_for_Each_step_typeA 0 //!?
#define g_max_time_for_Each_step_typeA 0 //!?
#define g_min_time_for_Each_step_typeB 0 //!?
#define g_max_time_for_Each_step_typeB 0 //!?
#define g_min_time_for_Each_step_typeC 0 //!?
#define g_max_time_for_Each_step_typeC 0 //!?

/* 2 employees for all types , or 2 employees for each type //!? */
#define g_number_of_employees_to_collect_chocolate_in_patches_of_10_pieces_per_type_and_sent_to_printing_the_expiration_date 2
#define g_number_of_pieces_per_patches 10 //!?

#define g_time_for_printing_the_expiration_date_per_patches 0 //!?

#define g_number_of_employess_to_move_from_printing_expiration_date_machine_to_containers_according_to_the_type_of_chocolate 3

#define g_number_of_employess_to_fill_the_carton_boxes_from_the_containers 3
#define g_number_of_chocolate_products_per_carton 20 //!? chocolate_products == patches

#define g_number_of_storage_employees_work_to_move_the_filled_carton_boxes_and_place_them_in_the_storge_area 2


#define g_number_of_loading_employees_to_load_trucks_with_the_merchandise_from_storage_area 2

#define g_number_of_shipping_trucks 3

#endif
