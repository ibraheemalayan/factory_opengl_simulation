#include "local.h"
#include "utils.h"
#include "globals.c"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// Arrays of threads
// Array of threads of_employees_per_manufacturing_line_each_executing_a_step_in_the_chocolate_manufacturing_process_typeA
pthread_t g_Array_of_Threads_TypeA[3][8];
// Array of threads of_employees_per_manufacturing_line_each_executing_a_step_in_the_chocolate_manufacturing_process_typeB
pthread_t g_Array_of_Threads_TypeB[2][6];
// Array of threads of_employees_per_manufacturing_line_each_executing_a_step_in_the_chocolate_manufacturing_process_typeC
pthread_t g_Array_of_Threads_TypeC[2][5];
// Generator thread; produces chocolate products to be processed
pthread_t generator;
// patcher employees
pthread_t patcher_employees[2];
// printer thread
pthread_t printer;

// array of pthread mutexes for each line
pthread_mutex_t A_pile_mutex [3][PILESIZE];
pthread_mutex_t B_pile_mutex [2][PILESIZE];
pthread_mutex_t C_pile_mutex [2][PILESIZE];
pthread_mutex_t patch_mutex_A;
pthread_mutex_t patch_mutex_B;
pthread_mutex_t patch_mutex_C;
// Arrays of mutex //Steps 1 to 6 have to happen in order


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

// Pointer to The storagAreaTypeA queue
struct chocolateNode *FrontStoragAreaTypeAQueue = NULL;
struct chocolateNode *RearStoragAreaTypeAQueue  = NULL;
// Pointer to The storagAreaTypeB queue
struct chocolateNode *FrontStoragAreaTypeBQueue = NULL;
struct chocolateNode *RearStoragAreaTypeBQueue  = NULL;
// Pointer to The storagAreaTypeC queue
struct chocolateNode *FrontStoragAreaTypeCQueue = NULL;
struct chocolateNode *RearStoragAreaTypeCQueue  = NULL;

//Mutex on printing Queue
pthread_mutex_t printingQueue_mutex = PTHREAD_MUTEX_INITIALIZER;

//Mutex on Container QueueS // index 0:A, 1:B, 2:C 
pthread_mutex_t G_mutexs_for_Container_Queues[3] = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER};

//Mutex on Storage Area Queues // index 0:A, 1:B, 2:C 
pthread_mutex_t G_mutexs_for_StorageArea_Queues[3] = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER};


//Number of chocolate products in containers
int G_numberOfchocolatePatchesInContainerTypeA = 0;
int G_numberOfchocolatePatchesInContainerTypeB = 0;
int G_numberOfchocolatePatchesInContainerTypeC = 0;

int G_numberOfchocolateInPrintingQueue = 0;

//Number of chocolate boxs in Storage area
int G_numberOfChocolateBoxsInStorageAreaTypeA = 0;
int G_numberOfChocolateBoxsInStorageAreaTypeB = 0;
int G_numberOfChocolateBoxsInStorageAreaTypeC = 0;


struct Object dequeueNodeFromQueue(pthread_mutex_t *mutex, struct chocolateNode **FrontQueue, int *numberOfchocolateItemInQueueTypeT);
void enqueueToQueue(struct Object chocolateInfo, pthread_mutex_t *mutex, struct chocolateNode **FrontQueue, struct chocolateNode **RearQueue, int *numberOfchocolateItemInQueueTypeT);
struct Object dequeueFromPrinterQueue();
void enqueuToPrinterQueue(struct Object chocolateInfo);
void createThreadsForEmpsWorkfromPrinterToContainers();
void createThreadsForEmpsWorkfromContainersToStorageArea();
void insertToStorageAreaA();
void insertToStorageAreaB();
void insertToStorageAreaC();
void insertToContainers();

//Test
void displyContainerQueueA();
void displyContainerQueueB();
void displyContainerQueueC();
void displyStorageAreaQueueA();
void displyStorageAreaQueueB();
void displyStorageAreaQueueC();
void displyPrintingQueue();
void testContainersQueue();
void testPrintingQueue();
void test_simulation();

// .................................end new........................................ 





