#ifndef __UI_HELPER_H_
#define __UI_HELPER_H_

#include <GL/glut.h> // GLUT, include glu.h and gl.h
#include "../include.h"
#include "../std.h"
#include "./structs.h"
#include "./globals.h"
#include "./constants.h"
#include "./shapes.h"

Item *create_item_obj(int id, int index, ItemType pkg_type, ChocolateType chocolate_type, LocationObject *current_location);
Coordinates get_queue_location_coords_for_index(LocationObject *queue, int index);
Coordinates get_queue_location_coords_for_next(LocationObject *queue);
void initialize_queues_coordinates(LocationObject *locations[]);
void update_item_location(Item *item);
void draw_items_in_queues();
LocationObject *get_proper_location_pointer(Location current_location);
void draw_locations(LocationObject *locations[]);

Item *create_item_obj(int id, int index, ItemType pkg_type, ChocolateType chocolate_type, LocationObject *current_location)
{
    Item *item = (Item *)malloc(sizeof(Item));

    item->id = id;
    item->index_in_queue = index;
    item->pkg_type = pkg_type;
    item->chocolate_type = chocolate_type;
    item->current_location = current_location;

    // FIXME
    item->current_coords.x = -1500;
    item->current_coords.y = 0;

    return item;
}

void initialize_queues_coordinates(LocationObject *locations[])
{

    locations[MANUFACTURING_LINE_A1]->coords.x = MANUFACTURING_LINE_X_VALUE;
    locations[MANUFACTURING_LINE_A1]->coords.y = A1_MANUFACTURING_LINE_Y_VALUE;
    locations[MANUFACTURING_LINE_A1]->width = A_MANUFACTURING_LINE_WIDTH;
    locations[MANUFACTURING_LINE_A1]->height = MANUFACTURING_LINE_HEGHIT;
    locations[MANUFACTURING_LINE_A1]->current_items = 0;

    locations[MANUFACTURING_LINE_A2]->coords.x = MANUFACTURING_LINE_X_VALUE;
    locations[MANUFACTURING_LINE_A2]->coords.y = A2_MANUFACTURING_LINE_Y_VALUE;
    locations[MANUFACTURING_LINE_A2]->width = A_MANUFACTURING_LINE_WIDTH;
    locations[MANUFACTURING_LINE_A2]->height = MANUFACTURING_LINE_HEGHIT;
    locations[MANUFACTURING_LINE_A2]->current_items = 0;

    locations[MANUFACTURING_LINE_A3]->coords.x = MANUFACTURING_LINE_X_VALUE;
    locations[MANUFACTURING_LINE_A3]->coords.y = A3_MANUFACTURING_LINE_Y_VALUE;
    locations[MANUFACTURING_LINE_A3]->width = A_MANUFACTURING_LINE_WIDTH;
    locations[MANUFACTURING_LINE_A3]->height = MANUFACTURING_LINE_HEGHIT;
    locations[MANUFACTURING_LINE_A3]->current_items = 0;

    locations[MANUFACTURING_LINE_B1]->coords.x = MANUFACTURING_LINE_X_VALUE;
    locations[MANUFACTURING_LINE_B1]->coords.y = B1_MANUFACTURING_LINE_Y_VALUE;
    locations[MANUFACTURING_LINE_B1]->width = B_MANUFACTURING_LINE_WIDTH;
    locations[MANUFACTURING_LINE_B1]->height = MANUFACTURING_LINE_HEGHIT;
    locations[MANUFACTURING_LINE_B1]->current_items = 0;

    locations[MANUFACTURING_LINE_B2]->coords.x = MANUFACTURING_LINE_X_VALUE;
    locations[MANUFACTURING_LINE_B2]->coords.y = B2_MANUFACTURING_LINE_Y_VALUE;
    locations[MANUFACTURING_LINE_B2]->width = B_MANUFACTURING_LINE_WIDTH;
    locations[MANUFACTURING_LINE_B2]->height = MANUFACTURING_LINE_HEGHIT;
    locations[MANUFACTURING_LINE_B2]->current_items = 0;

    locations[MANUFACTURING_LINE_C1]->coords.x = MANUFACTURING_LINE_X_VALUE;
    locations[MANUFACTURING_LINE_C1]->coords.y = C1_MANUFACTURING_LINE_Y_VALUE;
    locations[MANUFACTURING_LINE_C1]->width = C_MANUFACTURING_LINE_WIDTH;
    locations[MANUFACTURING_LINE_C1]->height = MANUFACTURING_LINE_HEGHIT;
    locations[MANUFACTURING_LINE_C1]->current_items = 0;

    locations[MANUFACTURING_LINE_C2]->coords.x = MANUFACTURING_LINE_X_VALUE;
    locations[MANUFACTURING_LINE_C2]->coords.y = C2_MANUFACTURING_LINE_Y_VALUE;
    locations[MANUFACTURING_LINE_C2]->width = C_MANUFACTURING_LINE_WIDTH;
    locations[MANUFACTURING_LINE_C2]->height = MANUFACTURING_LINE_HEGHIT;
    locations[MANUFACTURING_LINE_C2]->current_items = 0;

    locations[PATCHING_A]->coords.x = PATCHING_X_VALUE;
    locations[PATCHING_A]->coords.y = A_PATCHING_Y_VALUE;
    locations[PATCHING_A]->width = PATCHING_WIDTH;
    locations[PATCHING_A]->height = PATCHING_HEIGHT;
    locations[PATCHING_A]->current_items = 0;

    locations[PATCHING_B]->coords.x = PATCHING_X_VALUE;
    locations[PATCHING_B]->coords.y = B_PATCHING_Y_VALUE;
    locations[PATCHING_B]->width = PATCHING_WIDTH;
    locations[PATCHING_B]->height = PATCHING_HEIGHT;
    locations[PATCHING_B]->current_items = 0;

    locations[PATCHING_C]->coords.x = PATCHING_X_VALUE;
    locations[PATCHING_C]->coords.y = C_PATCHING_Y_VALUE;
    locations[PATCHING_C]->width = PATCHING_WIDTH;
    locations[PATCHING_C]->height = PATCHING_HEIGHT;
    locations[PATCHING_C]->current_items = 0;
}

