

#include "local.h"
#include "utils.h"
#include "globals.c"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>

//.................Functions....................
void initiate_mutexes();
void load_user_defined_values();
void start_simulation();
void create_generator_thread();
void create_employees_threads_type_A();
void create_employees_threads_type_B();
void create_employees_threads_type_C();
void create_patcher_employees();
void create_printer_machine();
void join_all();
int randomIntegerInRange(int lower, int upper);
void employeeDoStepPerLine(void *time_for_emp_takes_to_finsh_step_per_line);
void generator_routine(void *argptr);
void employee_lineA(void *position);
void employee_lineB(void *position);
void employee_lineC(void *position);
void patcher_routine(void *argptr);
void printer_routine(void *argptr);

//..................................new mohammad..................................

struct chocolateNode
{
    message_payload chocolateInfo;
    struct chocolateNode *next;
};

//.........................GLOBALS..............

// Pointers to The printer queue
struct chocolateNode *FrontPrinterQueue = NULL;
struct chocolateNode *RearPrinterQueue = NULL;

// Pointer to The containerTypeA queue
struct chocolateNode *FrontContainerTypeAQueue = NULL;
struct chocolateNode *RearContainerTypeAQueue = NULL;
// Pointer to The containerTypeB queue
struct chocolateNode *FrontContainerTypeBQueue = NULL;
struct chocolateNode *RearContainerTypeBQueue = NULL;
// Pointer to The containerTypeC queue
struct chocolateNode *FrontContainerTypeCQueue = NULL;
struct chocolateNode *RearContainerTypeCQueue = NULL;

// Pointer to The queue of filling the carton boxes: A
struct chocolateNode *FrontFillingTheCartonBoxesTypeAQueue = NULL;
struct chocolateNode *RearFillingTheCartonBoxesTypeAQueue = NULL;
// Pointer to The queue of filling the carton boxes: B
struct chocolateNode *FrontFillingTheCartonBoxesTypeBQueue = NULL;
struct chocolateNode *RearFillingTheCartonBoxesTypeBQueue = NULL;
// Pointer to The queue of filling the carton boxes: C
struct chocolateNode *FrontFillingTheCartonBoxesTypeCQueue = NULL;
struct chocolateNode *RearFillingTheCartonBoxesTypeCQueue = NULL;

// Pointer to The storagAreaTypeA queue
struct chocolateNode *FrontStoragAreaTypeAQueue = NULL;
struct chocolateNode *RearStoragAreaTypeAQueue = NULL;
// Pointer to The storagAreaTypeB queue
struct chocolateNode *FrontStoragAreaTypeBQueue = NULL;
struct chocolateNode *RearStoragAreaTypeBQueue = NULL;
// Pointer to The storagAreaTypeC queue
struct chocolateNode *FrontStoragAreaTypeCQueue = NULL;
struct chocolateNode *RearStoragAreaTypeCQueue = NULL;

// Pointers to trucks queues
struct chocolateNode *FrontTruckQueue1 = NULL;
struct chocolateNode *RearTruckQueue1 = NULL;

struct chocolateNode *FrontTruckQueue2 = NULL;
struct chocolateNode *RearTruckQueue2 = NULL;

struct chocolateNode *FrontTruckQueue3 = NULL;
struct chocolateNode *RearTruckQueue3 = NULL;

// Mutex on printing Queue
pthread_mutex_t printingQueue_mutex = PTHREAD_MUTEX_INITIALIZER;

// Mutex on Container QueueS // index 0:A, 1:B, 2:C
pthread_mutex_t G_mutexs_for_Container_Queues[3] = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER};

// Mutex on THE Filling  Carton Boxes Queues // index 0:A, 1:B, 2:C
pthread_mutex_t G_mutexs_for_FillingTheCartonBoxes_Queues[3] = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER};

// Mutex on Storage Area Queues // index 0:A, 1:B, 2:C
pthread_mutex_t G_mutexs_for_StorageArea_Queues[3] = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER};

// Mutex on Trucks queues // index 0:1, 1:2, 2:3
pthread_mutex_t G_mutexs_on_Trucks_Queues[3] = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER};

int G_numberOfchocolatePatchesInPrintingQueue = 0;

// Number of chocolate products in containers
int G_numberOfchocolatePatchesInContainerTypeA = 0;
int G_numberOfchocolatePatchesInContainerTypeB = 0;
int G_numberOfchocolatePatchesInContainerTypeC = 0;

// Number of chocolate boxs in the  Queue of Filling The Carton Boxes
int G_numberOfChocolateBoxsInTheFillingCartonBoxesQueueTypeA = 0;
int G_numberOfChocolateBoxsInTheFillingCartonBoxesQueueTypeB = 0;
int G_numberOfChocolateBoxsInTheFillingCartonBoxesQueueTypeC = 0;

// Number of chocolate boxs in Storage area
int G_numberOfChocolateBoxsInStorageAreaTypeA = 0;
int G_numberOfChocolateBoxsInStorageAreaTypeB = 0;
int G_numberOfChocolateBoxsInStorageAreaTypeC = 0;

// Number of Boxes in the Trucks
int G_numberOfChocolateBoxs_Truck_A = 0;
int G_numberOfChocolateBoxs_Truck_B = 0;
int G_numberOfChocolateBoxs_Truck_C = 0;

int G_numberOfChocolateBoxsIn_Truck1 = 0;
int G_numberOfChocolateBoxsIn_Truck2 = 0;
int G_numberOfChocolateBoxsIn_Truck3 = 0;

int G_Trucks_Order = 1; // 1:2:3
int G_round = 0;

message_payload dequeueNodeFromQueueNoInternalMutex(struct chocolateNode **FrontQueue, int *numberOfchocolateItemInQueueTypeT);
message_payload dequeueNodeFromQueueWithInternalMutex(pthread_mutex_t *mutex, struct chocolateNode **FrontQueue, int *numberOfchocolateItemInQueueTypeT);
void enqueueToQueue(message_payload chocolateInfo, pthread_mutex_t *mutex, struct chocolateNode **FrontQueue, struct chocolateNode **RearQueue, int *numberOfchocolateItemInQueueTypeT);

void insertToContainers();
void fillingTheCartonBoxesA();
void fillingTheCartonBoxesB();
void fillingTheCartonBoxesC();
void insertToStorageArea();
void insertToTrucks();
void printInfo();

void createThreads();

// Test
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
void testEnqueueToPrintingQueue();

