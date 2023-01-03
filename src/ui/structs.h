#ifndef __UI_STRUCTS_H_
#define __UI_STRUCTS_H_

#include "../include.h"

struct Coordinations
{
    float x;
    float y;
};

typedef struct Coordinations Coordinates;

struct Queue
{
    Coordinates coords;
    int current_people;
    float width;
    float height;
};

typedef struct Queue Queue;

struct Person
{
    int id;
    Coordinates current_coords;
    Coordinates destination_coords;
    float angriess; // current waited time out of max patience
    float speed;
    gender gender;
    int index_in_queue;
    Queue *current_queue;
};
typedef struct Person Person;

void print_person(Person *person)
{
    printf("{ id: %d, gender: %d, index: %d, q: %p, anger: %f, current_coords: (x: %f, y: %f), , dest_coords: (x: %f, y: %f)\n",
           person->id,
           person->gender,
           person->index_in_queue,
           person->current_queue,
           person->angriess,
           person->current_coords.x,
           person->current_coords.y,
           person->destination_coords.x,
           person->destination_coords.y);
}

#endif