int main()
{
    //load_user_defined_values();
    
    create_and_setup_message_queues();
    
    initiate_mutexes();

    //run_gui();

    // register signal handler for SIGINT to clean up
    signal(SIGINT, interrupt_sig_handler);

    //start_simulation();

    //..................................new mohammad..................................
    test_simulation();
    // .................................end new........................................
    


    join_all();

    // clean_up();
}

void initiate_mutexes(){
    int i,j;
    for (i = 0; i < 3; i++){
        for (j = 0; j < PILESIZE; j++){
            pthread_mutex_init(&A_pile_mutex[i][j],NULL);
        }
    }

    for (i = 0; i < 2; i++){
        for (j = 0; j < PILESIZE; j++){
            pthread_mutex_init(&B_pile_mutex[i][j],NULL);
        }
    }

    for (i = 0; i < 2; i++){
        for (j = 0; j < PILESIZE; j++){
            pthread_mutex_init(&C_pile_mutex[i][j],NULL);
        }
    }

    pthread_mutex_init(&patch_mutex_A, NULL);

    pthread_mutex_init(&patch_mutex_B, NULL);

    pthread_mutex_init(&patch_mutex_C, NULL);
}

void start_simulation()
{
    create_generator_thread();
    create_employees_threads_type_A();
    create_employees_threads_type_B();
    create_employees_threads_type_C();
    create_patcher_employees();
    create_printer_machine();
}

void create_generator_thread(){
    if (pthread_create(&generator, NULL, (void *)generator_routine, NULL) == -1){
        perror("generator thread creation: ");
        exit(1);
    }
}

void create_employees_threads_type_A()
{
    int i = 0, j = 0;
    employee_information position;
    for (j = 0; j < C_MANUFACTURING_LINES_TYPEA; j++)
    {
        for (i = 0; i < C_EMPLOYEES_PER_LINE_IN_MANUFACTURING_PROCESS_TYPE_A; i++)
        {
            position.linenum = j;
            position.index = i;
            if (pthread_create(&g_Array_of_Threads_TypeA[j][i], NULL, (void *)employee_lineA, (void *)&position) == -1){
                perror("Employee A, thread creation : ");
                exit(1);
            }
            usleep(30);
        }
    }
}

void create_employees_threads_type_B()
{
    int i = 0, j = 0;
    employee_information position;
    for (j = 0; j < C_MANUFACTURING_LINES_TYPEB; j++)
    {
        for (i = 0; i < C_EMPLOYEES_PER_LINE_IN_MANUFACTURING_PROCESS_TYPE_B; i++)
        {
            position.linenum = j;
            position.index = i;
            if (pthread_create(&g_Array_of_Threads_TypeB[j][i], NULL, (void *)employee_lineB, (void *)&position) == -1){
                perror("Employee B, thread creation : ");
                exit(1);
            }
            usleep(30);
        }
    }
}

void create_employees_threads_type_C()
{
    int i = 0, j = 0;
    employee_information position;
    for (j = 0; j < C_MANUFACTURING_LINES_TYPEC; j++)
    {
        for (i = 0; i < C_EMPLOYEES_PER_LINE_IN_MANUFACTURING_PROCESS_TYPE_C; i++)
        {
            position.linenum = j;
            position.index = i;
            if (pthread_create(&g_Array_of_Threads_TypeC[j][i], NULL, (void *)employee_lineC, (void *)&position) == -1){
                perror("Employee C, thread creation : ");
                exit(1);
            }
            usleep(30);
        }
    }
}

void create_patcher_employees(){

    for (int i =0; i<2; i ++){
        if (pthread_create(&patcher_employees[i], NULL, (void *)patcher_routine, NULL) == -1){
            perror("Patcher Employee , thread creation : ");
            exit(1);
        }
    }
}

void create_printer_machine(){
    if (pthread_create(&printer, NULL, (void *)printer_routine, NULL) == -1){
            perror("Patcher Employee , thread creation : ");
            exit(1);
        }
}