// .................................end new........................................

int main()
{

    // load_user_defined_values();

    create_and_setup_message_queues();

    initiate_mutexes();

    run_gui();

    // register signal handler for SIGINT to clean up
    signal(SIGINT, interrupt_sig_handler);

    start_simulation();

    join_all();

    clean_up();
}

void initiate_mutexes()
{
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < PILESIZE; j++)
        {
            if (pthread_mutex_init(&A_pile_mutex[i][j], NULL) == -1)
            {
                perror("Pile A mutex : ");
                exit(1);
            }
        }
    }

    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < PILESIZE; j++)
        {
            if (pthread_mutex_init(&B_pile_mutex[i][j], NULL) == -1)
            {
                perror("Pile B mutex : ");
                exit(1);
            }
        }
    }

    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < PILESIZE; j++)
        {
            if (pthread_mutex_init(&C_pile_mutex[i][j], NULL) == -1)
            {
                perror("Pile C mutex : ");
                exit(1);
            }
        }
    }

    if (pthread_mutex_init(&patch_mutex_A, NULL) == -1)
    {
        perror("patcher A mutex :");
        exit(1);
    }

    if (pthread_mutex_init(&patch_mutex_B, NULL) == -1)
    {
        perror("patcher B mutex :");
        exit(1);
    }

    if (pthread_mutex_init(&patch_mutex_C, NULL) == -1)
    {
        perror("patcher C mutex :");
        exit(1);
    }

    if (pthread_mutex_init(&id_mutex, NULL) == -1)
    {
        perror("id mutex :");
        exit(1);
    }
}

void start_simulation()
{
    create_generator_thread();
    create_employees_threads_type_A();
    create_employees_threads_type_B();
    create_employees_threads_type_C();
    create_patcher_employees();
    create_printer_machine();

    for (int i = 0; i < 4; i++)
        testEnqueueToPrintingQueue();
    createThreads();

    start_flag = 1;
}

void create_generator_thread()
{
    if (pthread_create(&generator, NULL, (void *)generator_routine, NULL) == -1)
    {
        perror("generator thread creation: ");
        exit(1);
    }
}

void create_employees_threads_type_A()
{
    int i = 0, j = 0;
    employee_information position[C_MANUFACTURING_LINES_TYPEA][C_EMPLOYEES_PER_LINE_IN_MANUFACTURING_PROCESS_TYPE_A];
    for (j = 0; j < C_MANUFACTURING_LINES_TYPEA; j++)
    {
        for (i = 0; i < C_EMPLOYEES_PER_LINE_IN_MANUFACTURING_PROCESS_TYPE_A; i++)
        {
            position[j][i].linenum = j;
            position[j][i].index = i;
            if (pthread_create(&g_Array_of_Threads_TypeA[j][i], NULL, (void *)employee_lineA, (void *)&position[j][i]) == -1)
            {
                perror("Employee A, thread creation : ");
                exit(1);
            }
            usleep(50000);
        }
    }
}

void create_employees_threads_type_B()
{
    int i = 0, j = 0;
    employee_information position[C_MANUFACTURING_LINES_TYPEB][C_EMPLOYEES_PER_LINE_IN_MANUFACTURING_PROCESS_TYPE_B];
    for (j = 0; j < C_MANUFACTURING_LINES_TYPEB; j++)
    {
        for (i = 0; i < C_EMPLOYEES_PER_LINE_IN_MANUFACTURING_PROCESS_TYPE_B; i++)
        {
            position[j][i].linenum = j;
            position[j][i].index = i;
            if (pthread_create(&g_Array_of_Threads_TypeB[j][i], NULL, (void *)employee_lineB, (void *)&position[j][i]) == -1)
            {
                perror("Employee B, thread creation : ");
                exit(1);
            }
            usleep(50000);
        }
    }
}

void create_employees_threads_type_C()
{
    int i = 0, j = 0;
    employee_information position[C_MANUFACTURING_LINES_TYPEC][C_EMPLOYEES_PER_LINE_IN_MANUFACTURING_PROCESS_TYPE_C];
    for (j = 0; j < C_MANUFACTURING_LINES_TYPEC; j++)
    {
        for (i = 0; i < C_EMPLOYEES_PER_LINE_IN_MANUFACTURING_PROCESS_TYPE_C; i++)
        {
            position[j][i].linenum = j;
            position[j][i].index = i;
            if (pthread_create(&g_Array_of_Threads_TypeC[j][i], NULL, (void *)employee_lineC, (void *)&position[j][i]) == -1)
            {
                perror("Employee C, thread creation : ");
                exit(1);
            }
            usleep(50000);
        }
    }
}

void create_patcher_employees()
{

    for (int i = 0; i < 2; i++)
    {
        if (pthread_create(&patcher_employees[i], NULL, (void *)patcher_routine, NULL) == -1)
        {
            perror("Patcher Employee , thread creation : ");
            exit(1);
        }
        usleep(50000);
    }
}

void create_printer_machine()
{
    if (pthread_create(&printer, NULL, (void *)printer_routine, NULL) == -1)
    {
        perror("Patcher Employee , thread creation : ");
        exit(1);
    }
    usleep(50000);
}

