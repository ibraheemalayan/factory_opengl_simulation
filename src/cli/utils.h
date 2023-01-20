#ifndef __UTILS_H_
#define __UTILS_H_

#include "../include.h"
#include "globals.c"

pid_t gui_pid = 0;
int ui_msgq_id;
int patcher_msgq_id;
int printer_msgq_id;

void run_gui();                                                          // runs the gui
void create_and_setup_message_queues();                                  // creates the message queue
void clean_up();                                                         // cleans up the message queues, and kills the gui and all children
void interrupt_sig_handler(int sig);                                     // signal handler for interrupts, to clean up resources
void generate_product(int empty_index, ChocolateType type, int linenum); // generates a product

int count = 0;
int start_flag = 0;

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
pthread_mutex_t A_pile_mutex[3][PILESIZE];
pthread_mutex_t B_pile_mutex[2][PILESIZE];
pthread_mutex_t C_pile_mutex[2][PILESIZE];
pthread_mutex_t patch_mutex_A;
pthread_mutex_t patch_mutex_B;
pthread_mutex_t patch_mutex_C;
pthread_mutex_t id_mutex;
// Arrays of mutex //Steps 1 to 6 have to happen in order

int generate_uniq_id()
{
    int temp;
    pthread_mutex_lock(&id_mutex);
    temp = id_counter;
    id_counter += 1;
    pthread_mutex_unlock(&id_mutex);

    return temp;
}

void run_gui()
{

    gui_pid = fork();
    if (gui_pid == -1)
    {
        printf("fork gui failure\n");
        clean_up();
        exit(-4);
    }
    else if (gui_pid == 0)
    {
        execlp("./bin/gui.o", "gui.o", NULL);
        perror("\n> gui: exec failure\n");
        clean_up();
        exit(-2);
    }
}

void create_and_setup_message_queues()
{

    key_t ui_queue_key, patcher_queue_key, printer_queue_key;

    // remove queue if exists
    remove("ui_queue.bin");

    // create file to use as message queue key
    system("touch ui_queue.bin");
    system("chmod 666 ui_queue.bin");

    if ((ui_queue_key = ftok("ui_queue.bin", 30)) == -1)
    {
        perror("ftok");
        clean_up();
        exit(1);
    }

    if ((patcher_queue_key = ftok("ui_queue.bin", 30)) == -1)
    {
        perror("ftok");
        clean_up();
        exit(1);
    }

    if ((printer_queue_key = ftok("ui_queue.bin", 30)) == -1)
    {
        perror("ftok");
        clean_up();
        exit(1);
    }

    ui_msgq_id = msgget(ui_queue_key, 0666 | IPC_CREAT);
    if (ui_msgq_id == -1)
    {
        perror("msgget ui queue");
        clean_up();
        exit(2);
    }

    patcher_msgq_id = msgget(patcher_queue_key, 0666 | IPC_CREAT);
    if (patcher_queue_key == -1)
    {
        perror("msgget ui queue");
        clean_up();
        exit(2);
    }

    printer_msgq_id = msgget(printer_queue_key, 0666 | IPC_CREAT);
    if (printer_queue_key == -1)
    {
        perror("msgget ui queue");
        clean_up();
        exit(2);
    }

    struct msqid_ds ui_queue_info;
    // read existing message queue info into ui_queue_info
    if (msgctl(ui_msgq_id, IPC_STAT, &ui_queue_info) == -1)
    {
        perror("Can not read message queue info");
        clean_up();
        exit(5);
    }

    ui_queue_info.msg_qbytes = 20480;

    // increase buffer size
    msgctl(ui_msgq_id, IPC_SET, &ui_queue_info);

    struct msqid_ds patcher_queue_info;
    // read existing message queue info into ui_queue_info
    if (msgctl(patcher_msgq_id, IPC_STAT, &patcher_queue_info) == -1)
    {
        perror("Can not read message queue info");
        clean_up();
        exit(5);
    }

    patcher_queue_info.msg_qbytes = 20480;

    // increase buffer size
    msgctl(patcher_msgq_id, IPC_SET, &patcher_queue_info);

    struct msqid_ds printer_queue_info;
    // read existing message queue info into ui_queue_info
    if (msgctl(printer_msgq_id, IPC_STAT, &printer_queue_info) == -1)
    {
        perror("Can not read message queue info");
        clean_up();
        exit(5);
    }

    printer_queue_info.msg_qbytes = 20480;

    // increase buffer size
    msgctl(printer_msgq_id, IPC_SET, &printer_queue_info);

    green_stdout();
    printf("UI message queues have been created\n");
    reset_stdout();
}

void clean_up()
{

    if (gui_pid != 0)
    {
        kill(gui_pid, SIGKILL);
        waitpid(gui_pid, NULL, 0);
    }

    // TODO kill all threads

    // remove the message queue from the System V IPC
    msgctl(ui_msgq_id, IPC_RMID, NULL);

    msgctl(patcher_msgq_id, IPC_RMID, NULL);

    msgctl(printer_msgq_id, IPC_RMID, NULL);

    // remove the queue file
    remove("ui_queue.bin");
}

void interrupt_sig_handler(int sig)
{
    red_stdout();
    printf("\n\nInterrupt signal received, cleaning up queues.\n");
    clean_up();
    reset_stdout();
    exit(0);
}

int randomIntegerInRange(int lower, int upper)
{
    return (rand() % (upper - lower + 1)) + lower;
}
// returns empty index if found, else returns PILESIZE
int array_full(chocolateProduct arr[])
{
    printf("called");
    for (int i = 0; i < PILESIZE; i++)
    {
        if (arr[i].id == 0)
            return i;
        printf("id = %d\n", arr[i].id);
    }
    return PILESIZE;
}

int find_product(chocolateProduct arr[], int start)
{
    for (int i = start; i < PILESIZE; i++)
    {
        if (arr[i].id != 0)
            return i;
    }
    return PILESIZE;
}

void generate_product(int empty_index, ChocolateType type, int linenum)
{
    char progress[8] = "00000000";

    chocolateProduct *array_ptr;

    switch (type)
    {
    case TYPE_A:
        array_ptr = type_A_pile[linenum];
        break;
    case TYPE_B:
        array_ptr = type_B_pile[linenum];
        break;
    case TYPE_C:
        array_ptr = type_C_pile[linenum];
        break;
    default:
        perror("UNEXCPECTED: PRODUCT GENERATOR");
        break;
    }

    array_ptr[empty_index].id = generate_uniq_id();

    snprintf(array_ptr[empty_index].progress, 8, "%s", progress);
    array_ptr[empty_index].type = type;
}

#endif