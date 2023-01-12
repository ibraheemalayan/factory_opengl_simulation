#include "local.h"
#include "utils.h"
#include "globals.c"

// Arrays of threads
// Array of threads of_employees_per_manufacturing_line_each_executing_a_step_in_the_chocolate_manufacturing_process_typeA
pthread_t g_Array_of_Threads_TypeA[3][8];
// Array of threads of_employees_per_manufacturing_line_each_executing_a_step_in_the_chocolate_manufacturing_process_typeB
pthread_t g_Array_of_Threads_TypeB[2][6];
// Array of threads of_employees_per_manufacturing_line_each_executing_a_step_in_the_chocolate_manufacturing_process_typeC
pthread_t g_Array_of_Threads_TypeC[2][5];

// Arrays of mutex //Steps 1 to 6 have to happen in order
pthread_mutex_t g_mutexs_for_lines_of_typeB[2][6] = {{PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER}, {PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER}};
int g_count_for_mutex_index_typeB_line1 = 0;
int g_count_for_mutex_index_typeB_line2 = 0;

int g_number_of_threads_per_project = 0;

//.................Functions....................
void load_user_defined_values();
void start_simulation();
void create_employees_threads_type_A();
void create_employees_threads_type_B();
void create_employees_threads_type_C();
int randomIntegerInRange(int lower, int upper);
void employeeDoStepPerLine(void *time_for_emp_takes_to_finsh_step_per_line);
void employeeDoStepPerLine_TypeB(void *line_number_as_index);

void main()
{
    load_user_defined_values();

    create_and_setup_message_queues();

    run_gui();

    // register signal handler for SIGINT to clean up
    signal(SIGINT, interrupt_sig_handler);

    //start_simulation();
    testPrintingQueue();

    clean_up();

    return 0;
}

void start_simulation()
{
    // Minimum number of threads 56 !?

    /*number of threads for employess of producing the chocolate products in the 7 lines : 46 threads
        TypeA : 3*8 =24
        TypeB : 2*6 =12
        TypeC : 2*5 =10
    */
    // create_employees_threads_type_A();
    create_employees_threads_type_B();
    // create_employees_threads_type_C();

    /* number of threads for employess of collect_chocolate_in_patches_of_10_pieces_per_type_and_sent_to_printing_the_expiration_date : 2 */
    // What about printing_the_expiration_date line 8 should be as thread //!?

    /* number of threads for employess for moving_from_printing_expiration_date_machine_to_containers 3 */

    /* number of threads for employess for fill_the_carton_boxes_from_the_containers 3 */

    /* number of threads for storage employess to_move_the_filled_carton_boxes_and_place_them_in_the_storge_area 2 */
    // Storage area == Queue //!?
}
void create_employees_threads_type_A()
{
    int i, j, time_for_emp_takes_to_finsh_step_per_line_A;
    for (j = 0; j < C_MANUFACTURING_LINES_TYPEA; j++)
    {
        for (i = 0; i < C_EMPLOYEES_PER_LINE_IN_MANUFACTURING_PROCESS_TYPE_A; i++)
        {
            time_for_emp_takes_to_finsh_step_per_line_A = randomIntegerInRange(C_MIN_A, C_MAX_A);
            pthread_create(&g_Array_of_Threads_TypeA[j][i], NULL, (void *)employeeDoStepPerLine, (void *)&time_for_emp_takes_to_finsh_step_per_line_A);
            pthread_join(g_Array_of_Threads_TypeA[j][i], NULL);
            g_number_of_threads_per_project++;
            printf("\n%d\n", g_number_of_threads_per_project);
        }
    }
}