void generator_routine(void *argptr)
{
    while (start_flag == 0)
        ;
    int empty_index, j, i;

    // initialize piles
    for (i = 0; i < C_MANUFACTURING_LINES_TYPEA; i++)
    {
        for (j = 0; j < PILESIZE; j++)
        {
            type_A_pile[i][j].id = 0;
        }
    }
    for (i = 0; i < C_MANUFACTURING_LINES_TYPEB; i++)
    {
        for (j = 0; j < PILESIZE; j++)
        {
            type_B_pile[i][j].id = 0;
        }
    }
    for (i = 0; i < C_MANUFACTURING_LINES_TYPEC; i++)
    {
        for (j = 0; j < PILESIZE; j++)
        {
            type_C_pile[i][j].id = 0;
        }
    }

    for (i = 0; i < 100; i)
    { // temporary because of lack of termination condition

        for (j = 0; j < C_MANUFACTURING_LINES_TYPEA; j++)
        {
            for (int k = 0; k <= PILESIZE; k++)
            {
                if (pthread_mutex_lock(&A_pile_mutex[j][k]) == -1)
                {
                    perror("mutex lock pile A :");
                    exit(1);
                }
                if (type_A_pile[j][k].id == 0)
                {
                    generate_product(k, 'a', j);
                }
                if (pthread_mutex_unlock(&A_pile_mutex[j][k]) == -1)
                {
                    perror("mutex unlock pile A :");
                    exit(1);
                }
            }
        }
        for (j = 0; j < C_MANUFACTURING_LINES_TYPEB; j++)
        {
            for (int k = 0; k <= PILESIZE; k++)
            {
                if (pthread_mutex_lock(&B_pile_mutex[j][k]) == -1)
                {
                    perror("mutex lock pile B :");
                    exit(1);
                }
                if (type_B_pile[j][k].id == 0)
                {
                    generate_product(k, 'b', j);
                    count++;
                }
                if (pthread_mutex_unlock(&B_pile_mutex[j][k]) == -1)
                {
                    perror("mutex unlock pile B :");
                    exit(1);
                }
            }
        }
        // printf("exited\n");
        if (count > 50)
            break;

        for (j = 0; j < C_MANUFACTURING_LINES_TYPEC; j++)
        {
            for (int k = 0; k <= PILESIZE; k++)
            {
                if (pthread_mutex_lock(&C_pile_mutex[j][k]) == -1)
                {
                    perror("mutex lock pile C :");
                    exit(1);
                }
                if (type_C_pile[j][k].id == 0)
                {
                    generate_product(k, 'c', j);
                }
                if (pthread_mutex_unlock(&C_pile_mutex[j][k]) == -1)
                {
                    perror("mutex unlock pile C :");
                    exit(1);
                }
            }
        }
    }
}

void employee_lineA(void *position)
{
    employee_information *temp = (employee_information *)position;
    int linenum = temp->linenum;
    int index = temp->index;
    int f = 0;
    message_buf buf;
    buf.mtype = 1;
    buf.payload.chocolate_type = TYPE_A;
    buf.payload.msg_type = 1;
    buf.payload.item_type = PRODUCT;
    while (start_flag == 0)
        ;
    if (index > 7 || linenum > 2)
    {
        perror("error with passing argument");
        clean_up();
        exit(1);
    }
    // printf("index : %d, linenum : %d\n", index, linenum);

    switch (linenum)
    {
    case 0:
        buf.payload.current_location = MANUFACTURING_LINE_A1;
        break;
    case 1:
        buf.payload.current_location = MANUFACTURING_LINE_A2;
        break;
    }
    buf.payload.index = index;

    int i = 0, j = 0;
    srand(g_Array_of_Threads_TypeA[linenum][index]);
    int step_time = (C_MIN_A + rand()) % C_MAX_A;
    while (1)
    {

        i = find_product(type_A_pile[linenum], i);
        if (i != PILESIZE)
        {
            if (pthread_mutex_trylock(&A_pile_mutex[linenum][i]) == 0)
            {
                if (type_A_pile[linenum][i].progress[index] == '0' && (index == 0 || type_A_pile[linenum][i].progress[index - 1] == '1' || (index > 3 && type_A_pile[linenum][i].progress[3] == '1')))
                {
                    type_A_pile[linenum][i].progress[index] = '1';
                    msgsnd(ui_msgq_id, &buf, sizeof(buf), 0);
                    usleep(step_time);
                    if (strcmp(type_A_pile[linenum][i].progress, "11111111") == 0)
                    {
                        buf.payload.id = type_A_pile[linenum][i].id;
                        msgsnd(patcher_msgq_id, &buf, sizeof(buf), 0);
                        type_A_pile[linenum][i].id = 0;
                        f++;
                    }
                }
                pthread_mutex_unlock(&A_pile_mutex[linenum][i]);
            }
        }
        i = (i + 1) % PILESIZE;
        j++;
        if (count >= 50)
        {
            break;
        }
    }
    if (f != 0)
        printf("A %d: %d items passed through this line\n", index, f);
}

void employee_lineB(void *position)
{
    employee_information *temp = (employee_information *)position;
    int linenum = temp->linenum;
    int index = temp->index;
    int f = 0;
    message_buf buf;
    buf.mtype = 1;
    buf.payload.chocolate_type = TYPE_B;
    buf.payload.msg_type = 1;
    buf.payload.item_type = PRODUCT;
    while (start_flag == 0)
        ;
    if (index > 5 || linenum > 1)
    {
        perror("error with passing argument");
        clean_up();
        exit(1);
    }

    switch (linenum)
    {
    case 0:
        buf.payload.current_location = MANUFACTURING_LINE_B1;
        break;
    case 1:
        buf.payload.current_location = MANUFACTURING_LINE_B2;
        break;
    }
    buf.payload.index = index;

    int i = 0, j = 0;
    srand(g_Array_of_Threads_TypeB[linenum][index]);
    int step_time = (C_MIN_B + rand()) % C_MAX_B;
    while (1)
    {

        i = find_product(type_B_pile[linenum], i);
        if (i != PILESIZE)
        {
            if (pthread_mutex_trylock(&B_pile_mutex[linenum][i]) == 0)
            {

                if (type_B_pile[linenum][i].progress[index] == '0' && (index == 0 || type_B_pile[linenum][i].progress[index - 1] == '1'))
                {
                    type_B_pile[linenum][i].progress[index] = '1';
                    msgsnd(ui_msgq_id, &buf, sizeof(buf), 0);
                    usleep(step_time);
                    if (strcmp(type_B_pile[linenum][i].progress, "11111100") == 0)
                    {
                        buf.payload.id = type_B_pile[linenum][i].id;
                        msgsnd(patcher_msgq_id, &buf, sizeof(buf), 0);
                        type_B_pile[linenum][i].id = 0;
                        f++;
                    }
                }
                pthread_mutex_unlock(&B_pile_mutex[linenum][i]);
            }
        }
        i = (i + 1) % PILESIZE;
        j++;
        if (count >= 50)
        {
            break;
        }
    }
    if (f != 0)
        printf("B %d: %d items passed through this line\n", index, f);
}

