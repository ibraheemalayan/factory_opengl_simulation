
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
#define PERSON 1L
#define PILESIZE 20

enum MsgType
{
  ITEM = 1,
  NEWPATCH,
  NEWBOX,
  NEWTRUCK,

  PATCHUPDATED,
  BOXUPDATED,
  ITEMUPDATED,
  TRUCKUPDATED,
};

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
  STORAGE_AREA_A,
  STORAGE_AREA_B,
  STORAGE_AREA_C,

  // trucks
  TRUCK_1,
  TRUCK_2,
  TRUCK_3,

  // removed from the screen
  DELIVERED

};

typedef enum Location Location;
typedef enum MsgType MsgType;
typedef enum ChocolateType ChocolateType;
typedef enum ItemType ItemType;

struct message_payload
{
  long msg_type; // first field of the message struct should be the message type
  int id;
  int index; // index if inside a queue
  Location current_location;
  ChocolateType chocolate_type;
  ItemType item_type;
  // used to delete items when turned into another object
  int ids_to_delete[10]; // can be ids of items, patches, boxes
};

typedef struct chocolateProduct
{
  ChocolateType type;
  unsigned char progress;
  unsigned int id;
} chocolateProduct;

typedef struct message_payload message_payload;

struct message_buf
{
  long mtype; // first field of the message struct should be the message type
  message_payload payload;
};

typedef struct employee_information
{
  int linenum;
  int index;
}employee_information;

typedef struct message_buf message_buf;

#endif
