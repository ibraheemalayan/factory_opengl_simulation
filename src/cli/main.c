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




//..................................new mohammad..................................

struct chocolateNode{
        struct Object chocolateInfo;
        struct chocolateNode * next;
        
};

//.........................GLOBALS..............

// Pointers to The printer queue
struct chocolateNode *FrontPrinterQueue = NULL;
struct chocolateNode *RearPrinterQueue  = NULL;

// Pointer to The containerTypeA queue
struct chocolateNode *FrontContainerTypeAQueue = NULL;
struct chocolateNode *RearContainerTypeAQueue  = NULL;
// Pointer to The containerTypeB queue
struct chocolateNode *FrontContainerTypeBQueue = NULL;
struct chocolateNode *RearContainerTypeBQueue  = NULL;
// Pointer to The containerTypeC queue
struct chocolateNode *FrontContainerTypeCQueue = NULL;
struct chocolateNode *RearContainerTypeCQueue  = NULL;

// Pointer to The queue of filling the carton boxes: A
struct chocolateNode *FrontFillingTheCartonBoxesTypeAQueue = NULL;
struct chocolateNode *RearFillingTheCartonBoxesTypeAQueue  = NULL;
// Pointer to The queue of filling the carton boxes: B
struct chocolateNode *FrontFillingTheCartonBoxesTypeBQueue = NULL;
struct chocolateNode *RearFillingTheCartonBoxesTypeBQueue  = NULL;
// Pointer to The queue of filling the carton boxes: C
struct chocolateNode *FrontFillingTheCartonBoxesTypeCQueue = NULL;
struct chocolateNode *RearFillingTheCartonBoxesTypeCQueue  = NULL;


// Pointer to The storagAreaTypeA queue
struct chocolateNode *FrontStoragAreaTypeAQueue = NULL;
struct chocolateNode *RearStoragAreaTypeAQueue  = NULL;
// Pointer to The storagAreaTypeB queue
struct chocolateNode *FrontStoragAreaTypeBQueue = NULL;
struct chocolateNode *RearStoragAreaTypeBQueue  = NULL;
// Pointer to The storagAreaTypeC queue
struct chocolateNode *FrontStoragAreaTypeCQueue = NULL;
struct chocolateNode *RearStoragAreaTypeCQueue  = NULL;


//Pointers to trucks queues 
struct chocolateNode *FrontTruckQueue1 = NULL;
struct chocolateNode *RearTruckQueue1  = NULL;

struct chocolateNode *FrontTruckQueue2 = NULL;
struct chocolateNode *RearTruckQueue2  = NULL;

struct chocolateNode *FrontTruckQueue3 = NULL;
struct chocolateNode *RearTruckQueue3  = NULL;


//Mutex on printing Queue
pthread_mutex_t printingQueue_mutex = PTHREAD_MUTEX_INITIALIZER;

//Mutex on Container QueueS // index 0:A, 1:B, 2:C 
pthread_mutex_t G_mutexs_for_Container_Queues[3] = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER};

//Mutex on THE Filling  Carton Boxes Queues // index 0:A, 1:B, 2:C 
pthread_mutex_t G_mutexs_for_FillingTheCartonBoxes_Queues[3] = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER};

//Mutex on Storage Area Queues // index 0:A, 1:B, 2:C 
pthread_mutex_t G_mutexs_for_StorageArea_Queues[3] = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER};

//Mutex on Trucks queues // index 0:1, 1:2, 2:3
pthread_mutex_t G_mutexs_on_Trucks_Queues[3] = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER};


int G_numberOfchocolatePatchesInPrintingQueue = 0;

//Number of chocolate products in containers
int G_numberOfchocolatePatchesInContainerTypeA = 0;
int G_numberOfchocolatePatchesInContainerTypeB = 0;
int G_numberOfchocolatePatchesInContainerTypeC = 0;


//Number of chocolate boxs in the  Queue of Filling The Carton Boxes
int G_numberOfChocolateBoxsInTheFillingCartonBoxesQueueTypeA = 0;
int G_numberOfChocolateBoxsInTheFillingCartonBoxesQueueTypeB = 0;
int G_numberOfChocolateBoxsInTheFillingCartonBoxesQueueTypeC = 0;