void employee_lineC(void *position)
{
    employee_information *temp = (employee_information *)position;
    int linenum = temp->linenum;
    int index = temp->index;
    int f = 0;
    message_buf buf;
    buf.mtype = 1;
    buf.payload.chocolate_type = TYPE_C;
    buf.payload.msg_type = 1;
    buf.payload.item_type = PRODUCT;
    while (start_flag == 0)
        ;
    if (index > 5 || linenum > 1)
    {
        perror("error with passing argument");
        clean_up();
        exit(1);
    }

    switch (linenum)
    {
    case 0:
        buf.payload.current_location = MANUFACTURING_LINE_C1;
        break;
    case 1:
        buf.payload.current_location = MANUFACTURING_LINE_C2;
        break;
    }
    buf.payload.index = index;

    int i = 0, j = 0;
    srand(g_Array_of_Threads_TypeC[linenum][index]);
    int step_time = (C_MIN_C + rand()) % C_MAX_C;
    while (1)
    {

        i = find_product(type_C_pile[linenum], i);
        if (i != PILESIZE)
        {
            if (pthread_mutex_trylock(&C_pile_mutex[linenum][i]) == 0)
            {
                if (type_C_pile[linenum][i].progress[index] == '0' && (index == 0 || type_C_pile[linenum][i].progress[index - 1] == '1' || (index > 3 && type_C_pile[linenum][i].progress[3] == '1')))
                {
                    msgsnd(ui_msgq_id, &buf, sizeof(buf), 0);
                    type_C_pile[linenum][i].progress[index] = '1';
                    usleep(step_time);
                    if (strcmp(type_C_pile[linenum][i].progress, "11111000") == 0)
                    {
                        buf.payload.id = type_C_pile[linenum][i].id;
                        msgsnd(patcher_msgq_id, &buf, sizeof(buf), 0);
                        type_C_pile[linenum][i].id = 0;
                        f++;
                    }
                }
                pthread_mutex_unlock(&C_pile_mutex[linenum][i]);
            }
        }
        i = (i + 1) % PILESIZE;
        j++;
        if (count >= 50)
        {
            break;
        }
    }
    if (f != 0)
        printf("C %d: %d items passed through this line\n", index, f);
}

void patcher_routine(void *argptr)
{
    while (start_flag == 0)
        ;
    int j = 0;
    int o = 0;
    message_buf buf;
    message_buf send;
    send.mtype = 1;
    send.payload.item_type = PATCH;

    while (1)
    {
        if (msgrcv(patcher_msgq_id, &buf, sizeof(buf), 1, IPC_NOWAIT) == -1)
        {
        }
        else
        {
            buf.mtype = 1;
            switch (buf.payload.chocolate_type)
            {
            case TYPE_A:
                buf.payload.current_location = PATCHING_A;
                break;
            case TYPE_B:
                buf.payload.current_location = PATCHING_B;
                break;
            case TYPE_C:
                buf.payload.current_location = PATCHING_C;
                break;
            }
            send.payload.current_location = buf.payload.current_location;
            msgsnd(ui_msgq_id, &buf, sizeof(buf), 0);

            if (buf.payload.chocolate_type == TYPE_A)
            {
                pthread_mutex_lock(&patch_mutex_A);
                arr_A[type_A_patch] = buf.payload.id;
                type_A_patch++;
                if (type_A_patch == 10)
                {
                    type_A_patch = 0;
                    send.payload.chocolate_type = TYPE_A;
                    buf.payload.item_type = PATCH;
                    buf.payload.id = generate_uniq_id();
                    for (o = 0; o < 9; o++)
                        buf.payload.ids_to_delete[o] = arr_A[o];
                    msgsnd(ui_msgq_id, &buf, sizeof(buf), 0);
                    msgsnd(printer_msgq_id, &send, sizeof(send), 0);
                }
                usleep(PATCHER_TIME);
                pthread_mutex_unlock(&patch_mutex_A);
            }
            else if (buf.payload.chocolate_type == TYPE_B)
            {
                pthread_mutex_lock(&patch_mutex_B);
                type_B_patch++;
                arr_B[type_B_patch] = buf.payload.id;
                if (type_B_patch == 10)
                {
                    type_B_patch = 0;
                    send.payload.chocolate_type = TYPE_B;
                    buf.payload.item_type = PATCH;
                    buf.payload.id = generate_uniq_id();
                    for (o = 0; o < 9; o++)
                        buf.payload.ids_to_delete[o] = arr_B[o];
                    msgsnd(ui_msgq_id, &buf, sizeof(buf), 0);
                    msgsnd(printer_msgq_id, &send, sizeof(send), 0);
                }
                usleep(PATCHER_TIME);
                pthread_mutex_unlock(&patch_mutex_B);
            }
            else if (buf.payload.chocolate_type == TYPE_C)
            {
                pthread_mutex_lock(&patch_mutex_C);
                type_C_patch++;
                arr_C[type_C_patch] = buf.payload.id;
                if (type_C_patch == 10)
                {
                    type_C_patch = 0;
                    send.payload.chocolate_type = TYPE_C;
                    buf.payload.item_type = PATCH;
                    buf.payload.id = generate_uniq_id();
                    for (o = 0; o < 9; o++)
                        buf.payload.ids_to_delete[o] = arr_C[o];
                    msgsnd(ui_msgq_id, &buf, sizeof(buf), 0);
                    msgsnd(printer_msgq_id, &send, sizeof(send), 0);
                }
                usleep(PATCHER_TIME);
                pthread_mutex_unlock(&patch_mutex_C);
            }
        }
        j++;
        if (j == 1000) // termination condition
            break;
    }
    pthread_exit(0);
}

void printer_routine(void *argptr)
{
    while (start_flag == 0)
        ;
    int j = 0;
    message_buf buf;
    while (1)
    {
        if (msgrcv(printer_msgq_id, &buf, sizeof(buf), 1, IPC_NOWAIT) == -1)
        {
        }
        else
        {
            printf("printer received msg\n");
            buf.payload.current_location = PRINTER;
            msgsnd(ui_msgq_id, &buf, sizeof(buf), 0);
            for (int i = 0; i < 10; i++)
            {
                // print date on patche node
                usleep(PRINTER_TIME);
            }
            // append to patche queue
        }
        j++;
        if (j == 1000) // termination condition needed
            break;
    }
}

