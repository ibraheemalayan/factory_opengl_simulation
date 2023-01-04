#include "local.h"

//..................user-defined-Read from input file.................
float g_time_for_taking_a_particular_box_to_the_storage_area_as_absent_for_the_storage_employee = 0;

//Manufacturing lines must be suspended depends on
int g_maximum_number_threshold_of_manufactured_chocolate_carton_boxes_in_the_storage_area = 0;
int g_minimum_number_threshold_of_manufactured_chocolate_carton_boxes_in_the_storage_area = 0;

int g_maximum_capacity_for_each_truck =0;
int g_number_of_carton_boxs_truck_can_hold_from_typeA =0;
int g_number_of_carton_boxs_truck_can_hold_from_typeB =0;
int g_number_of_carton_boxs_truck_can_hold_from_typeC =0;

float g_time_for_shipping_the_chocolate_cartons_for_truck1 =0;
float g_time_for_shipping_the_chocolate_cartons_for_truck2 =0;
float g_time_for_shipping_the_chocolate_cartons_for_truck3 =0;

int g_number_of_carton_boxs_threshold_to_produce_to_end_simulation_for_typeA =0;
int g_number_of_carton_boxs_threshold_to_produce_to_end_simulation_for_typeB =0;
int g_number_of_carton_boxs_threshold_to_produce_to_end_simulation_for_typeC =0;

int g_time_to_end_simulation_in_minutes=0;

//.................Functions....................
void createThreads();
void simulation();
void readInputFile();


void main(){
        simulation();

}
void createThreads(){

//Minimum number of threads 56 !?
/*number of threads for employess of producing the chocolate products in the 7 lines : 46 threads
        TypeA : 3*8 =24
        TypeB : 2*6 =12
        TypeC : 2*5 =10
*/

/* number of threads for employess of collect_chocolate_in_patches_of_10_pieces_per_type_and_sent_to_printing_the_expiration_date : 2 */
// What about printing_the_expiration_date line 8 should be as thread //!?

/* number of threads for employess for moving_from_printing_expiration_date_machine_to_containers 3 */

/* number of threads for employess for fill_the_carton_boxes_from_the_containers 3 */

/* number of threads for storage employess to_move_the_filled_carton_boxes_and_place_them_in_the_storge_area 2 */
//Storage area == Queue //!?

}

void simulation(){
        readInputFile();
        createThreads();
}

void readInputFile(){
        FILE * fp;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;

        fp = fopen("inputfile.txt", "r");
        if (fp == NULL)
                exit(EXIT_FAILURE);
        int lineNumber = 0;
        while ((read = getline(&line, &len, fp)) != -1) {
                lineNumber++;
                if (lineNumber == 1){
                        printf("%s\n", line);
                        //Splite the line
                        char delim[] = " ";
                        char *ptr = strtok(line, delim);
                        ptr = strtok(NULL, delim);
                        g_time_to_end_simulation_in_minutes = atoi(ptr);
                }
                else if (lineNumber == 2){
                        printf("%s\n", line);
                        //Splite the line
                        char delim[] = " ";
                        char *ptr = strtok(line, delim);
                        ptr = strtok(NULL, delim);
                        g_number_of_carton_boxs_threshold_to_produce_to_end_simulation_for_typeA = atoi(ptr);
                }
                else if (lineNumber == 3){
                        printf("%s\n", line);
                        //Splite the line
                        char delim[] = " ";
                        char *ptr = strtok(line, delim);
                        ptr = strtok(NULL, delim);
                        g_number_of_carton_boxs_threshold_to_produce_to_end_simulation_for_typeB = atoi(ptr);
                }
                else if (lineNumber == 4){
                        printf("%s\n", line);
                        //Splite the line
                        char delim[] = " ";
                        char *ptr = strtok(line, delim);
                        ptr = strtok(NULL, delim);
                        g_number_of_carton_boxs_threshold_to_produce_to_end_simulation_for_typeC = atoi(ptr);
                }
                else if (lineNumber == 5){
                        printf("%s\n", line);
                        //Splite the line
                        char delim[] = " ";
                        char *ptr = strtok(line, delim);
                        ptr = strtok(NULL, delim);
                        g_minimum_number_threshold_of_manufactured_chocolate_carton_boxes_in_the_storage_area = atoi(ptr);
                }
                else if (lineNumber == 6){
                        printf("%s\n", line);
                        //Splite the line
                        char delim[] = " ";
                        char *ptr = strtok(line, delim);
                        ptr = strtok(NULL, delim);
                        g_maximum_number_threshold_of_manufactured_chocolate_carton_boxes_in_the_storage_area = atoi(ptr);
                }
                else if (lineNumber == 7){
                        printf("%s\n", line);
                        //Splite the line
                        char delim[] = " ";
                        char *ptr = strtok(line, delim);
                        ptr = strtok(NULL, delim);
                        g_maximum_capacity_for_each_truck = atoi(ptr);
                }
                else if (lineNumber == 8){
                        printf("%s\n", line);
                        //Splite the line
                        char delim[] = " ";
                        char *ptr = strtok(line, delim);
                        ptr = strtok(NULL, delim);
                        g_time_for_taking_a_particular_box_to_the_storage_area_as_absent_for_the_storage_employee = atoi(ptr);
                }
                else if (lineNumber == 9){
                        printf("%s\n", line);
                        //Splite the line
                        char delim[] = " ";
                        char *ptr = strtok(line, delim);
                        ptr = strtok(NULL, delim);
                        g_time_for_shipping_the_chocolate_cartons_for_truck1 = atoi(ptr);
                }
                else if (lineNumber == 10){
                        printf("%s\n", line);
                        //Splite the line
                        char delim[] = " ";
                        char *ptr = strtok(line, delim);
                        ptr = strtok(NULL, delim);
                        g_time_for_shipping_the_chocolate_cartons_for_truck2 = atoi(ptr);
                }
                else if (lineNumber == 11){
                        printf("%s\n", line);
                        //Splite the line
                        char delim[] = " ";
                        char *ptr = strtok(line, delim);
                        ptr = strtok(NULL, delim);
                        g_time_for_shipping_the_chocolate_cartons_for_truck3 = atoi(ptr);
                }
                else if (lineNumber == 12){
                        printf("%s\n", line);
                        //Splite the line
                        char delim[] = " ";
                        char *ptr = strtok(line, delim);
                        ptr = strtok(NULL, delim);
                        g_number_of_carton_boxs_truck_can_hold_from_typeA = atoi(ptr);
                }
                else if (lineNumber == 13){
                        printf("%s\n", line);
                        //Splite the line
                        char delim[] = " ";
                        char *ptr = strtok(line, delim);
                        ptr = strtok(NULL, delim);
                        g_number_of_carton_boxs_truck_can_hold_from_typeB = atoi(ptr);
                }
                else if (lineNumber == 14){
                        printf("%s\n", line);
                        //Splite the line
                        char delim[] = " ";
                        char *ptr = strtok(line, delim);
                        ptr = strtok(NULL, delim);
                        g_number_of_carton_boxs_truck_can_hold_from_typeC = atoi(ptr);
                }



        }

        fclose(fp);
}