void create_employees_threads_type_B()
{
    int i, j;
    for (j = 0; j < C_MANUFACTURING_LINES_TYPEB; j++)
    {
        for (i = 0; i < C_EMPLOYEES_PER_LINE_IN_MANUFACTURING_PROCESS_TYPE_B; i++)
        {
            pthread_create(&g_Array_of_Threads_TypeB[j][i], NULL, (void *)employeeDoStepPerLine_TypeB, (void *)&j); // j as line index
            // pthread_join(g_Array_of_Threads_TypeB[j][i], NULL);
            g_number_of_threads_per_project++;
        }
    }
    for (j = 0; j < C_MANUFACTURING_LINES_TYPEB; j++)
        for (i = 0; i < C_EMPLOYEES_PER_LINE_IN_MANUFACTURING_PROCESS_TYPE_B; i++)
            pthread_join(g_Array_of_Threads_TypeB[j][i], NULL);
}

void create_employees_threads_type_C()
{
    int i, j, time_for_emp_takes_to_finsh_step_per_line_C;
    for (j = 0; j < C_MANUFACTURING_LINES_TYPEC; j++)
    {
        for (i = 0; i < C_EMPLOYEES_PER_LINE_IN_MANUFACTURING_PROCESS_TYPE_C; i++)
        {
            time_for_emp_takes_to_finsh_step_per_line_C = randomIntegerInRange(C_MIN_C, C_MAX_C);
            pthread_create(&g_Array_of_Threads_TypeC[j][i], NULL, (void *)employeeDoStepPerLine, (void *)&time_for_emp_takes_to_finsh_step_per_line_C);
            pthread_join(g_Array_of_Threads_TypeC[j][i], NULL);
            g_number_of_threads_per_project++;
            printf("\n%d\n", g_number_of_threads_per_project);
        }
    }
}
void employeeDoStepPerLine(void *time_for_emp_takes_to_finsh_step_per_line)
{
    int time = *((int *)time_for_emp_takes_to_finsh_step_per_line);
    sleep(time);
}
void employeeDoStepPerLine_TypeB(void *line_number_as_index)
{ // pass line identifaier

    // test on typeB lines : 2 lines and Steps 1 to 6 have to happen in order
    int line_number_index = *((int *)line_number_as_index);
    int time = randomIntegerInRange(C_MIN_B, C_MAX_B);
    while (1)
    {
        if (line_number_index == 0)
        {
            if (g_count_for_mutex_index_typeB_line1 == 0)
            { // Emp 1
                pthread_mutex_lock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line1]);
                printf("\nEmp line 1: %d lock\n", g_count_for_mutex_index_typeB_line1 + 1);
                sleep(time);
                pthread_mutex_unlock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line1]);
            }
            else if (g_count_for_mutex_index_typeB_line1 == 1)
            {                                                                                                                 // Emp 2
                pthread_mutex_lock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line1 - 1]); // To check if last mutex : relased
                pthread_mutex_unlock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line1 - 1]);

                pthread_mutex_lock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line1]);
                printf("\nEmp line 1: %d lock\n", g_count_for_mutex_index_typeB_line1 + 1);
                sleep(time);
                pthread_mutex_unlock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line1]);
            }
            else if (g_count_for_mutex_index_typeB_line1 == 2)
            {                                                                                                                 // Emp 3
                pthread_mutex_lock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line1 - 1]); // To check if last mutex : relased
                pthread_mutex_unlock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line1 - 1]);

                pthread_mutex_lock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line1]);
                printf("\nEmp line 1: %d lock\n", g_count_for_mutex_index_typeB_line1 + 1);
                sleep(time);
                pthread_mutex_unlock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line1]);
            }
            else if (g_count_for_mutex_index_typeB_line1 == 3)
            {                                                                                                                 // Emp 4
                pthread_mutex_lock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line1 - 1]); // To check if last mutex : relased
                pthread_mutex_unlock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line1 - 1]);

                pthread_mutex_lock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line1]);
                printf("\nEmp line 1: %d lock\n", g_count_for_mutex_index_typeB_line1 + 1);
                sleep(time);
                pthread_mutex_unlock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line1]);
            }
            else if (g_count_for_mutex_index_typeB_line1 == 4)
            {                                                                                                                 // Emp 5
                pthread_mutex_lock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line1 - 1]); // To check if last mutex : relased
                pthread_mutex_unlock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line1 - 1]);

                pthread_mutex_lock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line1]);
                printf("\nEmp line 1: %d lock\n", g_count_for_mutex_index_typeB_line1 + 1);
                sleep(time);
                pthread_mutex_unlock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line1]);
            }
            else if (g_count_for_mutex_index_typeB_line1 == 5)
            {                                                                                                                 // Emp 6
                pthread_mutex_lock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line1 - 1]); // To check if last mutex : relased
                pthread_mutex_unlock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line1 - 1]);

                pthread_mutex_lock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line1]);
                printf("\nEmp line 1: %d lock\n", g_count_for_mutex_index_typeB_line1 + 1);
                sleep(time);
                pthread_mutex_unlock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line1]);
            }

            g_count_for_mutex_index_typeB_line1++;
            if (g_count_for_mutex_index_typeB_line1 == 6)
                g_count_for_mutex_index_typeB_line1 = 0;
        }
        else
        {
            if (g_count_for_mutex_index_typeB_line2 == 0)
            { // Emp 1
                pthread_mutex_lock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line2]);
                printf("\nEmp line 2: %d lock\n", g_count_for_mutex_index_typeB_line2 + 1);
                sleep(time);
                pthread_mutex_unlock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line2]);
            }
            else if (g_count_for_mutex_index_typeB_line2 == 1)
            {                                                                                                                 // Emp 2
                pthread_mutex_lock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line2 - 1]); // To check if last mutex : relased
                pthread_mutex_unlock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line2 - 1]);

                pthread_mutex_lock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line2]);
                printf("\nEmp line 2: %d lock\n", g_count_for_mutex_index_typeB_line2 + 1);
                sleep(time);
                pthread_mutex_unlock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line2]);
            }
            else if (g_count_for_mutex_index_typeB_line2 == 2)
            {                                                                                                                 // Emp 3
                pthread_mutex_lock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line2 - 1]); // To check if last mutex : relased
                pthread_mutex_unlock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line2 - 1]);

                pthread_mutex_lock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line2]);
                printf("\nEmp line 2: %d lock\n", g_count_for_mutex_index_typeB_line2 + 1);
                sleep(time);
                pthread_mutex_unlock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line2]);
            }
            else if (g_count_for_mutex_index_typeB_line2 == 3)
            {                                                                                                                 // Emp 4
                pthread_mutex_lock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line2 - 1]); // To check if last mutex : relased
                pthread_mutex_unlock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line2 - 1]);

                pthread_mutex_lock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line2]);
                printf("\nEmp line 2: %d lock\n", g_count_for_mutex_index_typeB_line2 + 1);
                sleep(time);
                pthread_mutex_unlock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line2]);
            }
            else if (g_count_for_mutex_index_typeB_line2 == 4)
            {                                                                                                                 // Emp 5
                pthread_mutex_lock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line2 - 1]); // To check if last mutex : relased
                pthread_mutex_unlock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line2 - 1]);

                pthread_mutex_lock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line2]);
                printf("\nEmp line 2: %d lock\n", g_count_for_mutex_index_typeB_line2 + 1);
                sleep(time);
                pthread_mutex_unlock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line2]);
            }
            else if (g_count_for_mutex_index_typeB_line2 == 5)
            {                                                                                                                 // Emp 6
                pthread_mutex_lock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line2 - 1]); // To check if last mutex : relased
                pthread_mutex_unlock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line2 - 1]);

                pthread_mutex_lock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line2]);
                printf("\nEmp line 2: %d lock\n", g_count_for_mutex_index_typeB_line2 + 1);
                sleep(time);
                pthread_mutex_unlock(&g_mutexs_for_lines_of_typeB[line_number_index][g_count_for_mutex_index_typeB_line2]);
            }

            g_count_for_mutex_index_typeB_line2++;
            if (g_count_for_mutex_index_typeB_line2 == 6)
                g_count_for_mutex_index_typeB_line2 = 0;
        }

        time = randomIntegerInRange(C_MIN_B, C_MAX_B);
    }
}