void join_all()
{
    int i, j;

    pthread_join(generator, NULL);

    for (j = 0; j < C_MANUFACTURING_LINES_TYPEA; j++)
    {
        for (i = 0; i < C_EMPLOYEES_PER_LINE_IN_MANUFACTURING_PROCESS_TYPE_A; i++)
        {
            pthread_join(g_Array_of_Threads_TypeA[j][i], NULL);
        }
    }

    for (j = 0; j < C_MANUFACTURING_LINES_TYPEB; j++)
    {
        for (i = 0; i < C_EMPLOYEES_PER_LINE_IN_MANUFACTURING_PROCESS_TYPE_B; i++)
        {
            pthread_join(g_Array_of_Threads_TypeB[j][i], NULL);
        }
    }

    for (j = 0; j < C_MANUFACTURING_LINES_TYPEC; j++)
    {
        for (i = 0; i < C_EMPLOYEES_PER_LINE_IN_MANUFACTURING_PROCESS_TYPE_C; i++)
        {
            pthread_join(g_Array_of_Threads_TypeC[j][i], NULL);
        }
    }

    for (i = 0; i < 2; i++)
    {
        pthread_join(patcher_employees[i], NULL);
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
    {
        // perror("load_user_defined_values:");
        exit(EXIT_FAILURE);
    }
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

void enqueueToQueue(message_payload chocolateInfo, pthread_mutex_t *mutex, struct chocolateNode **FrontQueue, struct chocolateNode **RearQueue, int *numberOfchocolateItemInQueueTypeT)
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
        // Todo update index to ui
        ptr->chocolateInfo.chocolate_type = chocolateInfo.chocolate_type;

        if ((*FrontQueue) == NULL)
        {
            (*FrontQueue) = ptr;
            (*RearQueue) = ptr;
            (*FrontQueue)->next = NULL;
            (*RearQueue)->next = NULL;
        }
        else
        {
            (*RearQueue)->next = ptr;
            (*RearQueue) = ptr;
            (*RearQueue)->next = NULL;
        }
        // sleep
        (*numberOfchocolateItemInQueueTypeT)++;
    }
    pthread_mutex_unlock(mutex);
}

message_payload dequeueNodeFromQueueWithInternalMutex(pthread_mutex_t *mutex, struct chocolateNode **FrontQueue, int *numberOfchocolateItemInQueueTypeT)
{

    pthread_mutex_lock(mutex);
    struct chocolateNode *temp = NULL;
    message_payload chocolate = {0};
    // Todo update index to ui
    if ((*FrontQueue) == NULL)
    {
        printf("Underflow dequeueNodeFromQueueWithInternalMutex\n");
        return chocolate;
    }
    else
    {
        temp = (*FrontQueue);
        chocolate = temp->chocolateInfo;
        // Todo update index to ui
        (*FrontQueue) = (*FrontQueue)->next;
        free(temp);
        (*numberOfchocolateItemInQueueTypeT)--;
    }
    pthread_mutex_unlock(mutex);
    return chocolate;
}
message_payload dequeueNodeFromQueueNoInternalMutex(struct chocolateNode **FrontQueue, int *numberOfchocolateItemInQueueTypeT)
{
    struct chocolateNode *temp = NULL;
    message_payload chocolate = {0};
    // Todo update index to ui
    if ((*FrontQueue) == NULL)
    {
        printf("Underflow dequeueNodeFromQueue\n");
        return chocolate;
    }
    else
    {
        temp = (*FrontQueue);
        chocolate = temp->chocolateInfo;
        // Todo update index to ui
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
            printf("\n\nlocation:  %d  type: %d   id:%d\n\n", temp->chocolateInfo.current_location, temp->chocolateInfo.chocolate_type, temp->chocolateInfo.id);
            temp = temp->next;
        }
    }
}
void displyStorageAreaQueueA()
{

    struct chocolateNode *temp = NULL;
    if ((FrontStoragAreaTypeAQueue == NULL))
    {
        printf("\nStorage Area Queue A is Empty\n");
    }
    else
    {
        printf("\nStorage Area Queue A is :\n");
        temp = FrontStoragAreaTypeAQueue;
        while (temp)
        {
            printf("\nlocation in Storage Area Queue A:  %d  type: %d   id:%d\n", temp->chocolateInfo.current_location, temp->chocolateInfo.chocolate_type, temp->chocolateInfo.id);
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
            printf("\n\nlocation in Storage Area Queue B:  %d  type: %d   id:%d\n\n", temp->chocolateInfo.current_location, temp->chocolateInfo.chocolate_type, temp->chocolateInfo.id);
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
            printf("\n\nlocation in Storage Area Queue C:  %d  type: %d   id:%d\n\n", temp->chocolateInfo.current_location, temp->chocolateInfo.chocolate_type, temp->chocolateInfo.id);
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
            printf("\n\nlocation:  %d  type: %d   id:%d\n\n", temp->chocolateInfo.current_location, temp->chocolateInfo.chocolate_type, temp->chocolateInfo.id);
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
            printf("\n\nlocation:  %d  type: %d   id:%d\n\n", temp->chocolateInfo.current_location, temp->chocolateInfo.chocolate_type, temp->chocolateInfo.id);
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
            printf("\n\nlocation:  %d  type: %d   id:%d\n\n", temp->chocolateInfo.current_location, temp->chocolateInfo.chocolate_type, temp->chocolateInfo.id);
            temp = temp->next;
        }
    }
}

void displyTheFillingCartonBoxesQueueA()
{

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
            printf("\n\nlocation:  %d  type: %d   id:%d\n\n", temp->chocolateInfo.current_location, temp->chocolateInfo.chocolate_type, temp->chocolateInfo.id);
            temp = temp->next;
        }
    }
}
void displyTheFillingCartonBoxesQueueB()
{

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
            printf("\n\nlocation:  %d  type: %d   id:%d\n\n", temp->chocolateInfo.current_location, temp->chocolateInfo.chocolate_type, temp->chocolateInfo.id);
            temp = temp->next;
        }
    }
}
void displyTheFillingCartonBoxesQueueC()
{

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
            printf("\n\nlocation:  %d  type: %d   id:%d\n\n", temp->chocolateInfo.current_location, temp->chocolateInfo.chocolate_type, temp->chocolateInfo.id);
            temp = temp->next;
        }
    }
}
void testEnqueueToPrintingQueue()
{
    message_payload ch1;
    ch1.chocolate_type = TYPE_A;
    ch1.current_location = PRINTER;
    ch1.item_type = PATCH;
    ch1.index_in_queue = 0;
    ch1.id = 1;
    enqueueToQueue(ch1, &printingQueue_mutex, &FrontPrinterQueue, &RearPrinterQueue, &G_numberOfchocolatePatchesInPrintingQueue);
    message_payload ch2;
    ch2.chocolate_type = TYPE_B;
    ch2.current_location = PRINTER;
    ch2.item_type = PATCH;
    ch2.index_in_queue = 0;
    ch2.id = 2;
    enqueueToQueue(ch2, &printingQueue_mutex, &FrontPrinterQueue, &RearPrinterQueue, &G_numberOfchocolatePatchesInPrintingQueue);
    message_payload ch3;
    ch3.chocolate_type = TYPE_C;
    ch3.current_location = PRINTER;
    ch3.item_type = PATCH;
    ch3.index_in_queue = 0;
    ch3.id = 3;
    enqueueToQueue(ch3, &printingQueue_mutex, &FrontPrinterQueue, &RearPrinterQueue, &G_numberOfchocolatePatchesInPrintingQueue);
    message_payload ch5;
    ch5.chocolate_type = TYPE_A;
    ch5.current_location = PRINTER;
    ch5.item_type = PATCH;
    ch5.index_in_queue = 0;
    ch5.id = 4;
    enqueueToQueue(ch5, &printingQueue_mutex, &FrontPrinterQueue, &RearPrinterQueue, &G_numberOfchocolatePatchesInPrintingQueue);
    message_payload ch4;
    ch4.chocolate_type = TYPE_B;
    ch4.current_location = PRINTER;
    ch4.item_type = PATCH;
    ch4.index_in_queue = 0;
    ch4.id = 5;
    enqueueToQueue(ch4, &printingQueue_mutex, &FrontPrinterQueue, &RearPrinterQueue, &G_numberOfchocolatePatchesInPrintingQueue);
    message_payload ch6;
    ch6.chocolate_type = TYPE_C;
    ch6.current_location = PRINTER;
    ch6.item_type = PATCH;
    ch6.index_in_queue = 0;
    ch6.id = 6;
    enqueueToQueue(ch6, &printingQueue_mutex, &FrontPrinterQueue, &RearPrinterQueue, &G_numberOfchocolatePatchesInPrintingQueue);
}

