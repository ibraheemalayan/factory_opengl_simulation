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

    locations[PRINTER]->coords.x = PRINTER_X_VALUE;
    locations[PRINTER]->coords.y = PRINTER_Y_VALUE;
    locations[PRINTER]->width = PRINTER_WIDTH;
    locations[PRINTER]->height = PRINTER_HEIGHT;
    locations[PRINTER]->current_items = 0;

    locations[CONTAINER_A]->coords.x = CONTAINER_X_VALUE;
    locations[CONTAINER_A]->coords.y = A_CONTAINER_Y_VALUE;
    locations[CONTAINER_A]->width = CONTAINER_WIDTH;
    locations[CONTAINER_A]->height = CONTAINER_HEIGHT;
    locations[CONTAINER_A]->current_items = 0;

    locations[CONTAINER_B]->coords.x = CONTAINER_X_VALUE;
    locations[CONTAINER_B]->coords.y = B_CONTAINER_Y_VALUE;
    locations[CONTAINER_B]->width = CONTAINER_WIDTH;
    locations[CONTAINER_B]->height = CONTAINER_HEIGHT;
    locations[CONTAINER_B]->current_items = 0;

    locations[CONTAINER_C]->coords.x = CONTAINER_X_VALUE;
    locations[CONTAINER_C]->coords.y = C_CONTAINER_Y_VALUE;
    locations[CONTAINER_C]->width = CONTAINER_WIDTH;
    locations[CONTAINER_C]->height = CONTAINER_HEIGHT;
    locations[CONTAINER_C]->current_items = 0;

    locations[STORAGE_AREA]->coords.x = STORAGE_X_VALUE;
    locations[STORAGE_AREA]->coords.y = STORAGE_Y_VALUE;
    locations[STORAGE_AREA]->width = STORAGE_WIDTH;
    locations[STORAGE_AREA]->height = STORAGE_HEIGHT;
    locations[STORAGE_AREA]->current_items = 0;

    locations[TRUCK_1]->coords.x = TRUCK_ROAD_X_VALUE;
    locations[TRUCK_1]->coords.y = TRUCK_1_ROAD_Y_VALUE;
    locations[TRUCK_1]->width = TRUCK_PARKING_WIDTH;
    locations[TRUCK_1]->height = TRUCK_PARKING_HEIGHT;
    locations[TRUCK_1]->current_items = 0;

    locations[TRUCK_2]->coords.x = TRUCK_ROAD_X_VALUE;
    locations[TRUCK_2]->coords.y = TRUCK_2_ROAD_Y_VALUE;
    locations[TRUCK_2]->width = TRUCK_PARKING_WIDTH;
    locations[TRUCK_2]->height = TRUCK_PARKING_HEIGHT;
    locations[TRUCK_2]->current_items = 0;

    locations[TRUCK_3]->coords.x = TRUCK_ROAD_X_VALUE;
    locations[TRUCK_3]->coords.y = TRUCK_3_ROAD_Y_VALUE;
    locations[TRUCK_3]->width = TRUCK_PARKING_WIDTH;
    locations[TRUCK_3]->height = TRUCK_PARKING_HEIGHT;
    locations[TRUCK_3]->current_items = 0;
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

void update_truck_locations()
{

    // update truck 1 x coordinates
    if (truck_1_x_distance != truck_1_x_destintation)
    {

        float remaing_distance = truck_1_x_distance - truck_1_x_destintation;

        if ((remaing_distance < TRUCK_STEP_SIZE && remaing_distance > 0) || (remaing_distance > -TRUCK_STEP_SIZE && remaing_distance < 0))
        {
            truck_1_x_distance = truck_1_x_destintation;
        }
        else
        {
            truck_1_x_distance += (remaing_distance > 0) ? -TRUCK_STEP_SIZE : TRUCK_STEP_SIZE;
        }
    }

    // update truck 2 x coordinates
    if (truck_2_x_distance != truck_2_x_destintation)
    {

        float remaing_distance = truck_2_x_distance - truck_2_x_destintation;

        if ((remaing_distance < TRUCK_STEP_SIZE && remaing_distance > 0) || (remaing_distance > -TRUCK_STEP_SIZE && remaing_distance < 0))
        {
            truck_2_x_distance = truck_2_x_destintation;
        }
        else
        {
            truck_2_x_distance += (remaing_distance > 0) ? -TRUCK_STEP_SIZE : TRUCK_STEP_SIZE;
        }
    }

    // update truck 3 x coordinates
    if (truck_3_x_distance != truck_3_x_destintation)
    {

        float remaing_distance = truck_3_x_distance - truck_3_x_destintation;

        if ((remaing_distance < TRUCK_STEP_SIZE && remaing_distance > 0) || (remaing_distance > -TRUCK_STEP_SIZE && remaing_distance < 0))
        {
            truck_3_x_distance = truck_3_x_destintation;
        }
        else
        {
            truck_3_x_distance += (remaing_distance > 0) ? -TRUCK_STEP_SIZE : TRUCK_STEP_SIZE;
        }
    }
}

void draw_locations(LocationObject *locations[])
{
    int R, G, B;

    for (int i = 1; i <= 22; i++)
    {
        if (i >= TRUCK_1 && i <= TRUCK_3)
        {
            draw_rectangle(locations[i]->coords.x + TRUCK_QUEUE_ROAD_X_OFFSET, locations[i]->coords.y + TRUCK_QUEUE_ROAD_Y_OFFSET, locations[i]->width, locations[i]->height, 220, 220, 220);
            continue;
        }
        draw_rectangle(locations[i]->coords.x, locations[i]->coords.y, locations[i]->width, locations[i]->height, 220, 220, 220);
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