Coordinates get_queue_location_coords_for_next(LocationObject *location)
{
    return get_queue_location_coords_for_index(location, location->current_items);
}

Coordinates get_queue_location_coords_for_index(LocationObject *queue, int index)
{

    int max_objects_per_row = queue->width / PADDING_BETWEEN_OBJECTS;
    int row = index / max_objects_per_row;
    int column = index % max_objects_per_row;

    Coordinates coords;

    coords.y = queue->coords.y - 30 + queue->height - row * PADDING_BETWEEN_OBJECTS;
    coords.x = queue->coords.x - 30 + queue->width - column * PADDING_BETWEEN_OBJECTS;

    return coords;
}

void update_item_location(Item *item)
{

    // update X coordinates
    if (item->current_coords.x != item->destination_coords.x)
    {

        float remaing_distance = item->current_coords.x - item->destination_coords.x;

        if ((remaing_distance < STEP_SIZE && remaing_distance > 0) || (remaing_distance > -STEP_SIZE && remaing_distance < 0))
        {
            item->current_coords.x = item->destination_coords.x;
        }
        else
        {
            item->current_coords.x += (remaing_distance > 0) ? -STEP_SIZE : STEP_SIZE;
        }
    }
    // update Y coordinates
    if (item->current_coords.y != item->destination_coords.y)
    {
        float remaing_distance = item->current_coords.y - item->destination_coords.y;

        if ((remaing_distance < STEP_SIZE / 2 && remaing_distance > 0) || (remaing_distance > -STEP_SIZE / 2 && remaing_distance < 0))
        {
            item->current_coords.y = item->destination_coords.y;
        }
        else
        {
            item->current_coords.y += (remaing_distance > 0) ? -STEP_SIZE / 2 : STEP_SIZE / 2;
        }
    }
}

void draw_locations(LocationObject *locations[])
{
    int R, G, B;

    for (int i = 1; i <= 22; i++)
    {
        draw_rectangle(locations[i]->coords.x, locations[i]->coords.y, locations[i]->width, locations[i]->height, 220, 220, 220);
        if (i == 3)
        {
            R = 220;
            G = 220;
            B = 220;
        }
    }
}

void print_message(message_payload *buf)
{
    printf("{\n\tmsg_type: %ld", buf->msg_type);
    printf("\n\titem_id: %d", buf->id);
    printf("\n\tchocolate_type: %d", buf->chocolate_type);
    printf("\n\titem_type: %d", buf->item_type);
    printf("\n\tindex_in_queue: %d", buf->index);
    printf("\n\tcurrent_location: %i\n}\n", buf->current_location);
}

LocationObject *get_proper_location_pointer(Location current_location)
{

    // e.g returns manufacturing_line_a1_location_obj for MANUFACTURING_LINE_A1

    LocationObject *ptr = locations_ptrs[current_location];

    if (ptr == NULL)
    {
        red_stdout();
        printf("ERROR: get_proper_location_pointer() - returned a wrong location - %d\n", current_location);
        reset_stdout();

        return NULL;
    }

    return ptr;
}

#endif
