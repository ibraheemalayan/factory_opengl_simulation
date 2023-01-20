#ifndef __LOCAL_H_
#define __LOCAL_H_

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define C_NUMBER_OF_MANUFACTURING_LINES_FOR_PRODUCTS 7

#define C_MANUFACTURING_LINES_TYPEA 3
#define C_MANUFACTURING_LINES_TYPEB 2
#define C_MANUFACTURING_LINES_TYPEC 2

#define C_EMPLOYEES_PER_LINE_IN_MANUFACTURING_PROCESS_TYPE_A 8
#define C_EMPLOYEES_PER_LINE_IN_MANUFACTURING_PROCESS_TYPE_B 6
#define C_EMPLOYEES_PER_LINE_IN_MANUFACTURING_PROCESS_TYPE_C 5

// time limits for type A lines steps ( in microseconds )
#define C_LINE_STEP_MIN 100000
#define C_LINE_STEP_MAX 1000000

#define PATCHER_TIME 100

#define PRINTER_TIME 100

#define C_EMPLOYEES_TO_PATCH_ITEMS_BEFORE_PRINTING 2
#define C_ITEMS_PER_PATCH 10
#define C_ITEMS_PER_CARTON_BOX 20 // basically 2 patches

#define C_DATE_PRINTINC_TIME 0

#define C_EMPLOYEES_TO_MOVE_TO_CONTAINERS 3
#define C_EMPLOYEES_TO_FILL_IN_CARTON_BOXES 3
#define C_STORAGE_EMPLOYEES 2

#define C_TRUCKS_LOADING_EMPLOYEES 2

#define C_TRUCKS 3

#endif