void load_user_defined_values()
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("cli/inputfile.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    int lineNumber = 0;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        lineNumber++;
        if (lineNumber == 1)
        {
            printf("%s\n", line);
            // Splite the line
            char delim[] = " ";
            char *ptr = strtok(line, delim);
            ptr = strtok(NULL, delim);
            g_time_to_end_simulation_in_minutes = atoi(ptr);
        }
        else if (lineNumber == 2)
        {
            printf("%s\n", line);
            // Splite the line
            char delim[] = " ";
            char *ptr = strtok(line, delim);
            ptr = strtok(NULL, delim);
            g_number_of_carton_boxs_threshold_to_produce_to_end_simulation_for_typeA = atoi(ptr);
        }
        else if (lineNumber == 3)
        {
            printf("%s\n", line);
            // Splite the line
            char delim[] = " ";
            char *ptr = strtok(line, delim);
            ptr = strtok(NULL, delim);
            g_number_of_carton_boxs_threshold_to_produce_to_end_simulation_for_typeB = atoi(ptr);
        }
        else if (lineNumber == 4)
        {
            printf("%s\n", line);
            // Splite the line
            char delim[] = " ";
            char *ptr = strtok(line, delim);
            ptr = strtok(NULL, delim);
            g_number_of_carton_boxs_threshold_to_produce_to_end_simulation_for_typeC = atoi(ptr);
        }
        else if (lineNumber == 5)
        {
            printf("%s\n", line);
            // Splite the line
            char delim[] = " ";
            char *ptr = strtok(line, delim);
            ptr = strtok(NULL, delim);
            g_minimum_number_threshold_of_manufactured_chocolate_carton_boxes_in_the_storage_area = atoi(ptr);
        }
        else if (lineNumber == 6)
        {
            printf("%s\n", line);
            // Splite the line
            char delim[] = " ";
            char *ptr = strtok(line, delim);
            ptr = strtok(NULL, delim);
            g_maximum_number_threshold_of_manufactured_chocolate_carton_boxes_in_the_storage_area = atoi(ptr);
        }
        else if (lineNumber == 7)
        {
            printf("%s\n", line);
            // Splite the line
            char delim[] = " ";
            char *ptr = strtok(line, delim);
            ptr = strtok(NULL, delim);
            g_maximum_capacity_for_each_truck = atoi(ptr);
        }
        else if (lineNumber == 8)
        {
            printf("%s\n", line);
            // Splite the line
            char delim[] = " ";
            char *ptr = strtok(line, delim);
            ptr = strtok(NULL, delim);
            g_time_for_taking_a_particular_box_to_the_storage_area_as_absent_for_the_storage_employee = atoi(ptr);
        }
        else if (lineNumber == 9)
        {
            printf("%s\n", line);
            // Splite the line
            char delim[] = " ";
            char *ptr = strtok(line, delim);
            ptr = strtok(NULL, delim);
            g_time_for_shipping_the_chocolate_cartons_for_truck1 = atoi(ptr);
        }
        else if (lineNumber == 10)
        {
            printf("%s\n", line);
            // Splite the line
            char delim[] = " ";
            char *ptr = strtok(line, delim);
            ptr = strtok(NULL, delim);
            g_time_for_shipping_the_chocolate_cartons_for_truck2 = atoi(ptr);
        }
        else if (lineNumber == 11)
        {
            printf("%s\n", line);
            // Splite the line
            char delim[] = " ";
            char *ptr = strtok(line, delim);
            ptr = strtok(NULL, delim);
            g_time_for_shipping_the_chocolate_cartons_for_truck3 = atoi(ptr);
        }
        else if (lineNumber == 12)
        {
            printf("%s\n", line);
            // Splite the line
            char delim[] = " ";
            char *ptr = strtok(line, delim);
            ptr = strtok(NULL, delim);
            g_number_of_carton_boxs_truck_can_hold_from_typeA = atoi(ptr);
        }
        else if (lineNumber == 13)
        {
            printf("%s\n", line);
            // Splite the line
            char delim[] = " ";
            char *ptr = strtok(line, delim);
            ptr = strtok(NULL, delim);
            g_number_of_carton_boxs_truck_can_hold_from_typeB = atoi(ptr);
        }
        else if (lineNumber == 14)
        {
            printf("%s\n", line);
            // Splite the line
            char delim[] = " ";
            char *ptr = strtok(line, delim);
            ptr = strtok(NULL, delim);
            g_number_of_carton_boxs_truck_can_hold_from_typeC = atoi(ptr);
        }
    }

    fclose(fp);
}