void insertToContainers()
{
    message_payload chocolate;
    while (1)
    {
        pthread_mutex_lock(&printingQueue_mutex);
        if (G_numberOfchocolatePatchesInPrintingQueue > 0)
        {
            chocolate = dequeueNodeFromQueueNoInternalMutex(&FrontPrinterQueue, &G_numberOfchocolatePatchesInPrintingQueue);
            // Todo update index to ui
            if (chocolate.chocolate_type == TYPE_A)
            {
                chocolate.current_location = CONTAINER_A;
                enqueueToQueue(chocolate, &G_mutexs_for_Container_Queues[0], &FrontContainerTypeAQueue, &RearContainerTypeAQueue, &G_numberOfchocolatePatchesInContainerTypeA);
            }
            else if (chocolate.chocolate_type == TYPE_B)
            {
                chocolate.current_location = CONTAINER_B;
                enqueueToQueue(chocolate, &G_mutexs_for_Container_Queues[1], &FrontContainerTypeBQueue, &RearContainerTypeBQueue, &G_numberOfchocolatePatchesInContainerTypeB);
            }
            else if (chocolate.chocolate_type == TYPE_C)
            {
                chocolate.current_location = CONTAINER_C;
                enqueueToQueue(chocolate, &G_mutexs_for_Container_Queues[2], &FrontContainerTypeCQueue, &RearContainerTypeCQueue, &G_numberOfchocolatePatchesInContainerTypeC);
            }
        }
        pthread_mutex_unlock(&printingQueue_mutex);
    }
}