//Number of chocolate boxs in Storage area
int G_numberOfChocolateBoxsInStorageAreaTypeA = 0;
int G_numberOfChocolateBoxsInStorageAreaTypeB = 0;
int G_numberOfChocolateBoxsInStorageAreaTypeC = 0;

//Number of Boxes in the Trucks
int G_numberOfChocolateBoxs_Truck_A = 0;
int G_numberOfChocolateBoxs_Truck_B = 0;
int G_numberOfChocolateBoxs_Truck_C = 0;

//Conditions on Trucks
int G_isTruck1Available =0; //0:1
int G_isTruck2Available =0; //0:1
int G_isTruck3Available =0; //0:1


int G_Trucks_Order = 1; // 1:2:3





struct Object dequeueNodeFromQueueNoInternalMutex( struct chocolateNode **FrontQueue, int *numberOfchocolateItemInQueueTypeT);
struct Object dequeueNodeFromQueueWithInternalMutex(pthread_mutex_t *mutex, struct chocolateNode **FrontQueue, int *numberOfchocolateItemInQueueTypeT);
void enqueueToQueue(struct Object chocolateInfo, pthread_mutex_t *mutex, struct chocolateNode **FrontQueue, struct chocolateNode **RearQueue, int *numberOfchocolateItemInQueueTypeT);

void createThreadsForEmpsWorkfromPrinterToContainers();
void createThreadsForEmpsWorkfromContainersToFillingTheCartonBoxes();
void createThreadsForTwoEmpsWorkfromFillingTheCartonBoxesToStorageArea();
void createThreadsForTwoEmpsWorkfromStorageAreaToTruks();
void creatrThreadForPrint();

void insertToContainers();
void fillingTheCartonBoxesA();
void fillingTheCartonBoxesB();
void fillingTheCartonBoxesC();
void insertToStorageArea();
void insertToTrucks();
void printInfo();



//Test
void displyContainerQueueA();
void displyContainerQueueB();
void displyContainerQueueC();
void displyTheFillingCartonBoxesQueueA();
void displyTheFillingCartonBoxesQueueB();
void displyTheFillingCartonBoxesQueueC();
void displyStorageAreaQueueA();
void displyStorageAreaQueueB();
void displyStorageAreaQueueC();
void displyPrintingQueue();
void testPrintingQueue();
void test_simulation();

// .................................end new........................................ 



