#include "./structs.h"
#include "../include.h"
#include "./globals.h"
#include "./hash_table.h"
#include "./ui_helper.h"

HashTable *ht;

int msg_q_id;

void paint_and_swap_frame();
void background();
int read_message_queue(HashTable *ht);
void draw_items_in_queues();
void update_moving_items_locations();
void setup_message_queue();

void recursive_timed_update(int time);

// void draw_palestine_flag();
void validate_args(int argc, char *argv[]);

void setup_message_queue()
{

    key_t key;

    if ((key = ftok("ui_queue.bin", 30)) == -1)
    {
        perror("ftok, queue not found");
        exit(1);
    }

    msg_q_id = msgget(key, 0);
    if (msg_q_id == -1)
    {
        perror("msgget, error getting queue");
        exit(2);
    }
}

/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void paint_and_swap_frame()
{

    glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer (background)
    glLoadIdentity();             // reset disply

    // Draw Palestine flage
    // draw_palestine_flag();

    // Draw the queues
    draw_locations(locations_ptrs);

    draw_text();

    draw_walls();

    drawMetalDetector();

    draw_printer_animation(PRINTER_ANIMATION_X_VALUE, PRINTER_ANIMATION_Y_VALUE, male_rolling_gate_rotation);
    draw_printer_animation(PRINTER_ANIMATION_X_VALUE, -PRINTER_ANIMATION_Y_VALUE, -female_rolling_gate_rotation);

    draw_items_in_queues();

    glutSwapBuffers(); // Swap the buffers (replace current frame with the new one)
}

void validate_args(int argc, char *argv[])
{
}

void draw_items_in_queues()
{

    for (int i = 0; i < ht->size; i++)
    {
        if (ht->items[i])
        {
            draw_item(ht->items[i]->value);
            if (ht->overflow_buckets[i])
            {
                LinkedList *head = ht->overflow_buckets[i];
                while (head)
                {
                    draw_item(ht->items[i]->value);
                    head = head->next;
                }
            }
        }
    }
}
void update_moving_items_locations()
{
    return;
    for (int i = 0; i < ht->size; i++)
    {
        if (ht->items[i])
        {
            update_item_location(ht->items[i]->value);
            if (ht->overflow_buckets[i])
            {
                LinkedList *head = ht->overflow_buckets[i];
                while (head)
                {
                    update_item_location(ht->items[i]->value);
                    head = head->next;
                }
            }
        }
    }
}