void generator_routine(void *argptr){
    int empty_index, j,i;

    //initialize piles
    for (i = 0; i < C_MANUFACTURING_LINES_TYPEA; i++){
        for (j=0; j<PILESIZE; j++){
            type_A_pile[i][j] = (chocolateProduct *)malloc(sizeof(chocolateProduct));
            type_A_pile[i][j] -> id = 0;
        }
    }
    for (i = 0; i < C_MANUFACTURING_LINES_TYPEB; i++){
        for (j=0; j<PILESIZE; j++){
            type_B_pile[i][j] = (chocolateProduct *)malloc(sizeof(chocolateProduct));
            type_B_pile[i][j] -> id = 0;
        }
    }
    for (i = 0; i < C_MANUFACTURING_LINES_TYPEC; i++){
        for (j=0; j<PILESIZE; j++){
            type_C_pile[i][j] = (chocolateProduct *)malloc(sizeof(chocolateProduct));
            type_C_pile[i][j] -> id = 0;
        }
    }

    for(i = 0; i < PILESIZE; i++){ //temporary because of lack of termination condition
        for (j = 0 ; j < C_MANUFACTURING_LINES_TYPEA; j++){
            empty_index = array_full(type_A_pile[j]);
            if (empty_index != PILESIZE)
                generate_product(empty_index, 'a',j );
        }

        for (j =0 ; j < C_MANUFACTURING_LINES_TYPEB; j++){
            empty_index = array_full(type_B_pile[j]);
            if (empty_index != PILESIZE)
                generate_product(empty_index, 'b',j);
        }

        for (j =0 ; j < C_MANUFACTURING_LINES_TYPEC; j++){
            empty_index = array_full(type_C_pile[j]);
            if (empty_index != PILESIZE)
                generate_product(empty_index, 'c',j);
        }
        usleep(50000); //to be changed later
    }
}

void employee_lineA(void * position){
    employee_information *temp  = (employee_information *) position;
    int linenum = temp->linenum;
    int index = temp->index;
    message_buf buf;
    buf.mtype=1;
    buf.payload.chocolate_type=TYPE_A;
    buf.payload.msg_type = 1;
    buf.payload.item_type = PRODUCT;
    
    switch (linenum){
        case 0 :
            buf.payload.current_location = MANUFACTURING_LINE_A1;
            break;
        case 1 :
            buf.payload.current_location = MANUFACTURING_LINE_A2;
            break;
        case 2 :
            buf.payload.current_location = MANUFACTURING_LINE_A3;
            break;
    }
    buf.payload.index = index;

    int i = 0, j = 0;
    srand(g_Array_of_Threads_TypeA[linenum][index]);
    int step_time = (C_MIN_A + rand()) % C_MAX_A;
    while(1){
        i =find_product(type_A_pile[linenum], i);
        if (i != PILESIZE)
        {
            if(index < 4)   // if index is 0-3 
            {
            
                if (pthread_mutex_trylock(&A_pile_mutex[linenum][i]) == 0){ //trys to lock mutex
                    buf.payload.id = type_A_pile[linenum][i]->id;   // if mutex lock is a successm then load buf id to send to ui

                    if  (index != 0){   //if step 1-3

                        if ((type_A_pile[linenum][i]->progress & (1<<(index-1))) ==  1<<(index-1)){ // check to see if previous step is done

                            if (msgsnd(ui_msgq_id, &buf, sizeof(buf), 0) == -1)
                                perror("Line A UI Message Queue");
                            type_A_pile[linenum][i]->progress = type_A_pile[linenum][i]->progress | 1 << index; // do step
                            usleep(step_time);
                        }

                    }else{
                        // do step without checking
                        if (msgsnd(ui_msgq_id, &buf, sizeof(buf), 0) == -1)
                             perror("Line A UI Message Queue");
                        type_A_pile[linenum][i]->progress = type_A_pile[linenum][i]->progress | 1 << index;
                        usleep(step_time);

                    }
                    if (type_A_pile[linenum][i]->progress == 0xff){ //if all steps are done send item, and remove from pile
                        if (msgsnd(patcher_msgq_id, &buf, sizeof(buf), 0) == -1)
                            perror("Line A Message Queue");
                        type_A_pile[linenum][i]->id = 0;
                        type_A_pile[linenum][i]->progress = 0;
                        type_A_pile[linenum][i]->type = TYPE_A;
                    }
                    pthread_mutex_unlock(&A_pile_mutex[linenum][i]);

                }
            } 
            else
            {
                //all previous steps but without checking
                if (pthread_mutex_trylock(&A_pile_mutex[linenum][i]) == 0){

                    buf.payload.id = type_A_pile[linenum][i]->id;
                    if (msgsnd(ui_msgq_id, &buf, sizeof(buf), 0) == -1)
                         perror("Line A UI Message Queue");
                     type_A_pile[linenum][i]->progress = type_A_pile[linenum][i]->progress | (1<<index);
                    usleep(step_time);
                    if (type_A_pile[linenum][i]->progress == 0xff){
                        if (msgsnd(patcher_msgq_id, &buf, sizeof(buf), 0) == -1)
                            perror("Line A Patcher Message Queue");
                        type_A_pile[linenum][i]->id = 0;
                        type_A_pile[linenum][i]->progress = 0;
                        type_A_pile[linenum][i]->type = TYPE_A;
                    }
                    pthread_mutex_unlock(&A_pile_mutex[linenum][i]);

                }

            }
    }
    i = (i+1)%PILESIZE;
    j++;
    if (j == 20000) // temporary termination condition
        break;
    }
    pthread_exit(0);
}

