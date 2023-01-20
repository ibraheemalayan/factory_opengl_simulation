#ifndef __UI_STRUCTS_H_
#define __UI_STRUCTS_H_

#include "../include.h"

struct Coordinations
{
    float x;
    float y;
};

typedef struct Coordinations Coordinates;

struct LocationObject
{
    Coordinates coords;
    int current_items;
    float width;
    float height;
};

typedef struct LocationObject LocationObject;

struct ItemObj
{
    int id;
    ItemType pkg_type;
    ChocolateType chocolate_type;

    Coordinates current_coords;
    Coordinates destination_coords;

    int index_in_queue;
    LocationObject *current_location;
    Location location_index;
};
typedef struct ItemObj ItemObj;

void print_item(ItemObj *item)
{

    printf("{ id: %d, pkg_type: %d, chocolate_type: %d, index: %d, current_location: %d, q: %p, current_coords: (x: %f, y: %f), , dest_coords: (x: %f, y: %f)\n",
           item->id,
           item->pkg_type,
           item->chocolate_type,
           item->index_in_queue,
           item->location_index,
           item->current_location,
           item->current_coords.x,
           item->current_coords.y,
           item->destination_coords.x,
           item->destination_coords.y);
}

#endif