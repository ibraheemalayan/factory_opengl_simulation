#include "../include.h"

void create_and_setup_message_queue();
void write_mock_messages();
void clean_queues();

int ui_msgq_id, children_msgq_id;

void create_and_setup_message_queue()
{

    key_t ui_queue_key, children_queue_key;

    // remove queue if exists
    remove("ui_queue.bin");

    // remove queue if exists
    remove("children_queue.bin");

    // create file to use as message queue key
    system("touch ui_queue.bin");
    system("chmod 666 ui_queue.bin");

    // create file to use as message queue key
    system("touch children_queue.bin");
    system("chmod 666 children_queue.bin");

    if ((ui_queue_key = ftok("ui_queue.bin", 30)) == -1)
    {
        perror("ftok");
        exit(1);
    }
    if ((children_queue_key = ftok("children_queue.bin", 30)) == -1)
    {
        perror("ftok");
        exit(1);
    }

    ui_msgq_id = msgget(children_queue_key, 0666 | IPC_CREAT);
    if (ui_msgq_id == -1)
    {
        perror("msgget ui queue");
        clean_queues();
        exit(2);
    }

    children_msgq_id = msgget(ui_queue_key, 0666 | IPC_CREAT);
    if (children_msgq_id == -1)
    {
        perror("msgget children queue");
        clean_queues();
        exit(2);
    }

    struct msqid_ds ui_queue_info;
    // read existing message queue info into ui_queue_info
    if (msgctl(ui_msgq_id, IPC_STAT, &ui_queue_info) == -1)
    {
        perror("Can not read message queue info");
        clean_queues();
        exit(5);
    }

    ui_queue_info.msg_qbytes = 20480;

    // increase buffer size
    msgctl(ui_msgq_id, IPC_SET, &ui_queue_info);

    struct msqid_ds children_queue_info;
    // read existing message queue info into ui_queue_info
    if (msgctl(children_msgq_id, IPC_STAT, &children_queue_info) == -1)
    {
        perror("Can not read message queue info");
        clean_queues();
        exit(5);
    }

    children_queue_info.msg_qbytes = 20480;

    // increase buffer size
    msgctl(children_msgq_id, IPC_SET, &children_queue_info);

    green_stdout();
    printf("Message queues have been created\n");
    reset_stdout();
}

void write_mock_messages()
{

    message_buf buf;

    buf.pid = 12L;

    buf.payload.current_location = MaleOutsideGatesArea;
    buf.payload.index_in_queue = 23;

    if (msgsnd(children_msgq_id, &buf, sizeof(buf) + sizeof(MsgType), 0) == -1)
    {
        perror("msgsnd");
        exit(4);
    }
}

void clean_queues()
{
    // remove the message queue from the System V IPC
    msgctl(children_msgq_id, IPC_RMID, NULL);

    // remove the queue file
    remove("children_queue.bin");
}

static void interrupt_sig_handler(int sig)
{
    write_mock_messages();
    red_stdout();
    printf("\n\nInterrupt signal received\n");
    // clean_queues();
    reset_stdout();
    // exit(0);
}

int main()
{

    create_and_setup_message_queue();

    // register signal handler for SIGINT to clean up
    signal(SIGINT, interrupt_sig_handler);

    // sleep(10);

    int i = 0;

    while (i < 30)
    {
        pause();
    }

    clean_queues();
    return 0;
}