void main()
{
    
    //create_and_setup_message_queues();

    //run_gui();

    // register signal handler for SIGINT to clean up
    //signal(SIGINT, interrupt_sig_handler);

    //start_simulation();
    
    load_user_defined_values();
    //..................................new mohammad..................................

    test_simulation();
    // .................................end new........................................
    


    //clean_up();

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
            g_time_for_taking_a_particular_box_to_the_STORAGE_AREAs_absent_for_the_storage_employee = atoi(ptr);
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



//..................................new mohammad.................................. 

/*Noor : use this function with the two threads:
 "The chocolate products that are produced by all the manufacturing lines are collected in patches of 10 pieces per type by 2 employees"
 Hint 1 :each thread form these two threads should at first fillThePatche then enqueuToPrinterQueue  */ 

void enqueueToQueue(struct Object chocolateInfo, pthread_mutex_t *mutex, struct chocolateNode **FrontQueue, struct chocolateNode **RearQueue, int *numberOfchocolateItemInQueueTypeT)
{
    pthread_mutex_lock(mutex);
    struct chocolateNode *ptr = (struct chocolateNode *)malloc(sizeof(struct chocolateNode));
    if (ptr == NULL)
    {
        printf("\nOVERFLOW \n");
        return;
    }
    else
    {
        //Todo update index to ui
        ptr->chocolateInfo=chocolateInfo;
        if ((*FrontQueue) == NULL)
        {
            (*FrontQueue) = ptr;
            (*RearQueue) = ptr;
            (*FrontQueue)->next= NULL;
            (*RearQueue)->next = NULL;
        }
        else
        {
            (*RearQueue)->next = ptr;
            (*RearQueue) = ptr;
            (*RearQueue)->next = NULL;
        }
        //sleep
        (*numberOfchocolateItemInQueueTypeT)++;
    }
    pthread_mutex_unlock(mutex);
}

struct Object dequeueNodeFromQueueWithInternalMutex(pthread_mutex_t *mutex, struct chocolateNode **FrontQueue, int *numberOfchocolateItemInQueueTypeT)
{   
    
    pthread_mutex_lock(mutex);
    struct chocolateNode *temp = NULL;
    struct Object chocolate = {0};
    //Todo update index to ui
    if ((*FrontQueue) == NULL)
    {   
        printf("Underflow dequeueNodeFromQueueWithInternalMutex\n");
        return chocolate;
    }
    else
    {   
        temp = (*FrontQueue);
        chocolate = temp->chocolateInfo;
        //Todo update index to ui
        (*FrontQueue) = (*FrontQueue)->next;
        free(temp);
        (*numberOfchocolateItemInQueueTypeT)--;
    }
    pthread_mutex_unlock(mutex);
    return chocolate;
}
struct Object dequeueNodeFromQueueNoInternalMutex( struct chocolateNode **FrontQueue, int *numberOfchocolateItemInQueueTypeT)
{   
        struct chocolateNode *temp = NULL;
    struct Object chocolate = {0};
    //Todo update index to ui
    if ((*FrontQueue) == NULL)
    {   
        printf("Underflow dequeueNodeFromQueue\n");
        return chocolate;
    }
    else
    {   
        temp = (*FrontQueue);
        chocolate = temp->chocolateInfo;
        //Todo update index to ui
        (*FrontQueue) = (*FrontQueue)->next;
        free(temp);
        (*numberOfchocolateItemInQueueTypeT)--;
    }
    return chocolate;
}



void displyPrintingQueue()
{

    struct chocolateNode *temp = NULL;
    if ((FrontPrinterQueue == NULL))
    {
        printf("\n\nPrinting Queue is Empty\n");
    }
    else
    {
        printf("\n\nThe Printing Queue is :\n\n");
        temp = FrontPrinterQueue;
        while (temp)
        {
            printf("\n\nlocation:  %d  type: %d   id:%d\n\n", temp->chocolateInfo.current_location,temp->chocolateInfo.chocolate_type, temp->chocolateInfo.id);
            temp = temp->next;
        }
    }
}
void displyStorageAreaQueueA()
{

    struct chocolateNode *temp = NULL;
    if ((FrontStoragAreaTypeAQueue == NULL))
    {
        printf("\n\nStorage Area Queue A is Empty\n");
    }
    else
    {
        printf("\n\nStorage Area Queue A is :\n\n");
        temp = FrontStoragAreaTypeAQueue;
        while (temp)
        {
            printf("\n\nlocation in Storage Area Queue A:  %d  type: %d   id:%d\n\n", temp->chocolateInfo.current_location,temp->chocolateInfo.chocolate_type, temp->chocolateInfo.id);
            temp = temp->next;
        }
    }
}
void displyStorageAreaQueueB()
{

    struct chocolateNode *temp = NULL;
    if ((FrontStoragAreaTypeBQueue == NULL))
    {
        printf("\n\nStorage Area Queue B is Empty\n");
    }
    else
    {
        printf("\n\nStorage Area Queue B is :\n\n");
        temp = FrontStoragAreaTypeBQueue;
        while (temp)
        {
            printf("\n\nlocation in Storage Area Queue B:  %d  type: %d   id:%d\n\n", temp->chocolateInfo.current_location,temp->chocolateInfo.chocolate_type, temp->chocolateInfo.id);
            temp = temp->next;
        }
    }
}
void displyStorageAreaQueueC()
{

    struct chocolateNode *temp = NULL;
    if ((FrontStoragAreaTypeCQueue == NULL))
    {
        printf("\n\nStorage Area Queue C is Empty\n");
    }
    else
    {
        printf("\n\nStorage Area Queue C is :\n\n");
        temp = FrontStoragAreaTypeCQueue;
        while (temp)
        {
            printf("\n\nlocation in Storage Area Queue C:  %d  type: %d   id:%d\n\n", temp->chocolateInfo.current_location,temp->chocolateInfo.chocolate_type, temp->chocolateInfo.id);
            temp = temp->next;
        }
    }
}
void displyContainerQueueA()
{

    struct chocolateNode *temp = NULL;
    if ((FrontContainerTypeAQueue == NULL))
    {
        printf("\n\nContainer Queue A is Empty\n");
    }
    else
    {
        printf("\n\nThe Container Queue A is :\n\n");
        temp = FrontContainerTypeAQueue;
        while (temp)
        {
            printf("\n\nlocation:  %d  type: %d   id:%d\n\n", temp->chocolateInfo.current_location,temp->chocolateInfo.chocolate_type, temp->chocolateInfo.id);
            temp = temp->next;
        }
    }
}
void displyContainerQueueB()
{

    struct chocolateNode *temp = NULL;
    if ((FrontContainerTypeBQueue == NULL))
    {
        printf("\n\nContainer Queue B is Empty\n");
    }
    else
    {
        printf("\n\nThe Container Queue B is :\n\n");
        temp = FrontContainerTypeBQueue;
        while (temp)
        {
            printf("\n\nlocation:  %d  type: %d   id:%d\n\n", temp->chocolateInfo.current_location,temp->chocolateInfo.chocolate_type, temp->chocolateInfo.id);
            temp = temp->next;
        }
    }
}
void displyContainerQueueC()
{

    struct chocolateNode *temp = NULL;
    if ((FrontContainerTypeCQueue == NULL))
    {
        printf("\n\nContainer Queue C is Empty\n");
    }
    else
    {
        printf("\n\nThe Container Queue C is :\n\n");
        temp = FrontContainerTypeCQueue;
        while (temp)
        {
            printf("\n\nlocation:  %d  type: %d   id:%d\n\n", temp->chocolateInfo.current_location,temp->chocolateInfo.chocolate_type, temp->chocolateInfo.id);
            temp = temp->next;
        }
    }
}

void displyTheFillingCartonBoxesQueueA(){

    struct chocolateNode *temp = NULL;
    if ((FrontFillingTheCartonBoxesTypeAQueue == NULL))
    {
        printf("\n\nFilling Carton Boxes Queue A is Empty\n");
    }
    else
    {
        printf("\n\nFilling Carton Boxes Queue A is :\n\n");
        temp = FrontFillingTheCartonBoxesTypeAQueue;
        while (temp)
        {
            printf("\n\nlocation:  %d  type: %d   id:%d\n\n", temp->chocolateInfo.current_location,temp->chocolateInfo.chocolate_type, temp->chocolateInfo.id);
            temp = temp->next;
        }
    }


}
void displyTheFillingCartonBoxesQueueB(){

    struct chocolateNode *temp = NULL;
    if ((FrontFillingTheCartonBoxesTypeBQueue == NULL))
    {
        printf("\n\nFilling Carton Boxes Queue B is Empty\n");
    }
    else
    {
        printf("\n\nFilling Carton Boxes Queue B is :\n\n");
        temp = FrontFillingTheCartonBoxesTypeBQueue;
        while (temp)
        {
            printf("\n\nlocation:  %d  type: %d   id:%d\n\n", temp->chocolateInfo.current_location,temp->chocolateInfo.chocolate_type, temp->chocolateInfo.id);
            temp = temp->next;
        }
    }


}
void displyTheFillingCartonBoxesQueueC(){

    struct chocolateNode *temp = NULL;
    if ((FrontFillingTheCartonBoxesTypeCQueue == NULL))
    {
        printf("\n\nFilling Carton Boxes Queue C is Empty\n");
    }
    else
    {
        printf("\n\nFilling Carton Boxes Queue C is :\n\n");
        temp = FrontFillingTheCartonBoxesTypeCQueue;
        while (temp)
        {
            printf("\n\nlocation:  %d  type: %d   id:%d\n\n", temp->chocolateInfo.current_location,temp->chocolateInfo.chocolate_type, temp->chocolateInfo.id);
            temp = temp->next;
        }
    }


}


void testPrintingQueue(){
        struct Object ch1;
        ch1.chocolate_type=TYPE_A;
        ch1.current_location=PRINTER;
        ch1.pkg_type=PATCH;
        ch1.index_in_queue=0;
        ch1.id=1;
        enqueueToQueue(ch1,&printingQueue_mutex,&FrontPrinterQueue, &RearPrinterQueue, &G_numberOfchocolatePatchesInPrintingQueue);
        struct Object ch2;
        ch2.chocolate_type=TYPE_B;
        ch2.current_location=PRINTER;
        ch2.pkg_type=PATCH;
        ch2.index_in_queue=0;
        ch2.id=2;
        enqueueToQueue(ch2,&printingQueue_mutex,&FrontPrinterQueue, &RearPrinterQueue, &G_numberOfchocolatePatchesInPrintingQueue);
        struct Object ch3;
        ch3.chocolate_type=TYPE_C;
        ch3.current_location=PRINTER;
        ch3.pkg_type=PATCH;
        ch3.index_in_queue=0;
        ch3.id=3;
        enqueueToQueue(ch3,&printingQueue_mutex,&FrontPrinterQueue, &RearPrinterQueue, &G_numberOfchocolatePatchesInPrintingQueue);
        struct Object ch5;
        ch5.chocolate_type=TYPE_A;
        ch5.current_location=PRINTER;
        ch5.pkg_type=PATCH;
        ch5.index_in_queue=0;
        ch5.id=4;
        enqueueToQueue(ch5,&printingQueue_mutex,&FrontPrinterQueue, &RearPrinterQueue, &G_numberOfchocolatePatchesInPrintingQueue);
        struct Object ch4;
        ch4.chocolate_type=TYPE_B;
        ch4.current_location=PRINTER;
        ch4.pkg_type=PATCH;
        ch4.index_in_queue=0;
        ch4.id=5;
        enqueueToQueue(ch4,&printingQueue_mutex,&FrontPrinterQueue, &RearPrinterQueue, &G_numberOfchocolatePatchesInPrintingQueue);
        struct Object ch6;
        ch6.chocolate_type=TYPE_C;
        ch6.current_location=PRINTER;
        ch6.pkg_type=PATCH;
        ch6.index_in_queue=0;
        ch6.id=6;
        enqueueToQueue(ch6,&printingQueue_mutex,&FrontPrinterQueue, &RearPrinterQueue, &G_numberOfchocolatePatchesInPrintingQueue);
        
}

void insertToContainers(){
    struct Object chocolate;
    while (1)
    {   
        pthread_mutex_lock(&printingQueue_mutex);
        if(G_numberOfchocolatePatchesInPrintingQueue >0){
            chocolate = dequeueNodeFromQueueNoInternalMutex( &FrontPrinterQueue,&G_numberOfchocolatePatchesInPrintingQueue ); 
            //Todo update index to ui
            if (chocolate.chocolate_type==TYPE_A){
                chocolate.current_location=CONTAINER_A;
                enqueueToQueue(chocolate, &G_mutexs_for_Container_Queues[0], &FrontContainerTypeAQueue, &RearContainerTypeAQueue, &G_numberOfchocolatePatchesInContainerTypeA);
            }
            else if (chocolate.chocolate_type==TYPE_B){
                chocolate.current_location=CONTAINER_B;
                enqueueToQueue(chocolate, &G_mutexs_for_Container_Queues[1], &FrontContainerTypeBQueue, &RearContainerTypeBQueue, &G_numberOfchocolatePatchesInContainerTypeB);
            }
            else if (chocolate.chocolate_type==TYPE_C){
                chocolate.current_location=CONTAINER_C;
                enqueueToQueue(chocolate, &G_mutexs_for_Container_Queues[2], &FrontContainerTypeCQueue, &RearContainerTypeCQueue, &G_numberOfchocolatePatchesInContainerTypeC);
            } 
        }
        pthread_mutex_unlock(&printingQueue_mutex);  
    }
    
}

void createThreadsForEmpsWorkfromPrinterToContainers(){
    /* create a new 3 threads that will keep move chocolate from Printer Queue To Containers Queues */
    pthread_t p_thread1;
    pthread_create(&p_thread1, NULL, (void *)insertToContainers, NULL);
    pthread_t p_thread2;
    pthread_create(&p_thread2, NULL, (void *)insertToContainers, NULL);
    pthread_t p_thread3;
    pthread_create(&p_thread3, NULL, (void *)insertToContainers, NULL);
    //pthread_join(p_thread1, NULL);  
    //pthread_join(p_thread2, NULL);
    //pthread_join(p_thread3, NULL); 
}

void createThreadsForEmpsWorkfromContainersToFillingTheCartonBoxes(){
    // create a new 3 threads that will keep move chocolate from Containers Queues To filling  Carton Boxes Queues 
    pthread_t p_thread4;
    pthread_create(&p_thread4, NULL, (void *)fillingTheCartonBoxesA, NULL);
    pthread_t p_thread5;
    pthread_create(&p_thread5, NULL, (void *)fillingTheCartonBoxesB, NULL);
    pthread_t p_thread6;
    pthread_create(&p_thread6, NULL, (void *)fillingTheCartonBoxesC, NULL);
    //pthread_join(p_thread4, NULL);  
    //pthread_join(p_thread5, NULL);
    //pthread_join(p_thread6, NULL);  
  
       
}

void createThreadsForTwoEmpsWorkfromFillingTheCartonBoxesToStorageArea(){
    // create a new 2 threads that will keep move chocolate from filling  Carton Boxes Queues To Storage Area Queues 
    pthread_t p_thread7;
    pthread_create(&p_thread7, NULL, (void *)insertToStorageArea, NULL);
    pthread_t p_thread8;
    pthread_create(&p_thread8, NULL, (void *)insertToStorageArea, NULL);
    //pthread_join(p_thread7, NULL);  
    //pthread_join(p_thread8, NULL);  
}

void createThreadsForTwoEmpsWorkfromStorageAreaToTruks(){
    // create a new 2 threads that will keep move chocolate from Storage Area Queues To trucks 
    pthread_t p_thread9;
    pthread_create(&p_thread9, NULL, (void *)insertToTrucks, NULL);
    pthread_t p_thread10;
    pthread_create(&p_thread10, NULL, (void *)insertToTrucks, NULL);
    //pthread_join(p_thread9, NULL);  
    //pthread_join(p_thread10, NULL);  

}
void creatrThreadForPrint(){
    pthread_t p_thread11;
    pthread_create(&p_thread11, NULL, (void *)printInfo, NULL);
    //pthread_join(p_thread11, NULL);  

    
}

void printInfo(){

    while (1)
    {
    /*
    displyPrintingQueue();
    displyContainerQueueA();
    displyContainerQueueB();
    displyContainerQueueC();
    displyTheFillingCartonBoxesQueueA();
    displyTheFillingCartonBoxesQueueB();
    displyTheFillingCartonBoxesQueueC();
    */
    displyStorageAreaQueueA();
    displyStorageAreaQueueB();
    displyStorageAreaQueueC();
    sleep(5);        
    }
}

void insertToTrucks(){

struct Object chocolate;
int save_index_mutex;

while(1){
    while (1)
    {  
        pthread_mutex_lock(&G_mutexs_on_Trucks_Queues[G_Trucks_Order-1]);
        save_index_mutex=G_Trucks_Order-1;
        //printf("G_numberOfChocolateBoxs_Truck_A:%d\n\n",G_numberOfChocolateBoxs_Truck_A);
        //printf("g_number_of_carton_boxs_truck_can_hold_from_typeA:%d\n\n",g_number_of_carton_boxs_truck_can_hold_from_typeA);
        if(G_numberOfChocolateBoxs_Truck_A < g_number_of_carton_boxs_truck_can_hold_from_typeA){
            //printf("G_numberOfChocolateBoxsInStorageAreaTypeA:%d\n\n",G_numberOfChocolateBoxsInStorageAreaTypeA);
            if(G_numberOfChocolateBoxsInStorageAreaTypeA > 0){
                //printf("G_numberOfChocolateBoxsInStorageAreaTypeA:%d\n\n",G_numberOfChocolateBoxsInStorageAreaTypeA);
                chocolate=dequeueNodeFromQueueWithInternalMutex(&G_mutexs_for_StorageArea_Queues[0],&FrontStoragAreaTypeAQueue,&G_numberOfChocolateBoxsInStorageAreaTypeA);
                G_numberOfChocolateBoxs_Truck_A++;
                //printf("G_numberOfChocolateBoxs_Truck_A:%d\n\n",G_numberOfChocolateBoxs_Truck_A);
                break;
            }
        } 
        if(G_numberOfChocolateBoxs_Truck_B < g_number_of_carton_boxs_truck_can_hold_from_typeB){
            if(G_numberOfChocolateBoxsInStorageAreaTypeB > 0){
                chocolate=dequeueNodeFromQueueWithInternalMutex(&G_mutexs_for_StorageArea_Queues[1],&FrontStoragAreaTypeBQueue,&G_numberOfChocolateBoxsInStorageAreaTypeB);
                G_numberOfChocolateBoxs_Truck_B++;
                break;
            }    
        }
        if(G_numberOfChocolateBoxs_Truck_C < g_number_of_carton_boxs_truck_can_hold_from_typeC){
            if(G_numberOfChocolateBoxsInStorageAreaTypeC > 0){
                chocolate=dequeueNodeFromQueueWithInternalMutex(&G_mutexs_for_StorageArea_Queues[2],&FrontStoragAreaTypeCQueue,&G_numberOfChocolateBoxsInStorageAreaTypeC);
                G_numberOfChocolateBoxs_Truck_C++;
                break;
            }    
        }

        if (G_numberOfChocolateBoxs_Truck_A == g_number_of_carton_boxs_truck_can_hold_from_typeA && G_numberOfChocolateBoxs_Truck_B == g_number_of_carton_boxs_truck_can_hold_from_typeB && G_numberOfChocolateBoxs_Truck_C == g_number_of_carton_boxs_truck_can_hold_from_typeC){
            if(G_Trucks_Order == 1 )
                G_Trucks_Order = 2;
            else if(G_Trucks_Order == 2 )
                G_Trucks_Order = 3;
            else if(G_Trucks_Order == 3 )
                G_Trucks_Order = 1;   

            G_numberOfChocolateBoxs_Truck_A=0;
            G_numberOfChocolateBoxs_Truck_B=0;
            G_numberOfChocolateBoxs_Truck_C=0;    
       
        }
        pthread_mutex_unlock(&G_mutexs_on_Trucks_Queues[save_index_mutex]);

    
    }
}

}

void insertToStorageArea(){

    struct Object chocolate;
    while (1)
    {       
            pthread_mutex_lock(&G_mutexs_for_FillingTheCartonBoxes_Queues[0]);
            if(G_numberOfChocolateBoxsInTheFillingCartonBoxesQueueTypeA > 0){

                    //Todo update index And Location to ui
                    chocolate=dequeueNodeFromQueueNoInternalMutex(&FrontFillingTheCartonBoxesTypeAQueue, &G_numberOfChocolateBoxsInTheFillingCartonBoxesQueueTypeA);
                    chocolate.current_location=STORAGE_AREA; 
                    enqueueToQueue(chocolate, &G_mutexs_for_StorageArea_Queues[0], &FrontStoragAreaTypeAQueue, &RearStoragAreaTypeAQueue, &G_numberOfChocolateBoxsInStorageAreaTypeA);
            }
            pthread_mutex_unlock(&G_mutexs_for_FillingTheCartonBoxes_Queues[0]);

            pthread_mutex_lock(&G_mutexs_for_FillingTheCartonBoxes_Queues[1]);
            if(G_numberOfChocolateBoxsInTheFillingCartonBoxesQueueTypeB > 0){

                    //Todo update index And Location to ui
                    chocolate=dequeueNodeFromQueueNoInternalMutex(&FrontFillingTheCartonBoxesTypeBQueue, &G_numberOfChocolateBoxsInTheFillingCartonBoxesQueueTypeB);
                    chocolate.current_location=STORAGE_AREA; 
                    enqueueToQueue(chocolate, &G_mutexs_for_StorageArea_Queues[1], &FrontStoragAreaTypeBQueue, &RearStoragAreaTypeBQueue, &G_numberOfChocolateBoxsInStorageAreaTypeB);
            }
            pthread_mutex_unlock(&G_mutexs_for_FillingTheCartonBoxes_Queues[1]);

            pthread_mutex_lock(&G_mutexs_for_FillingTheCartonBoxes_Queues[2]);
            if(G_numberOfChocolateBoxsInTheFillingCartonBoxesQueueTypeC > 0){

                    //Todo update index And Location to ui
                    chocolate=dequeueNodeFromQueueNoInternalMutex(&FrontFillingTheCartonBoxesTypeCQueue, &G_numberOfChocolateBoxsInTheFillingCartonBoxesQueueTypeC);
                    chocolate.current_location=STORAGE_AREA; 
                    enqueueToQueue(chocolate, &G_mutexs_for_StorageArea_Queues[2], &FrontStoragAreaTypeCQueue, &RearStoragAreaTypeCQueue, &G_numberOfChocolateBoxsInStorageAreaTypeC);
            }
            pthread_mutex_unlock(&G_mutexs_for_FillingTheCartonBoxes_Queues[2]);
    }
}

void fillingTheCartonBoxesA(){
    struct Object chocolate1;
    struct Object chocolate2;
    while (1)
    {
            if(G_numberOfchocolatePatchesInContainerTypeA >= 2){

                    //Todo update index And Location to ui and update id
                    chocolate1=dequeueNodeFromQueueWithInternalMutex(&G_mutexs_for_Container_Queues[0], &FrontContainerTypeAQueue, &G_numberOfchocolatePatchesInContainerTypeA);
                    chocolate2=dequeueNodeFromQueueWithInternalMutex(&G_mutexs_for_Container_Queues[0], &FrontContainerTypeAQueue, &G_numberOfchocolatePatchesInContainerTypeA);
                    chocolate1.current_location=CARTON_BOX_A;
                    chocolate1.pkg_type=CARTON_BOX;
                    chocolate2.current_location=CARTON_BOX_A;
                    chocolate2.pkg_type=CARTON_BOX;
                    enqueueToQueue(chocolate1, &G_mutexs_for_FillingTheCartonBoxes_Queues[0], &FrontFillingTheCartonBoxesTypeAQueue, &RearFillingTheCartonBoxesTypeAQueue, &G_numberOfChocolateBoxsInTheFillingCartonBoxesQueueTypeA);
            }
    }
}

void fillingTheCartonBoxesB(){
    struct Object chocolate1;
    struct Object chocolate2;
    while (1)
    {
            if(G_numberOfchocolatePatchesInContainerTypeB >= 2){

                    //Todo update index And Location to ui and update id
                    chocolate1=dequeueNodeFromQueueWithInternalMutex(&G_mutexs_for_Container_Queues[1], &FrontContainerTypeBQueue, &G_numberOfchocolatePatchesInContainerTypeB);
                    chocolate2=dequeueNodeFromQueueWithInternalMutex(&G_mutexs_for_Container_Queues[1], &FrontContainerTypeBQueue, &G_numberOfchocolatePatchesInContainerTypeB);
                    chocolate1.current_location=CARTON_BOX_B;
                    chocolate1.pkg_type=CARTON_BOX;
                    chocolate2.current_location=CARTON_BOX_B;
                    chocolate2.pkg_type=CARTON_BOX;
                    enqueueToQueue(chocolate1, &G_mutexs_for_FillingTheCartonBoxes_Queues[1], &FrontFillingTheCartonBoxesTypeBQueue, &RearFillingTheCartonBoxesTypeBQueue, &G_numberOfChocolateBoxsInTheFillingCartonBoxesQueueTypeB);
            }
    }
}

void fillingTheCartonBoxesC(){
    struct Object chocolate1;
    struct Object chocolate2;
    while (1)
    {
            if(G_numberOfchocolatePatchesInContainerTypeC >= 2){

                    //Todo update index And Location to ui and update id
                    chocolate1=dequeueNodeFromQueueWithInternalMutex(&G_mutexs_for_Container_Queues[2], &FrontContainerTypeCQueue, &G_numberOfchocolatePatchesInContainerTypeC);
                    chocolate2=dequeueNodeFromQueueWithInternalMutex(&G_mutexs_for_Container_Queues[2], &FrontContainerTypeCQueue, &G_numberOfchocolatePatchesInContainerTypeC);
                    chocolate1.current_location=CARTON_BOX_C;
                    chocolate1.pkg_type=CARTON_BOX;
                    chocolate2.current_location=CARTON_BOX_C;
                    chocolate2.pkg_type=CARTON_BOX;
                    enqueueToQueue(chocolate1, &G_mutexs_for_FillingTheCartonBoxes_Queues[2], &FrontFillingTheCartonBoxesTypeCQueue, &RearFillingTheCartonBoxesTypeCQueue, &G_numberOfChocolateBoxsInTheFillingCartonBoxesQueueTypeC);
            }
    }
}

void test_simulation(){

    for(int i = 0;i < 2;i++)
        testPrintingQueue();
    createThreadsForTwoEmpsWorkfromStorageAreaToTruks();
    createThreadsForEmpsWorkfromPrinterToContainers();
    createThreadsForEmpsWorkfromContainersToFillingTheCartonBoxes();
    createThreadsForTwoEmpsWorkfromFillingTheCartonBoxesToStorageArea();
    //creatrThreadForPrint();
    //sleep(5);
    //displyStorageAreaQueueA();
    //displyStorageAreaQueueB();
    //displyStorageAreaQueueC();

}