void employee_lineB(void * position){
    employee_information *temp  = (employee_information *) position;
    int linenum = temp->linenum;
    int index = temp->index;
    message_buf buf;
    buf.mtype=1;
    buf.payload.chocolate_type=TYPE_B;
    buf.payload.msg_type = 1;
    buf.payload.item_type = PRODUCT;
    
    switch (linenum){
        case 0 :
            buf.payload.current_location = MANUFACTURING_LINE_B1;
            break;
        case 1 :
            buf.payload.current_location = MANUFACTURING_LINE_B2;
            break;
    }
    buf.payload.index = index;

    int i = 0, j = 0;
    srand(g_Array_of_Threads_TypeB[linenum][index]);
    int step_time = (C_MIN_B + rand()) % C_MAX_B;
    while(1){
        i =find_product(type_B_pile[linenum], i);
        if (i != PILESIZE)
        {
            if (pthread_mutex_trylock(&B_pile_mutex[linenum][i]) == 0){
                buf.payload.id = type_B_pile[linenum][i]->id;
                if (msgsnd(ui_msgq_id, &buf, sizeof(buf), 0) == -1)
                    perror("Line B UI Message Queue");
                type_B_pile[linenum][i]->progress = type_B_pile[linenum][i]->progress | (1 << index);
                usleep(step_time);
                if (type_B_pile[linenum][i]->progress == 0x3f){
                    if (msgsnd(patcher_msgq_id, &buf, sizeof(buf), 0) == -1)
                         perror("Line B Patcher Message Queue");
                    type_B_pile[linenum][i]->id = 0;
                    type_B_pile[linenum][i]->progress = 0;
                    type_B_pile[linenum][i]->type = TYPE_B;
                }
                pthread_mutex_unlock(&B_pile_mutex[linenum][i]);

            }
    }
    i = (i+1)%PILESIZE;
    j++;
    if (j == 20000)// write termination condition here
        break;
    }
    pthread_exit(0);

}