void insertToTrucks()
{

    message_payload chocolate;
    int save_index_mutex;

    while (1)
    {
        while (1)
        {
            pthread_mutex_lock(&G_mutexs_on_Trucks_Queues[G_Trucks_Order - 1]);
            save_index_mutex = G_Trucks_Order - 1;
            if (G_numberOfChocolateBoxs_Truck_A < g_number_of_carton_boxs_truck_can_hold_from_typeA)
            {
                if (G_numberOfChocolateBoxsInStorageAreaTypeA > 0)
                {
                    chocolate = dequeueNodeFromQueueWithInternalMutex(&G_mutexs_for_StorageArea_Queues[0], &FrontStoragAreaTypeAQueue, &G_numberOfChocolateBoxsInStorageAreaTypeA);
                    pthread_mutex_unlock(&G_mutexs_on_Trucks_Queues[save_index_mutex]);
                    if (save_index_mutex == 0)
                    {
                        enqueueToQueue(chocolate, &G_mutexs_on_Trucks_Queues[save_index_mutex], &FrontTruckQueue1, &RearTruckQueue1, &G_numberOfChocolateBoxsIn_Truck1);
                    }
                    else if (save_index_mutex == 1)
                    {
                        enqueueToQueue(chocolate, &G_mutexs_on_Trucks_Queues[save_index_mutex], &FrontTruckQueue2, &RearTruckQueue2, &G_numberOfChocolateBoxsIn_Truck2);
                    }
                    else if (save_index_mutex == 2)
                    {
                        enqueueToQueue(chocolate, &G_mutexs_on_Trucks_Queues[save_index_mutex], &FrontTruckQueue3, &RearTruckQueue3, &G_numberOfChocolateBoxsIn_Truck3);
                    }

                    G_numberOfChocolateBoxs_Truck_A++;
                    printf("\n\nG_numberOfChocolateBoxs_Truck_A:%d   Truck :%d           G_round:%d\n\n", G_numberOfChocolateBoxs_Truck_A, save_index_mutex + 1, G_round);
                    break;
                }
            }
            if (G_numberOfChocolateBoxs_Truck_B < g_number_of_carton_boxs_truck_can_hold_from_typeB)
            {
                if (G_numberOfChocolateBoxsInStorageAreaTypeB > 0)
                {
                    chocolate = dequeueNodeFromQueueWithInternalMutex(&G_mutexs_for_StorageArea_Queues[1], &FrontStoragAreaTypeBQueue, &G_numberOfChocolateBoxsInStorageAreaTypeB);
                    pthread_mutex_unlock(&G_mutexs_on_Trucks_Queues[save_index_mutex]);
                    if (save_index_mutex == 0)
                    {
                        enqueueToQueue(chocolate, &G_mutexs_on_Trucks_Queues[save_index_mutex], &FrontTruckQueue1, &RearTruckQueue1, &G_numberOfChocolateBoxsIn_Truck1);
                    }
                    else if (save_index_mutex == 1)
                    {
                        enqueueToQueue(chocolate, &G_mutexs_on_Trucks_Queues[save_index_mutex], &FrontTruckQueue2, &RearTruckQueue2, &G_numberOfChocolateBoxsIn_Truck2);
                    }
                    else if (save_index_mutex == 2)
                    {
                        enqueueToQueue(chocolate, &G_mutexs_on_Trucks_Queues[save_index_mutex], &FrontTruckQueue3, &RearTruckQueue3, &G_numberOfChocolateBoxsIn_Truck3);
                    }
                    G_numberOfChocolateBoxs_Truck_B++;
                    printf("\n\nG_numberOfChocolateBoxs_Truck_B:%d   Truck :%d           G_round:%d\n\n", G_numberOfChocolateBoxs_Truck_B, save_index_mutex + 1, G_round);
                    break;
                }
            }
            if (G_numberOfChocolateBoxs_Truck_C < g_number_of_carton_boxs_truck_can_hold_from_typeC)
            {
                if (G_numberOfChocolateBoxsInStorageAreaTypeC > 0)
                {
                    chocolate = dequeueNodeFromQueueWithInternalMutex(&G_mutexs_for_StorageArea_Queues[2], &FrontStoragAreaTypeCQueue, &G_numberOfChocolateBoxsInStorageAreaTypeC);
                    pthread_mutex_unlock(&G_mutexs_on_Trucks_Queues[save_index_mutex]);
                    if (save_index_mutex == 0)
                    {
                        enqueueToQueue(chocolate, &G_mutexs_on_Trucks_Queues[save_index_mutex], &FrontTruckQueue1, &RearTruckQueue1, &G_numberOfChocolateBoxsIn_Truck1);
                    }
                    else if (save_index_mutex == 1)
                    {
                        enqueueToQueue(chocolate, &G_mutexs_on_Trucks_Queues[save_index_mutex], &FrontTruckQueue2, &RearTruckQueue2, &G_numberOfChocolateBoxsIn_Truck2);
                    }
                    else if (save_index_mutex == 2)
                    {
                        enqueueToQueue(chocolate, &G_mutexs_on_Trucks_Queues[save_index_mutex], &FrontTruckQueue3, &RearTruckQueue3, &G_numberOfChocolateBoxsIn_Truck3);
                    }
                    G_numberOfChocolateBoxs_Truck_C++;
                    printf("\n\nG_numberOfChocolateBoxs_Truck_C:%d   Truck :%d           G_round:%d\n\n", G_numberOfChocolateBoxs_Truck_C, save_index_mutex + 1, G_round);
                    break;
                }
            }

            if (G_numberOfChocolateBoxs_Truck_A == g_number_of_carton_boxs_truck_can_hold_from_typeA && G_numberOfChocolateBoxs_Truck_B == g_number_of_carton_boxs_truck_can_hold_from_typeB && G_numberOfChocolateBoxs_Truck_C == g_number_of_carton_boxs_truck_can_hold_from_typeC)
            {
                if (G_Trucks_Order == 1)
                    G_Trucks_Order = 2;
                else if (G_Trucks_Order == 2)
                    G_Trucks_Order = 3;
                else if (G_Trucks_Order == 3)
                {
                    G_Trucks_Order = 1;
                    G_round++;
                }

                G_numberOfChocolateBoxs_Truck_A = 0;
                G_numberOfChocolateBoxs_Truck_B = 0;
                G_numberOfChocolateBoxs_Truck_C = 0;
            }
            pthread_mutex_unlock(&G_mutexs_on_Trucks_Queues[save_index_mutex]);
        }
    }
}
void printInfo()
{

    while (1)
    {

        displyPrintingQueue();
        displyContainerQueueA();
        displyContainerQueueB();
        displyContainerQueueC();
        displyTheFillingCartonBoxesQueueA();
        displyTheFillingCartonBoxesQueueB();
        displyTheFillingCartonBoxesQueueC();
        displyStorageAreaQueueA();
        displyStorageAreaQueueB();
        displyStorageAreaQueueC();
        sleep(1);
    }
}

void insertToStorageArea()
{

    message_payload chocolate;
    while (1)
    {
        pthread_mutex_lock(&G_mutexs_for_FillingTheCartonBoxes_Queues[0]);
        if (G_numberOfChocolateBoxsInTheFillingCartonBoxesQueueTypeA > 0)
        {

            // Todo update index And Location to ui
            chocolate = dequeueNodeFromQueueNoInternalMutex(&FrontFillingTheCartonBoxesTypeAQueue, &G_numberOfChocolateBoxsInTheFillingCartonBoxesQueueTypeA);
            chocolate.current_location = STORAGE_AREA;
            enqueueToQueue(chocolate, &G_mutexs_for_StorageArea_Queues[0], &FrontStoragAreaTypeAQueue, &RearStoragAreaTypeAQueue, &G_numberOfChocolateBoxsInStorageAreaTypeA);
        }
        pthread_mutex_unlock(&G_mutexs_for_FillingTheCartonBoxes_Queues[0]);

        pthread_mutex_lock(&G_mutexs_for_FillingTheCartonBoxes_Queues[1]);
        if (G_numberOfChocolateBoxsInTheFillingCartonBoxesQueueTypeB > 0)
        {

            // Todo update index And Location to ui
            chocolate = dequeueNodeFromQueueNoInternalMutex(&FrontFillingTheCartonBoxesTypeBQueue, &G_numberOfChocolateBoxsInTheFillingCartonBoxesQueueTypeB);
            chocolate.current_location = STORAGE_AREA;
            enqueueToQueue(chocolate, &G_mutexs_for_StorageArea_Queues[1], &FrontStoragAreaTypeBQueue, &RearStoragAreaTypeBQueue, &G_numberOfChocolateBoxsInStorageAreaTypeB);
        }
        pthread_mutex_unlock(&G_mutexs_for_FillingTheCartonBoxes_Queues[1]);

        pthread_mutex_lock(&G_mutexs_for_FillingTheCartonBoxes_Queues[2]);
        if (G_numberOfChocolateBoxsInTheFillingCartonBoxesQueueTypeC > 0)
        {

            // Todo update index And Location to ui
            chocolate = dequeueNodeFromQueueNoInternalMutex(&FrontFillingTheCartonBoxesTypeCQueue, &G_numberOfChocolateBoxsInTheFillingCartonBoxesQueueTypeC);
            chocolate.current_location = STORAGE_AREA;
            enqueueToQueue(chocolate, &G_mutexs_for_StorageArea_Queues[2], &FrontStoragAreaTypeCQueue, &RearStoragAreaTypeCQueue, &G_numberOfChocolateBoxsInStorageAreaTypeC);
        }
        pthread_mutex_unlock(&G_mutexs_for_FillingTheCartonBoxes_Queues[2]);
    }
}

