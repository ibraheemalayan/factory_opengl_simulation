#ifndef __UTILS_H_
#define __UTILS_H_

#include "../include.h"
#include "globals.c"

pid_t gui_pid = 0;
int ui_msgq_id;

void run_gui();                         // runs the gui
void create_and_setup_message_queues(); // creates the message queue
void clean_up();                        // cleans up the message queues, and kills the gui and all children
void interrupt_sig_handler(int sig);    // signal handler for interrupts, to clean up resources

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

    key_t ui_queue_key;

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

    ui_msgq_id = msgget(ui_queue_key, 0666 | IPC_CREAT);
    if (ui_msgq_id == -1)
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
    srand(time(NULL)); // randomize seed
    return (rand() % (upper - lower + 1)) + lower;
}
// returns empty index if found, else returns PILESIZE
int array_full(chocolateProduct *arr[]){
    for(int i = 0; i < PILESIZE; i++){
        if (arr[i] == NULL)
            return i;
    }
    return PILESIZE;
}


void generate_product(int empty_index, char type, int linenum){
    srand(time(NULL));
    chocolateProduct *temp;
    temp = (chocolateProduct *)malloc(sizeof(chocolateProduct));

    switch(type){
        case 'a' : 
            temp->type =TYPE_A;
            break;
        case 'b' : 
            temp->type =TYPE_B;
            break;
        case 'c' : 
            temp->type =TYPE_C;
            break;
    }

    id_counter+=(rand()%100);
    temp->id = id_counter;
    temp->progress = 0;
    printf("progress : %X\tID : %d\ttype : %d\n", temp->progress, id_counter, temp->type);


    if (type == 'a'){
        // type_A_pile[linenum][empty_index]->type = temp->type;
        // type_A_pile[linenum][empty_index]->id = temp->id;
        // type_A_pile[linenum][empty_index]->progress = temp->progress;
    }
}

#endif