void employee_lineC(void * position){
    employee_information *temp  = (employee_information *) position;
    int linenum = temp->linenum;
    int index = temp->index;
    message_buf buf;
    buf.mtype=1;
    buf.payload.chocolate_type=TYPE_C;
    buf.payload.msg_type = 1;
    buf.payload.item_type = PRODUCT;
    switch (linenum){
        case 0 :
            buf.payload.current_location = MANUFACTURING_LINE_C1;
            break;
        case 1 :
            buf.payload.current_location = MANUFACTURING_LINE_C2;
            break;
    }
    buf.payload.index = index;

    int i = 0, j = 0;
    srand(g_Array_of_Threads_TypeC[linenum][index]);
    int step_time = (C_MIN_C + rand()) % C_MAX_C;
    while(1){
        i =find_product(type_C_pile[linenum], i);
        if (i != PILESIZE)
        {
            if(index < 4)
            {
            
                if (pthread_mutex_trylock(&C_pile_mutex[linenum][i]) == 0){
                    buf.payload.id = type_C_pile[linenum][i]->id;
                    if  (index != 0){

                        if ((type_C_pile[linenum][i]->progress & (1<<(index-1))) == 1<<(index-1)){

                            if (msgsnd(ui_msgq_id, &buf, sizeof(buf), 0) == -1)
                                perror("Line C UI Message Queue");
                            type_C_pile[linenum][i]->progress = type_C_pile[linenum][i]->progress | (1<<index);
                            usleep(step_time);
                        }

                    }else{

                        if (msgsnd(ui_msgq_id, &buf, sizeof(buf), 0) == -1)
                             perror("Line C UI Message Queue");
                        type_C_pile[linenum][i]->progress = type_C_pile[linenum][i]->progress | (1<<index);
                        usleep(step_time);

                    }
                    if (type_C_pile[linenum][i]->progress == 0x1f){
                        if (msgsnd(patcher_msgq_id, &buf, sizeof(buf), 0) == -1)
                            perror("Line C patcher Message Queue");
                        type_C_pile[linenum][i]->id = 0;
                        type_C_pile[linenum][i]->progress = 0;
                        type_C_pile[linenum][i]->type = TYPE_C;
                    }
                    pthread_mutex_unlock(&C_pile_mutex[linenum][i]);

                }
            } 
            else
            {

                if (pthread_mutex_trylock(&C_pile_mutex[linenum][i]) == 0){

                    buf.payload.id = type_C_pile[linenum][i]->id;
                    if (msgsnd(ui_msgq_id, &buf, sizeof(buf), 0) == -1)
                        perror("Line C UI Message Queue");
                    type_C_pile[linenum][i]->progress = type_C_pile[linenum][i]->progress | (1<<index);
                    usleep(step_time);

                    if (type_C_pile[linenum][i]->progress == 0x1f){
                        if (msgsnd(patcher_msgq_id, &buf, sizeof(buf), 0) == -1)
                             perror("Line C Patcher Message Queue");
                        type_C_pile[linenum][i]->id = 0;
                        type_C_pile[linenum][i]->progress = 0;
                        type_C_pile[linenum][i]->type = TYPE_C;
                    }

                    pthread_mutex_unlock(&C_pile_mutex[linenum][i]);

                }

            }
    }
    i = (i+1)%PILESIZE;
    j++;
    if (j == 20000)
        break;
    }
    pthread_exit(0);
}

void patcher_routine(void *argptr){
    int j = 0;
    message_buf buf;
    message_buf send;
    send.mtype = 1;
    send.payload.item_type = PATCH;
    while(1){
        if (msgrcv(patcher_msgq_id,  &buf, sizeof(buf),1,IPC_NOWAIT) == -1){
            
        }else{
            buf.mtype = 1;
            switch (buf.payload.chocolate_type){
                case TYPE_A:
                    buf.payload.current_location=PATCHING_A;
                    break;
                case TYPE_B:
                    buf.payload.current_location=PATCHING_B;
                    break;
                case TYPE_C:
                    buf.payload.current_location=PATCHING_C;
                    break;
            }
            send.payload.current_location=buf.payload.current_location;
            msgsnd(ui_msgq_id,&buf,sizeof(buf),0);

            if (buf.payload.chocolate_type == TYPE_A){
                pthread_mutex_lock(&patch_mutex_A);
                type_A_patch++;
                if (type_A_patch == 10){
                    type_A_patch = 0;
                    send.payload.chocolate_type = TYPE_A;
                    buf.payload.item_type=PATCH;
                    buf.payload.id = patch_id_A;
                    patch_id_A += 10;
                    msgsnd(ui_msgq_id,&buf,sizeof(buf),0);
                    msgsnd(printer_msgq_id, &send, sizeof(send), 0);
                }
                usleep(PATCHER_TIME);
                pthread_mutex_unlock(&patch_mutex_A);
            }else if (buf.payload.chocolate_type == TYPE_B){
                pthread_mutex_lock(&patch_mutex_B);
                type_B_patch++;
                if (type_B_patch == 10){
                    type_B_patch = 0;
                    send.payload.chocolate_type = TYPE_B;
                    buf.payload.item_type=PATCH;
                    buf.payload.id = patch_id_B;
                    patch_id_B += 10;
                    msgsnd(ui_msgq_id,&buf,sizeof(buf),0);
                    msgsnd(printer_msgq_id, &send, sizeof(send), 0);
                }
                usleep(PATCHER_TIME);
                pthread_mutex_unlock(&patch_mutex_B);
            }else if (buf.payload.chocolate_type == TYPE_C){
                pthread_mutex_lock(&patch_mutex_C);
                type_C_patch++;
                if (type_C_patch == 10){
                    type_C_patch = 0;
                    send.payload.chocolate_type = TYPE_C;
                    buf.payload.item_type=PATCH;
                    buf.payload.id = patch_id_C;
                    patch_id_C += 10;
                    msgsnd(ui_msgq_id,&buf,sizeof(buf),0);
                    msgsnd(printer_msgq_id, &send, sizeof(send), 0);
                }
                usleep(PATCHER_TIME);
                pthread_mutex_unlock(&patch_mutex_C);
            }

        }
        j++;
        if (j == 1000)//termination condition
            break;
    }
    pthread_exit(0);
}