int read_message_queue(HashTable *ht)
{

    return -1;

    //     message_buf message_queue_buffer;

    //     // msg_type set to Zero to read the first message in the queue regarless of it its type
    //     if (msgrcv(msg_q_id, &message_queue_buffer, sizeof(message_queue_buffer.payload), PERSON, IPC_NOWAIT | MSG_NOERROR) == -1)
    //     {
    //         if (errno == ENOMSG)
    //         {
    //             return -1;
    //         }

    //         // if the error is not ENOMSG, then it is an error
    //         perror("msgrcv");
    //         exit(3);
    //     }

    //     // printf("received message:\n\n");
    //     // print_message(&(message_queue_buffer.payload));

    //     if (message_queue_buffer.payload.msg_type == PersonEntered)
    //     {

    //         LocationObject *current_location = get_proper_queue_pointer(message_queue_buffer.payload.current_location);
    //         Person *p = create_person(
    //             message_queue_buffer.payload.person_pid,
    //             message_queue_buffer.payload.index_in_queue,
    //             message_queue_buffer.payload.gender,
    //             message_queue_buffer.payload.angriness,
    //             current_location);

    //         p->destination_coords = get_queue_location_coords_for_index(current_location, p->index_in_queue);

    //         ht_insert(ht, p->id, p);
    //     }
    //     else if (message_queue_buffer.payload.msg_type == PersonExitedUnserved)
    //     {
    //         Person *p = ht_search(ht, message_queue_buffer.payload.person_pid);
    //         p->destination_coords.y = 1000;
    //         p->angriess = 1;
    //     }
    //     else if (message_queue_buffer.payload.msg_type == PersonExitedSatisfied)
    //     {
    //         Person *p = ht_search(ht, message_queue_buffer.payload.person_pid);
    //         p->destination_coords.y = -1000;
    //         p->angriess = 0;
    //     }
    //     else if (message_queue_buffer.payload.msg_type == PersonExitedUnsatisfied)
    //     { // TODO color satisfied in green
    //         Person *p = ht_search(ht, message_queue_buffer.payload.person_pid);
    //         p->destination_coords.y = -1000;
    //         p->angriess = 1;
    //     }
    //     else if (message_queue_buffer.payload.msg_type == PersonUpdated)
    //     {
    //         Person *p = ht_search(ht, message_queue_buffer.payload.person_pid);
    //         LocationObject *current_location = get_proper_queue_pointer(message_queue_buffer.payload.current_location);

    //         if (p->current_location != current_location)
    //         {
    //             p->current_location->current_people--;
    //             p->current_location = current_location;
    //             current_location->current_people++;
    //         }
    //         p->index_in_queue = message_queue_buffer.payload.index_in_queue;

    //         p->destination_coords = get_queue_location_coords_for_index(current_location, p->index_in_queue);

    //         p->index_in_queue = message_queue_buffer.payload.index_in_queue;
    //         p->angriess = message_queue_buffer.payload.angriness;
    //     }

    //     return 1;
}

void recursive_timed_update(int time)
{
    if (!simulation_finished)
    {
        glutTimerFunc(1000 / FPS, recursive_timed_update, 0);
    }
    glutPostRedisplay(); // marks the current window as needing to be redisplayed

    while (read_message_queue(ht) != -1)
    {
        // keep reading messgaes until queue is empty
    }

    update_moving_items_locations();

    male_rolling_gate_rotation += ROLLING_CROSS_DEGREE_PER_FRAME;
    female_rolling_gate_rotation += ROLLING_CROSS_DEGREE_PER_FRAME;
}

void setup_ui(int argc, char **argv)
{

    glutInit(&argc, argv);                       // Initialize GLUT
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // for animation

    glutInitWindowSize(900, 450);                            // Set the window's initial width & height
    glutInitWindowPosition(0, 0);                            // Position the window's initial top-left corner of the screen
    glutCreateWindow("Chocolate Factory OpenGL Simulation"); // Create a window with the given title
    glutDisplayFunc(paint_and_swap_frame);                   // Register display callback handler for window re-paint
    glutReshapeFunc(reshape);                                // Static display
    glutTimerFunc(0, recursive_timed_update, 0);             // Call function after specified amount of time

    background(); // Background color
}

// void create_random_people()
// {

//     for (int i = 1; i < people_count; i++)
//     {

//         gender g = (rand() % 2) ? Male : Female;
//         LocationObject *q = (g == Male) ? queue_A1 : queue_A2;

//         Person *p = create_person(i, q->current_people, g, ((float)(rand() % 8)) * 0.1, q);

//         p->destination_coords = get_queue_location_coords_for_index(q, p->index_in_queue);

//         ht_insert(ht, p->id, p);

//         q->current_people++;
//     }
// }

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char **argv)
{

    printf("Starting UI...\n");

    srand(time(NULL)); // initialize random seed

    validate_args(argc, argv); // does nothing in standalone mode

    setup_ui(argc, argv);

    locations_ptrs[0] = NULL;  // location of undefined
    locations_ptrs[23] = NULL; // location of delivered

    for (int i = 0; i <= 22; i++)
    {
        locations_ptrs[i] = (LocationObject *)malloc(sizeof(LocationObject));
    }

    initialize_queues_coordinates(locations_ptrs);

    ht = create_table(CAPACITY);

    // setup_message_queue();

    glutMainLoop(); // Enter the event-processing loop

    return 0;
}