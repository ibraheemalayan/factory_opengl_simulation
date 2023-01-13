#include "local.h"
#include "utils.h"
#include "globals.c"
#include <bits/pthreadtypes.h>
#include <pthread.h>
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

// array of pthread mutexes for each line
pthread_mutex_t A_pile_mutex [3][PILESIZE];
pthread_mutex_t B_pile_mutex [2][PILESIZE];
pthread_mutex_t C_pile_mutex [2][PILESIZE];
// Arrays of mutex //Steps 1 to 6 have to happen in order

//.................Functions....................
void initiate_mutexes();
void load_user_defined_values();
void start_simulation();
void create_generator_thread();
void create_employees_threads_type_A();
void create_employees_threads_type_B();
void create_employees_threads_type_C();
void join_all();
int randomIntegerInRange(int lower, int upper);
void employeeDoStepPerLine(void *time_for_emp_takes_to_finsh_step_per_line);
void generator_routine(void *argptr);
void employee_lineA(void *position);
void employee_lineB(void *position);
void employee_lineC(void *position);

void main()
{
    //load_user_defined_values();
    
    create_and_setup_message_queues();
    
    initiate_mutexes();

    //run_gui();

    // register signal handler for SIGINT to clean up
    signal(SIGINT, interrupt_sig_handler);

    start_simulation();

    join_all();

    clean_up();
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
}

void start_simulation()
{
    // printf("generating threads\n");
    create_generator_thread();
    create_employees_threads_type_A();
    create_employees_threads_type_B();
    create_employees_threads_type_C();
}