void printer_routine(void *argptr){
    int j = 0;
    message_buf buf;
    while(1){
        if(msgrcv(printer_msgq_id, &buf, sizeof(buf), 1, IPC_NOWAIT) == -1){
        }else{
            buf.payload.current_location = PRINTER;
            msgsnd(ui_msgq_id,&buf,sizeof(buf),0);
            for (int i = 0; i < 10; i ++){
                //print date on patche node
                usleep(PRINTER_TIME);
            }
            // append to patche queue
        }
        j++;
        if (j==1000) // termination condition needed
            break;
    }

}

void join_all(){
    int i,j;

    pthread_join(generator,NULL);

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

    for (i = 0; i < 2; i++){
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
    if (fp == NULL){
        perror("load_user_defined_values:");
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
    return (rand() % upper) + lower;
}



//..................................new mohammad.................................. 

/*Noor : use this function with the two threads:
 "The chocolate products that are produced by all the manufacturing lines are collected in patches of 10 pieces per type by 2 employees"
 Hint 1 :each thread form these two threads should at first fillThePatche then enqueuToPrinterQueue  */ 
 /*Noor
 Hint 2 :enqueuToPrinterQueue & dequeueFromPrinterQueue became deal with just one Item of chocolate
 */
 

void enqueuToPrinterQueue(struct Object chocolateInfo){
        pthread_mutex_lock(&printingQueue_mutex);
        struct chocolateNode *ptr = (struct chocolateNode *)malloc(sizeof(struct chocolateNode));
        if (ptr == NULL)
        {
                printf("\nOVERFLOW\n");
                return;
        }
        else
        {
                //Todo update index to ui 
                
                ptr->chocolateInfo =chocolateInfo;
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
        G_numberOfchocolateInPrintingQueue++;        
        //usleep(randomIntegerInRange(SLEEP_MIN, SLEEP_MAX));
        }
        pthread_mutex_unlock(&printingQueue_mutex); 
}


struct Object dequeueFromPrinterQueue(){
        
        struct chocolateNode *chocolate= NULL;
        struct Object chocolateInfo = {0};
        if (FrontPrinterQueue == NULL)
        {
                printf("Underflow dequeueFromPrinterQueue\n");
                return chocolateInfo;
        }
        else
        {
                chocolate = FrontPrinterQueue;
                chocolateInfo = chocolate->chocolateInfo;
                FrontPrinterQueue = FrontPrinterQueue->next;
                if(FrontPrinterQueue == NULL)
                    RearPrinterQueue =NULL;

                free(chocolate);
                G_numberOfchocolateInPrintingQueue--;

                
        }
        return chocolateInfo; 
}

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

struct Object dequeueNodeFromQueue(pthread_mutex_t *mutex, struct chocolateNode **FrontQueue, int *numberOfchocolateItemInQueueTypeT)
{   
    
    pthread_mutex_lock(mutex);
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
    pthread_mutex_unlock(mutex);
    return chocolate;
}



void displyPrintingQueue()
{

    struct chocolateNode *temp = NULL;
    if ((FrontPrinterQueue == NULL) && (RearPrinterQueue == NULL))
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


void testPrintingQueue(){
        struct Object ch1;
        ch1.chocolate_type=TYPE_A;
        ch1.current_location=PRINTER;
        ch1.pkg_type=PATCH;
        ch1.index_in_queue=0;
        ch1.id=1;
        enqueuToPrinterQueue(ch1);
        struct Object ch2;
        ch2.chocolate_type=TYPE_B;
        ch2.current_location=PRINTER;
        ch2.pkg_type=PATCH;
        ch2.index_in_queue=0;
        ch2.id=2;
        enqueuToPrinterQueue(ch2);
        struct Object ch3;
        ch3.chocolate_type=TYPE_C;
        ch3.current_location=PRINTER;
        ch3.pkg_type=PATCH;
        ch3.index_in_queue=0;
        ch3.id=3;
        enqueuToPrinterQueue(ch3);
        struct Object ch5;
        ch5.chocolate_type=TYPE_A;
        ch5.current_location=PRINTER;
        ch5.pkg_type=PATCH;
        ch5.index_in_queue=0;
        ch5.id=4;
        enqueuToPrinterQueue(ch5);
        struct Object ch4;
        ch4.chocolate_type=TYPE_B;
        ch4.current_location=PRINTER;
        ch4.pkg_type=PATCH;
        ch4.index_in_queue=0;
        ch4.id=5;
        enqueuToPrinterQueue(ch4);
        struct Object ch6;
        ch6.chocolate_type=TYPE_C;
        ch6.current_location=PRINTER;
        ch6.pkg_type=PATCH;
        ch6.index_in_queue=0;
        ch6.id=6;
        enqueuToPrinterQueue(ch6);
        displyPrintingQueue();

}

void testContainersQueue(){
        struct Object ch1= dequeueFromPrinterQueue();
        ch1.current_location=CONTAINER_A;
        ch1.index_in_queue=0;
        enqueueToQueue(ch1, &G_mutexs_for_Container_Queues[0], &FrontContainerTypeAQueue, &RearContainerTypeAQueue, &G_numberOfchocolatePatchesInContainerTypeA);

        struct Object ch2= dequeueFromPrinterQueue(); 
        ch2.current_location=CONTAINER_B;
        ch2.index_in_queue=0;
        enqueueToQueue(ch2, &G_mutexs_for_Container_Queues[0], &FrontContainerTypeBQueue, &RearContainerTypeBQueue, &G_numberOfchocolatePatchesInContainerTypeA);
        

}

void insertToContainers(){
    struct Object chocolate;
    while (1)
    {   
        pthread_mutex_lock(&printingQueue_mutex);
        if(G_numberOfchocolateInPrintingQueue >0){
            chocolate = dequeueFromPrinterQueue(); 
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
}

void createThreadsForEmpsWorkfromContainersToStorageArea(){
    // create a new 3 threads that will keep move chocolate from Containers Queues To StorageArea Queues 
    pthread_t p_thread4;
    pthread_create(&p_thread4, NULL, (void *)insertToStorageAreaA, NULL);

    pthread_t p_thread5;
    pthread_create(&p_thread5, NULL, (void *)insertToStorageAreaB, NULL);
    pthread_t p_thread6;
    pthread_create(&p_thread6, NULL, (void *)insertToStorageAreaC, NULL);
    
}

void insertToStorageAreaA(){
    struct Object chocolate1;
    struct Object chocolate2;
    while (1)
    {
            if(G_numberOfchocolatePatchesInContainerTypeA >= 2){

                    //Todo update index And Location to ui and update id
                    chocolate1=dequeueNodeFromQueue(&G_mutexs_for_Container_Queues[0], &FrontContainerTypeAQueue, &G_numberOfchocolatePatchesInContainerTypeA);
                    chocolate2=dequeueNodeFromQueue(&G_mutexs_for_Container_Queues[0], &FrontContainerTypeAQueue, &G_numberOfchocolatePatchesInContainerTypeA);
                    chocolate1.current_location=CARTON_BOX_A;
                    chocolate1.current_location=STORAGE_AREA_A; // !?
                    chocolate1.pkg_type=CARTON_BOX;
                    chocolate2.current_location=CARTON_BOX_A;
                    chocolate2.current_location=STORAGE_AREA_A; // !?
                    chocolate2.pkg_type=CARTON_BOX;
                    enqueueToQueue(chocolate1, &G_mutexs_for_StorageArea_Queues[0], &FrontStoragAreaTypeAQueue, &RearStoragAreaTypeAQueue, &G_numberOfChocolateBoxsInStorageAreaTypeA);
            }
    }
}

void insertToStorageAreaB(){
    struct Object chocolate1;
    struct Object chocolate2;
    while (1)
    {
            if(G_numberOfchocolatePatchesInContainerTypeB >= 2){

                    //Todo update index And Location to ui and update id
                    chocolate1=dequeueNodeFromQueue(&G_mutexs_for_Container_Queues[1], &FrontContainerTypeBQueue, &G_numberOfchocolatePatchesInContainerTypeB);
                    chocolate2=dequeueNodeFromQueue(&G_mutexs_for_Container_Queues[1], &FrontContainerTypeBQueue, &G_numberOfchocolatePatchesInContainerTypeB);
                    chocolate1.current_location=CARTON_BOX_B;
                    chocolate1.current_location=STORAGE_AREA_B; // !?
                    chocolate1.pkg_type=CARTON_BOX;
                    chocolate2.current_location=CARTON_BOX_B;
                    chocolate2.current_location=STORAGE_AREA_B; // !?
                    chocolate2.pkg_type=CARTON_BOX;
                    enqueueToQueue(chocolate1, &G_mutexs_for_StorageArea_Queues[1], &FrontStoragAreaTypeBQueue, &RearStoragAreaTypeBQueue, &G_numberOfChocolateBoxsInStorageAreaTypeB);
            }
    }
}

void insertToStorageAreaC(){
    struct Object chocolate1;
    struct Object chocolate2;
    while (1)
    {
            if(G_numberOfchocolatePatchesInContainerTypeC >= 2){

                    //Todo update index And Location to ui and update id
                    chocolate1=dequeueNodeFromQueue(&G_mutexs_for_Container_Queues[2], &FrontContainerTypeCQueue, &G_numberOfchocolatePatchesInContainerTypeC);
                    chocolate2=dequeueNodeFromQueue(&G_mutexs_for_Container_Queues[2], &FrontContainerTypeCQueue, &G_numberOfchocolatePatchesInContainerTypeC);
                    chocolate1.current_location=CARTON_BOX_C;
                    chocolate1.current_location=STORAGE_AREA_C; // !?
                    chocolate1.pkg_type=CARTON_BOX;
                    chocolate2.current_location=CARTON_BOX_C;
                    chocolate2.current_location=STORAGE_AREA_C; // !?
                    chocolate2.pkg_type=CARTON_BOX;
                    enqueueToQueue(chocolate1, &G_mutexs_for_StorageArea_Queues[2], &FrontStoragAreaTypeCQueue, &RearStoragAreaTypeCQueue, &G_numberOfChocolateBoxsInStorageAreaTypeC);
            }
    }
}
void test_simulation(){
    testPrintingQueue();
    createThreadsForEmpsWorkfromPrinterToContainers();
    createThreadsForEmpsWorkfromContainersToStorageArea();
    sleep(3);
    displyContainerQueueA();
    displyContainerQueueB();
    displyContainerQueueC();
    displyPrintingQueue();
    sleep(5);
    displyContainerQueueA();
    displyContainerQueueB();
    displyContainerQueueC();
    displyPrintingQueue();
    displyStorageAreaQueueA();
    displyStorageAreaQueueB();
    displyStorageAreaQueueC();

    
}