int randomIntegerInRange(int lower, int upper)
{
    srand(time(NULL)); // randomize seed
    return (rand() % (upper - lower + 1)) + lower;
}



//New Mohammad

//.........................FUNCTIONS..............
struct chocolateNode * dequeueFromPrinterQueue();
void enqueuToPrinterQueue(struct chocolateNode *patche );

struct chocolateNode{
        char chocolateType;
        char expirationDate[20];
        
};

struct printerNode{
        struct chocolateNode patche[10];
        struct printerNode * next;       
};


// Pointers to The printer queue
struct printerNode *FrontPrinterQueue = NULL;
struct printerNode *RearPrinterQueue  = NULL;

//Mutex on printing Queue
pthread_mutex_t printingQueue_mutex = PTHREAD_MUTEX_INITIALIZER;


/*Noor : use this function with the two threads:
 "The chocolate products that are produced by all the manufacturing lines are collected in patches of 10 pieces per type by 2 employees"
 Hint 1 :each thread form these two threads should at first fillThePatche then enqueuToPrinterQueue  */ 
/*Noor:
Hint 2: dequeueFromPrinterQueue and enqueuToPrinterQueue need to test and update , Iam not sure about basing array of struct
*/
void enqueuToPrinterQueue(struct chocolateNode *patche){
        pthread_mutex_lock(&printingQueue_mutex);
      //struct personInformation person = personInf;
        struct printerNode *ptr = (struct printerNode *)malloc(sizeof(struct printerNode));
        if (ptr == NULL)
        {
                printf("\nOVERFLOW\n");
                return;
        }
        else
        {
                //ToDo Update index for ui 
                
                for (int i = 0; i < 10; i++){
                        ptr->patche[i]=patche[i];
                }
                if (FrontPrinterQueue == NULL)
                {
                        FrontPrinterQueue = ptr;
                        RearPrinterQueue = ptr;
                        FrontPrinterQueue->next = NULL;
                        RearPrinterQueue->next= NULL;
                }
                else
                {
                        RearPrinterQueue->next = ptr;
                        RearPrinterQueue = ptr;
                        RearPrinterQueue->next = NULL;
                }
        //usleep(randomIntegerInRange(SLEEP_MIN, SLEEP_MAX));
        }
        pthread_mutex_unlock(&printingQueue_mutex); 
}


struct chocolateNode * dequeueFromPrinterQueue(){
        
        pthread_mutex_lock(&printingQueue_mutex);
        struct printerNode *temp = NULL;
        struct chocolateNode *patche = NULL;
        if (FrontPrinterQueue == NULL)
        {
                printf("Underflow\n");
                return patche;
        }
        else
        {
                temp = FrontPrinterQueue;
                patche = temp->patche;
                FrontPrinterQueue = FrontPrinterQueue->next;
                free(temp);
                
        }
        pthread_mutex_unlock(&printingQueue_mutex);
        return patche; 
}
