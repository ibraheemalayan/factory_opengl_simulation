#include "./structs.h"
#include "../include.h"
#include "./globals.h"
#include "./hash_table.h"
#include "./ui_helper.h"

HashTable *ht;

int msg_q_id;

void paint_and_swap_frame();
void background();
int read_and_handle_msg_queue(HashTable *ht);
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

    draw_teller_text();

    draw_walls();

    drawMetalDetector();

    draw_rolling_gate(ROLLING_GATES_X, ROLLING_GATES_Y, male_rolling_gate_rotation);
    draw_rolling_gate(ROLLING_GATES_X, -ROLLING_GATES_Y, female_rolling_gate_rotation);

    // draw_rolling_gate(0, -100);

    draw_items_in_queues();

    draw_truck(PARKED_TRUCK_X + truck_1_x_distance, PARKED_TRUCK_1_Y - TRUCK_QUEUE_ROAD_Y_OFFSET);
    draw_truck(PARKED_TRUCK_X + truck_2_x_distance, PARKED_TRUCK_2_Y - TRUCK_QUEUE_ROAD_Y_OFFSET);
    draw_truck(PARKED_TRUCK_X + truck_3_x_distance, PARKED_TRUCK_3_Y - TRUCK_QUEUE_ROAD_Y_OFFSET);

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

int read_and_handle_msg_queue(HashTable *ht)
{

    message_buf message_queue_buffer;

    // msg_type set to Zero to read the first message in the queue regarless of it its type
    if (msgrcv(msg_q_id, &message_queue_buffer, sizeof(message_queue_buffer.payload), PERSON, IPC_NOWAIT | MSG_NOERROR) == -1)
    {
        if (errno == ENOMSG)
        {
            return -1;
        }

        // if the error is not ENOMSG, then it is an error
        perror("msgrcv");
        exit(3);
    }

    // printf("received message:\n\n");
    // print_message(&(message_queue_buffer.payload));

    if (message_queue_buffer.payload.msg_type == OBJECT_CREATED)
    {

        LocationObject *current_location = locations_ptrs[message_queue_buffer.payload.current_location];

        ItemObj *it = create_item_obj(
            message_queue_buffer.payload.id,
            message_queue_buffer.payload.index,
            message_queue_buffer.payload.item_type,
            message_queue_buffer.payload.chocolate_type,
            current_location);

        it->destination_coords = get_queue_location_coords_for_index(current_location, it->index_in_queue);

        ht_insert(ht, it->id, it);

        if (message_queue_buffer.payload.item_type != PRODUCT)
        {
            int num_of_ids = (message_queue_buffer.payload.item_type == CARTON_BOX) ? 2 : 10;
            for (int i = 0; i < num_of_ids; i++)
            {
                ht_delete(ht, message_queue_buffer.payload.ids_to_delete[i]);
            }
        }
    }
    else if (message_queue_buffer.payload.msg_type == TRUCK_RETURNED)
    {
        if (message_queue_buffer.payload.index == 1)
        {
            truck_1_x_destintation = PARKED_TRUCK_X_DISTANCE;
        }
        else if (message_queue_buffer.payload.index == 2)
        {
            truck_2_x_destintation = PARKED_TRUCK_X_DISTANCE;
        }
        else
        {
            truck_3_x_destintation = PARKED_TRUCK_X_DISTANCE;
        }
    }
    else if (message_queue_buffer.payload.msg_type == TRUCK_LEFT)
    {

        for (int i = 0; i < MAX_BOXES_PER_TRUCK; i++)
        {
            if (message_queue_buffer.payload.ids_to_delete[i] == 0)
            {
                continue;
            }
            ht_delete(ht, message_queue_buffer.payload.ids_to_delete[i]);
        }

        if (message_queue_buffer.payload.index == 1)
        {
            truck_1_x_destintation = LEFT_TRUCK_X_DISTANCE;
        }
        else if (message_queue_buffer.payload.index == 2)
        {
            truck_2_x_destintation = LEFT_TRUCK_X_DISTANCE;
        }
        else
        {
            truck_3_x_destintation = LEFT_TRUCK_X_DISTANCE;
        }
    }
    else if (message_queue_buffer.payload.msg_type == OBJECT_MOVED)
    {
        ItemObj *it = ht_search(ht, message_queue_buffer.payload.id);
        LocationObject *current_location = locations_ptrs[message_queue_buffer.payload.current_location];

        if (it->current_location != current_location)
        {
            it->current_location->current_items--;
            it->current_location = current_location;
            current_location->current_items++;

            if (message_queue_buffer.payload.current_location >= TRUCK_1 || message_queue_buffer.payload.current_location <= TRUCK_3)
            {
                it->index_in_queue = 0;
            }
        }
        it->index_in_queue = message_queue_buffer.payload.index;

        it->destination_coords = get_queue_location_coords_for_index(current_location, it->index_in_queue);

        it->index_in_queue = message_queue_buffer.payload.index;
    }

    return 1;
}

void recursive_timed_update(int time)
{
    if (!simulation_finished)
    {
        glutTimerFunc(1000 / FPS, recursive_timed_update, 0);
    }
    glutPostRedisplay(); // marks the current window as needing to be redisplayed

    while (read_and_handle_msg_queue(ht) != -1)
    {
        // keep reading messgaes until queue is empty
    }

    update_moving_items_locations();
    update_truck_locations();

    male_rolling_gate_rotation += ROLLING_GATE_DEGREE_PER_FRAME;
    female_rolling_gate_rotation += ROLLING_GATE_DEGREE_PER_FRAME;
}

void setup_ui(int argc, char **argv)
{

    glutInit(&argc, argv);                       // Initialize GLUT
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // for animation

    glutInitWindowSize(900, 450);                // Set the window's initial width & height
    glutInitWindowPosition(0, 0);                // Position the window's initial top-left corner of the screen
    glutCreateWindow("OIM Simulation");          // Create a window with the given title
    glutDisplayFunc(paint_and_swap_frame);       // Register display callback handler for window re-paint
    glutReshapeFunc(reshape);                    // Static display
    glutTimerFunc(0, recursive_timed_update, 0); // Call function after specified amount of time

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

            rand_queue_index = random_int_in_range(1, 7);
        }
        else if (p_type == PATCH)
        {
            rand_queue_index = random_int_in_range(8, 17);
        }
        else // if (p_type == BOX)
        {
            rand_queue_index = random_int_in_range(18, 22);
        }

        LocationObject *q = locations[rand_queue_index];

        ItemObj *item_obj = create_item_obj(i, i % 2, p_type, c_type, q);

        item_obj->destination_coords = get_queue_location_coords_for_index(q, item_obj->index_in_queue);

        // print_item(item_obj);

        ht_insert(ht, item_obj->id, item_obj);

        q->current_items++;
    }
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char **argv)
{

    printf("Starting UI...\n");

    srand(time(NULL)); // initialize random seed

    validate_args(argc, argv); // does nothing in standalone mode

    setup_ui(argc, argv);

    locations_ptrs[0] = NULL;
    locations_ptrs[23] = NULL;

    for (int i = 0; i <= 22; i++)
    {
        locations_ptrs[i] = (LocationObject *)malloc(sizeof(LocationObject));
    }

    initialize_queues_coordinates(locations_ptrs);

    ht = create_table(CAPACITY);

    create_random_items(locations_ptrs);

    setup_message_queue();

    glutMainLoop(); // Enter the event-processing loop

    return 0;
}