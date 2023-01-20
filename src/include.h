
/*
 * local header file for pipe client-server
 */

#ifndef __INCLUDE_H_
#define __INCLUDE_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <sys/msg.h>

#include "std.h"

#define UISIG 7
#define MTYPE 1L
#define PILESIZE 10

#define MAX_BOXES_PER_TRUCK 18 // keep it even number

enum ItemType
{
  PRODUCT = 1,
  PATCH,
  CARTON_BOX,
};

enum ChocolateType
{
  TYPE_A = 1,
  TYPE_B,
  TYPE_C,

};

// the order of the enum values is important !
// ! DO NOT CHANGE THE ORDER !
enum Location
{
  UNDEFINED = 0,

  // manufacturing lines
  MANUFACTURING_LINE_A1,
  MANUFACTURING_LINE_A2,
  MANUFACTURING_LINE_A3,

  MANUFACTURING_LINE_B1,
  MANUFACTURING_LINE_B2,

  MANUFACTURING_LINE_C1,
  MANUFACTURING_LINE_C2,

  // patching
  PATCHING_A,
  PATCHING_B,
  PATCHING_C,

  // printing
  PRINTER,

  // containers
  CONTAINER_A,
  CONTAINER_B,
  CONTAINER_C,

  // carton boxes
  CARTON_BOX_A,
  CARTON_BOX_B,
  CARTON_BOX_C,

  // storage
  STORAGE_AREA,

  // trucks
  TRUCK_1,
  TRUCK_2,
  TRUCK_3,

  // removed from the screen
  DELIVERED

};

typedef enum Location Location;
typedef enum ChocolateType ChocolateType;
typedef enum ItemType ItemType;

enum MsgType
{
  OBJECT_CREATED = 1,
  OBJECT_MOVED,
  TRUCK_LEFT,
  TRUCK_RETURNED
};

typedef enum MsgType MsgType;

struct message_payload
{
  MsgType msg_type; // first field of the message struct should be the message type
  int id;
  int index; // index if inside a queue
  Location current_location;
  ChocolateType chocolate_type;
  ItemType item_type;
  // used to delete items when turned into another object
  int ids_to_delete[MAX_BOXES_PER_TRUCK]; // can be ids of items, patches, boxes
  int index_in_queue;
};

// when an object is created:
//     msg_type = OBJECT_CREATED
//     ids_to_delete = ids of objects to hide (if any), like:
//                     the 10 items that were turned into a patch
//                     the 2 patches that were turned into a box

// when a truck leaves:
//     msg_type = TRUCK_LEFT
//     index = the truck number (1, 2 or 3)
//     ids_to_delete = the ids of N boxes that were filled in the leaving truck (N <= MAX_BOXES_PER_TRUCK, remaining cells must be NULL or 0)

// when an object is moved:
//     msg_type = OBJECT_MOVED
//     ids_to_delete = NULL

// when a truck returns:
//     msg_type = TRUCK_RETURNED
//     index = the truck number (1, 2 or 3)
//     ids_to_delete = NULL

typedef struct message_payload message_payload;

struct Object
{
  ChocolateType type;
  char progress[9];
  int id;
  Location current_location;
};

typedef struct Object chocolateProduct;

struct message_buf
{
  long mtype; // first field of the message struct should be the message type
  message_payload payload;
};
typedef struct message_buf message_buf;

typedef struct employee_information
{
  int linenum;
  ChocolateType type;
  int index;

} employee_information;

typedef struct message_buf message_buf;

#endif