void fillingTheCartonBoxesA()
{
    message_payload chocolate1;
    message_payload chocolate2;
    while (1)
    {
        if (G_numberOfchocolatePatchesInContainerTypeA >= 2)
        {

            // Todo update index And Location to ui and update id
            chocolate1 = dequeueNodeFromQueueWithInternalMutex(&G_mutexs_for_Container_Queues[0], &FrontContainerTypeAQueue, &G_numberOfchocolatePatchesInContainerTypeA);
            chocolate2 = dequeueNodeFromQueueWithInternalMutex(&G_mutexs_for_Container_Queues[0], &FrontContainerTypeAQueue, &G_numberOfchocolatePatchesInContainerTypeA);
            chocolate1.current_location = CARTON_BOX_A;
            chocolate1.item_type = CARTON_BOX;
            chocolate2.current_location = CARTON_BOX_A;
            chocolate2.item_type = CARTON_BOX;
            enqueueToQueue(chocolate1, &G_mutexs_for_FillingTheCartonBoxes_Queues[0], &FrontFillingTheCartonBoxesTypeAQueue, &RearFillingTheCartonBoxesTypeAQueue, &G_numberOfChocolateBoxsInTheFillingCartonBoxesQueueTypeA);
        }
    }
}

void fillingTheCartonBoxesB()
{
    message_payload chocolate1;
    message_payload chocolate2;
    while (1)
    {
        if (G_numberOfchocolatePatchesInContainerTypeB >= 2)
        {

            // Todo update index And Location to ui and update id
            chocolate1 = dequeueNodeFromQueueWithInternalMutex(&G_mutexs_for_Container_Queues[1], &FrontContainerTypeBQueue, &G_numberOfchocolatePatchesInContainerTypeB);
            chocolate2 = dequeueNodeFromQueueWithInternalMutex(&G_mutexs_for_Container_Queues[1], &FrontContainerTypeBQueue, &G_numberOfchocolatePatchesInContainerTypeB);
            chocolate1.current_location = CARTON_BOX_B;
            chocolate1.item_type = CARTON_BOX;
            chocolate2.current_location = CARTON_BOX_B;
            chocolate2.item_type = CARTON_BOX;
            enqueueToQueue(chocolate1, &G_mutexs_for_FillingTheCartonBoxes_Queues[1], &FrontFillingTheCartonBoxesTypeBQueue, &RearFillingTheCartonBoxesTypeBQueue, &G_numberOfChocolateBoxsInTheFillingCartonBoxesQueueTypeB);
        }
    }
}

void fillingTheCartonBoxesC()
{
    message_payload chocolate1;
    message_payload chocolate2;
    while (1)
    {
        if (G_numberOfchocolatePatchesInContainerTypeC >= 2)
        {

            // Todo update index And Location to ui and update id
            chocolate1 = dequeueNodeFromQueueWithInternalMutex(&G_mutexs_for_Container_Queues[2], &FrontContainerTypeCQueue, &G_numberOfchocolatePatchesInContainerTypeC);
            chocolate2 = dequeueNodeFromQueueWithInternalMutex(&G_mutexs_for_Container_Queues[2], &FrontContainerTypeCQueue, &G_numberOfchocolatePatchesInContainerTypeC);
            chocolate1.current_location = CARTON_BOX_C;
            chocolate1.item_type = CARTON_BOX;
            chocolate2.current_location = CARTON_BOX_C;
            chocolate2.item_type = CARTON_BOX;
            enqueueToQueue(chocolate1, &G_mutexs_for_FillingTheCartonBoxes_Queues[2], &FrontFillingTheCartonBoxesTypeCQueue, &RearFillingTheCartonBoxesTypeCQueue, &G_numberOfChocolateBoxsInTheFillingCartonBoxesQueueTypeC);
        }
    }
}

void createThreads()
{
    // create Threads For Emps Work from Printer To Containers
    /* create a new 3 threads that will keep move chocolate from Printer Queue To Containers Queues */
    pthread_t p_thread1;
    pthread_create(&p_thread1, NULL, (void *)insertToContainers, NULL);
    pthread_t p_thread2;
    pthread_create(&p_thread2, NULL, (void *)insertToContainers, NULL);
    pthread_t p_thread3;
    pthread_create(&p_thread3, NULL, (void *)insertToContainers, NULL);

    // createThreadsForEmpsWorkfromContainersToFillingTheCartonBoxes
    //  create a new 3 threads that will keep move chocolate from Containers Queues To filling  Carton Boxes Queues
    pthread_t p_thread4;
    pthread_create(&p_thread4, NULL, (void *)fillingTheCartonBoxesA, NULL);
    pthread_t p_thread5;
    pthread_create(&p_thread5, NULL, (void *)fillingTheCartonBoxesB, NULL);
    pthread_t p_thread6;
    pthread_create(&p_thread6, NULL, (void *)fillingTheCartonBoxesC, NULL);

    // createThreadsForTwoEmpsWorkfromFillingTheCartonBoxesToStorageArea
    //  create a new 2 threads that will keep move chocolate from filling  Carton Boxes Queues To Storage Area Queues
    pthread_t p_thread7;
    pthread_create(&p_thread7, NULL, (void *)insertToStorageArea, NULL);
    pthread_t p_thread8;
    pthread_create(&p_thread8, NULL, (void *)insertToStorageArea, NULL);

    // create Threads For Two Emps Work from Storage Area To Truks
    //  create a new 2 threads that will keep move chocolate from Storage Area Queues To trucks
    pthread_t p_thread9;
    pthread_create(&p_thread9, NULL, (void *)insertToTrucks, NULL);
    pthread_t p_thread10;
    pthread_create(&p_thread10, NULL, (void *)insertToTrucks, NULL);

    // creatr Thread For Print
    pthread_t p_thread11;
    pthread_create(&p_thread11, NULL, (void *)printInfo, NULL);

    pthread_join(p_thread1, NULL);
    pthread_join(p_thread2, NULL);
    pthread_join(p_thread3, NULL);
    pthread_join(p_thread4, NULL);
    pthread_join(p_thread5, NULL);
    pthread_join(p_thread6, NULL);
    pthread_join(p_thread7, NULL);
    pthread_join(p_thread8, NULL);
    pthread_join(p_thread9, NULL);
    pthread_join(p_thread10, NULL);
    pthread_join(p_thread11, NULL);
}