void create_generator_thread(){
    pthread_create(&generator, NULL, (void *)generator_routine, NULL);
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
            // printf("postion : (%d,%d)\n", position.linenum, position.index);
            pthread_create(&g_Array_of_Threads_TypeA[j][i], NULL, (void *)employee_lineA, (void *)&position);
            usleep(5);
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
            // printf("postion : (%d,%d)\n", position.linenum, position.index);
            pthread_create(&g_Array_of_Threads_TypeB[j][i], NULL, (void *)employee_lineB, (void *)&position);
            usleep(5);
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
            // printf("postion : (%d,%d)\n", position.linenum, position.index);
            pthread_create(&g_Array_of_Threads_TypeC[j][i], NULL, (void *)employee_lineC, (void *)&position);
            usleep(5);
        }
    }
}
void generator_routine(void *argptr){
    int empty_index, j,i;

    //initialize piles
    for (i = 0; i < C_MANUFACTURING_LINES_TYPEA; i++){
        for (j=0; j<PILESIZE; j++){
            type_A_pile[i][j] = (chocolateProduct *)malloc(sizeof(chocolateProduct));
            type_A_pile[i][j] = NULL;
        }
    }
    for (i = 0; i < C_MANUFACTURING_LINES_TYPEB; i++){
        for (j=0; j<PILESIZE; j++){
            type_B_pile[i][j] = (chocolateProduct *)malloc(sizeof(chocolateProduct));
            type_B_pile[i][j] = NULL;
        }
    }
    for (i = 0; i < C_MANUFACTURING_LINES_TYPEC; i++){
        for (j=0; j<PILESIZE; j++){
            type_C_pile[i][j] = (chocolateProduct *)malloc(sizeof(chocolateProduct));
            type_C_pile[i][j] = NULL;
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
    // employee_information *temp  = (employee_information *) position;
    // int linenum = temp->linenum;
    // int index = temp->index;
    // message_buf buf;
    // buf.mtype=1;
    // buf.payload.chocolate_type=TYPE_A;
    // buf.payload.msg_type = 1;
    // buf.payload.item_type = PRODUCT;
    // switch (linenum){
    //     case 0 :
    //         buf.payload.current_location = MANUFACTURING_LINE_A1;
    //         break;
    //     case 1 :
    //         buf.payload.current_location = MANUFACTURING_LINE_A2;
    //         break;
    //     case 2 :
    //         buf.payload.current_location = MANUFACTURING_LINE_A3;
    //         break;
    // }
    // buf.payload.index = index;

    // int i = 0, j = 0;
    // srand(g_Array_of_Threads_TypeA[linenum][index]);
    // int step_time = (C_MIN_A + rand()) % C_MAX_A;
    // while(1){
    //     i = find_product(type_A_pile[linenum], i);
    //     if (i != PILESIZE)
    //     {
    //         if(index < 4)
    //         {
            
    //             if (pthread_mutex_trylock(&A_pile_mutex[linenum][i])){
    //                 buf.payload.id = type_A_pile[linenum][i]->id;

    //                 if  (index != 0){

    //                     if ((type_A_pile[linenum][i]->progress & (1<<(index-1))) ==  1<<(index-1)){

    //                         buf.payload.id = type_A_pile[linenum][i]->id;
    //                         if (msgsnd(ui_msgq_id, &buf, sizeof(buf), 0) == -1)
    //                             perror("Line A UI Message Queue");
    //                         type_A_pile[linenum][i]->progress = type_A_pile[linenum][i]->progress;
    //                         usleep(step_time);
    //                     }

    //                 }else{

    //                     if (msgsnd(ui_msgq_id, &buf, sizeof(buf), 0) == -1)
    //                          perror("Line A UI Message Queue");
    //                     type_A_pile[linenum][i]->progress = type_A_pile[linenum][i]->progress;
    //                     usleep(step_time);

    //                 }
    //                 printf("A : %X\n", type_A_pile[linenum][i]->progress);
    //                 if (type_A_pile[linenum][i]->progress == 0xff){
    //                     printf("ready\n");
    //                     if (msgsnd(patcher_msgq_id, &buf, sizeof(buf), 0) == -1)
    //                         perror("Line A Message Queue");
    //                     type_A_pile[linenum][i] = NULL;
    //                 }
    //                 pthread_mutex_unlock(&A_pile_mutex[linenum][i]);

    //             }
    //         } 
    //         else
    //         {

    //             if (pthread_mutex_trylock(&A_pile_mutex[linenum][i])){

    //                 buf.payload.id = type_A_pile[linenum][i]->id;
    //                 if (msgsnd(ui_msgq_id, &buf, sizeof(buf), 0) == -1)
    //                     perror("Line A UI Message Queue");
    //                 type_A_pile[linenum][i]->progress = type_A_pile[linenum][i]->progress | (1<<index);
    //                 usleep(step_time);
    //                 printf("A : %X\n", type_A_pile[linenum][i]->progress);
    //                 if (type_A_pile[linenum][i]->progress == 0xff){
    //                     printf("ready\n");
    //                     if (msgsnd(patcher_msgq_id, &buf, sizeof(buf), 0) == -1)
    //                         perror("Line A Patcher Message Queue");
    //                     type_A_pile[linenum][i] = NULL;
    //                 }
    //                 pthread_mutex_unlock(&A_pile_mutex[linenum][i]);

    //             }

    //         }
    // }
    // i = (i+1)%PILESIZE;
    // j++;
    // if (j == 10000)
    //     break;
    // }
    // pthread_exit(0);
}

void employee_lineB(void * position){
    employee_information *temp  = (employee_information *) position;
    int linenum = temp->linenum;
    int index = temp->index;
    //printf("%d\n",index);
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
    // printf("%d", index);
    while(1){
        i = find_product(type_B_pile[linenum], i);
        if (i != PILESIZE)
        {
            if (pthread_mutex_trylock(&B_pile_mutex[linenum][i])){

                buf.payload.id = type_B_pile[linenum][i]->id;
                if (msgsnd(ui_msgq_id, &buf, sizeof(buf), 0) == -1)
                    perror("Line B UI Message Queue");
                type_B_pile[linenum][i]->progress = type_B_pile[linenum][i]->progress | (1 << index);
                usleep(step_time);
                //printf("B : %b \n", 0 | (1 << index));
                if (type_B_pile[linenum][i]->progress == 0x3f){
                    printf("ready\n");
                    if (msgsnd(patcher_msgq_id, &buf, sizeof(buf), 0) == -1)
                         perror("Line B Patcher Message Queue");
                    type_B_pile[linenum][i] = NULL;
                }
                pthread_mutex_unlock(&B_pile_mutex[linenum][i]);

            }
    }
    i = (i+1)%PILESIZE;
    j++;
    if (j == 10)
        break;
    }
    pthread_exit(0);

}

void employee_lineC(void * position){
    // employee_information *temp  = (employee_information *) position;
    // int linenum = temp->linenum;
    // int index = temp->index;
    // message_buf buf;
    // buf.mtype=1;
    // buf.payload.chocolate_type=TYPE_C;
    // buf.payload.msg_type = 1;
    // buf.payload.item_type = PRODUCT;
    // switch (linenum){
    //     case 0 :
    //         buf.payload.current_location = MANUFACTURING_LINE_C1;
    //         break;
    //     case 1 :
    //         buf.payload.current_location = MANUFACTURING_LINE_C2;
    //         break;
    // }
    // buf.payload.index = index;

    // int i = 0, j = 0;
    // srand(g_Array_of_Threads_TypeC[linenum][index]);
    // int step_time = (C_MIN_C + rand()) % C_MAX_C;
    // while(1){
    //     i = find_product(type_C_pile[linenum], i);
    //     if (i != PILESIZE)
    //     {
    //         if(index < 4)
    //         {
            
    //             if (pthread_mutex_trylock(&C_pile_mutex[linenum][i])){
    //                 buf.payload.id = type_C_pile[linenum][i]->id;
    //                 if  (index != 0){

    //                     if ((type_C_pile[linenum][i]->progress & (1<<(index-1))) == 1<<(index-1)){

    //                         if (msgsnd(ui_msgq_id, &buf, sizeof(buf), 0) == -1)
    //                             perror("Line C UI Message Queue");
    //                         type_C_pile[linenum][i]->progress = type_C_pile[linenum][i]->progress | (1<<index);
    //                         usleep(step_time);
    //                     }

    //                 }else{

    //                     if (msgsnd(ui_msgq_id, &buf, sizeof(buf), 0) == -1)
    //                          perror("Line C UI Message Queue");
    //                     type_C_pile[linenum][i]->progress = type_C_pile[linenum][i]->progress | (1<<index);
    //                     usleep(step_time);

    //                 }
    //                 printf("C : %X\n", type_C_pile[linenum][i]->progress);
    //                 if (type_C_pile[linenum][i]->progress == 0x1f){
    //                     printf("ready\n");
    //                     if (msgsnd(patcher_msgq_id, &buf, sizeof(buf), 0) == -1)
    //                         perror("Line C patcher Message Queue");
    //                     type_C_pile[linenum][i] = NULL;
    //                 }
    //                 pthread_mutex_unlock(&C_pile_mutex[linenum][i]);

    //             }
    //         } 
    //         else
    //         {

    //             if (pthread_mutex_trylock(&C_pile_mutex[linenum][i])){

    //                 buf.payload.id = type_C_pile[linenum][i]->id;
    //                 if (msgsnd(ui_msgq_id, &buf, sizeof(buf), 0) == -1)
    //                     perror("Line C UI Message Queue");
    //                 type_C_pile[linenum][i]->progress = type_C_pile[linenum][i]->progress | (1<<index);
    //                 usleep(step_time);

    //                 printf("C : %X\n", type_C_pile[linenum][i]->progress);
    //                 if (type_C_pile[linenum][i]->progress == 0x1f){
    //                     printf("ready\n");
    //                     if (msgsnd(patcher_msgq_id, &buf, sizeof(buf), 0) == -1)
    //                          perror("Line C Patcher Message Queue");
    //                     type_C_pile[linenum][i] = NULL;
    //                 }

    //                 pthread_mutex_unlock(&C_pile_mutex[linenum][i]);

    //             }

    //         }
    // }
    // i = (i+1)%PILESIZE;
    // j++;
    // if (j == 10000)
    //     break;
    // }
    // pthread_exit(0);

}

void join_all(){
    int i,j;
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

    pthread_join(generator,NULL);
}





/*void employeeDoStepPerLine_TypeB(void *line_number_as_index)
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
}*/

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

