#include "local.h"
#include "../include.h"

//............Functions..............
int randomIntegerInRange(int lower, int upper);
void register_signal_handlers();
void handle_officer_order_signal(int the_sig);
void handle_alarm_signal(int the_sig);
void printInfoForChildPerson(char *argv[]);
void setup_message_queue();
void validate_args(int argc, char *argv[]);
void update_ui(MsgType msg_type);

#define MAX_PATIENCE_TIME 200
#define MIN_PATIENCE_TIME 50
#define PATIENCE_STEP 2
#define FIRST_ALARM_FACTOR 1000   // 10 ms
#define ALARM_INTERVAL 400 * 1000 // 4000 ms

#define init_random() (srand(time(NULL) ^ (getpid() << 16)))

int ui_msgq_id;
int parent_msgq_id;

float my_max_patience;
float waited_time = 0.0f;

pid_t my_pid;

int left = 0;

int index_in_queue; // index if inside a queue
Location current_location;
gender my_gender;
DocumentType doc_type;

void validate_args(int argc, char *argv[])
{
    if (argc != 4)
    {
        errno = EINVAL;
        red_stderr();
        printf("\nMust pass three arguments: gender, offical_document_needed, index, instead of %d", argc);
        reset_stderr();
        exit(-1);
    }

    // the first argument is -1 if last child
    if (!(my_gender = atoi(argv[1])) || (my_gender != Male && my_gender != Female))
    {
        errno = EINVAL;
        red_stdout();
        printf("\nThe first argument must be the gender which can be 1 or 2 not %s", argv[1]);
        fflush(stdout);
        reset_stdout();
        exit(-1);
    };

    current_location = (my_gender == Male) ? MaleOutsideGatesArea : FemaleOutsideGatesArea;

    doc_type = atoi(argv[2]);

    if (doc_type < 0 || doc_type > 3)
    {
        errno = EINVAL;
        red_stdout();
        printf("\nThe second argument must be the document type which can be in the range 0 and 3 instead of %s", argv[2]);
        fflush(stdout);
        reset_stdout();
        exit(-1);
    };

    if (!(index_in_queue = atoi(argv[3])))
    {
        errno = EINVAL;
        red_stdout();
        printf("\nThe third argument must be an integer, not %s", argv[3]);
        fflush(stdout);
        reset_stdout();
        exit(-1);
    };

    index_in_queue--;
}

void main(int argc, char *argv[])
{
    init_random();

    validate_args(argc, argv);

    my_pid = getpid();

    my_max_patience = randomIntegerInRange(MIN_PATIENCE_TIME, MAX_PATIENCE_TIME);

    printInfoForChildPerson(argv);

    register_signal_handlers();

    setup_message_queue();

    update_ui(PersonEntered);

    // Set an alarm to go off in VALUE microseconds. every INTERVAL microseconds.

    ualarm(FIRST_ALARM_FACTOR * my_max_patience, ALARM_INTERVAL);

    while (1)
    {
        pause();
    }
}

void register_signal_handlers()
{
    // register handler for SIGUSR1, which is sent by the parent process when it wants to change the index of the child
    if (sigset(SIGUSR1, handle_officer_order_signal) == -1 || sigset(SIGUSR2, handle_officer_order_signal) == -1)
    {
        perror("Sigset can not set SIGUSR1 or SIGUSR2");
        exit(SIGQUIT);
    }

    // register handler for SIGUSR1, which is sent by the parent process when it wants to change the index of the child
    if (sigset(SIGALRM, handle_alarm_signal) == -1)
    {
        perror("Sigset can not set SIGALRM");
        exit(SIGQUIT);
    }
}

void setup_message_queue()
{

    key_t ui_q_key, parent_q_key;

    if ((ui_q_key = ftok("ui_queue.bin", 30)) == -1)
    {
        perror("ftok, queue not found");
        exit(1);
    }

    ui_msgq_id = msgget(ui_q_key, 0);
    if (ui_msgq_id == -1)
    {
        perror("msgget, error getting queue");
        exit(2);
    }

    if ((parent_q_key = ftok("children_queue.bin", 30)) == -1)
    {
        perror("ftok, queue not found");
        exit(1);
    }

    parent_msgq_id = msgget(parent_q_key, 0);
    if (parent_msgq_id == -1)
    {
        perror("msgget, error getting queue");
        exit(2);
    }
}

void handle_officer_order_signal(int the_sig)
{
    parent_message_buf message_queue_buffer;

    int retries = 0;

    while (1)
    {
        // Try to receive message
        errno = 0;
        int status = msgrcv(parent_msgq_id, &message_queue_buffer, sizeof(message_queue_buffer.payload), (long)my_pid, IPC_NOWAIT | MSG_NOERROR);

        if (status < 0)
        {
            if (errno == EINTR)
            {
                usleep(100);
                continue; // try again
            }

            if (errno == ENOMSG)
            {
                if (retries++ < 10)
                {
                    usleep(1000);
                    continue; // try again
                }

                red_stdout();
                printf("No message of type %d in the queue, although the parent sent a signal", my_pid);
                reset_stdout();
                return;
                // exit(2); // FIXME
            }

            // if the error is not ENOMSG, then it is an error
            perror("msgrcv in child");
            exit(3);
        }
        break;
    }

    // green_stdout();
    // printf("A message of type %d was received", my_pid);
    // reset_stdout();

    index_in_queue = message_queue_buffer.payload.index_in_queue;

    if (current_location > message_queue_buffer.payload.current_location)
    {
        return;
    }

    current_location = message_queue_buffer.payload.current_location;

    if (current_location == LeaveHappy)
    {
        red_stdout();
        printf("\n\nPerson %d has left happy ", my_pid);
        reset_stdout();

        update_ui(PersonExitedSatisfied);

        exit(0);
    }

    if (current_location == LeaveUnhappy)
    {
        red_stdout();
        printf("\n\nPerson %d has left the teller unsatisfied", my_pid);
        reset_stdout();

        update_ui(PersonExitedUnsatisfied);

        exit(0);
    }

    update_ui(PersonUpdated);
}

void handle_alarm_signal(int the_sig)
{
    waited_time += PATIENCE_STEP;

    if (waited_time >= my_max_patience)
    {
        red_stdout();
        printf("Person %d has left the queue because he waited too long", my_pid);
        reset_stdout();

        update_ui(PersonExitedUnserved);

        exit(0);
    }

    update_ui(PersonUpdated);
}

int randomIntegerInRange(int lower, int upper)
{
    return (rand() % (upper - lower + 1)) + lower;
}

void printInfoForChildPerson(char *argv[])
{
    printf(
        "person{pid:%d, gender:%d, index:%d, document:%d, patience:%f}\n", getpid(), my_gender, index_in_queue, doc_type, my_max_patience);
    fflush(stdout);
}

void update_ui(MsgType msg_type)
{

    message_buf buf;

    buf.mtype = MTYPE;

    buf.payload.msg_type = msg_type;
    buf.payload.person_pid = my_pid;
    buf.payload.angriness = waited_time / my_max_patience;
    buf.payload.gender = my_gender;
    buf.payload.index_in_queue = index_in_queue;
    buf.payload.current_location = current_location;

    if (msgsnd(ui_msgq_id, &buf, sizeof(buf), 0) == -1)
    {
        perror("ui msgsnd in child");
        exit(3);
    }
}