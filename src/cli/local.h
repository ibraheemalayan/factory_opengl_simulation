#ifndef __LOCAL_H_
#define __LOCAL_H_

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "../include.h"

#define SLEEP_MIN 50 * 1000  // 30 ms
#define SLEEP_MAX 300 * 1000 // 100 ms

struct personInformation
{
        pid_t personID;                      // Process ip
        char gender;                         // F,M
        DocumentType officialDocumentNeeded; // "0"birth certificates, "1"travel documents, "2"family reunion documents, "3"ID-related problems
        int indexLocationInTheHostQueue;
        int tiketNumberInGroupingArea;
        Location currentLocation;
};

struct accessQueueNode
{
        struct personInformation personInfo;
        struct accessQueueNode *nextPesron;
};

struct parent_child_message_payload
{
        int index_in_queue; // index if inside a queue
        Location current_location;
};

typedef struct parent_child_message_payload parent_child_message_payload;

struct parent_message_buf
{
        long pid; // first field of the message struct should be the message type
        parent_child_message_payload payload;
};

struct PidNode // for addProcessToListThatHaveAllProcessesIDs
{
        pid_t personID;
        struct PidNode *nextPid;
};

typedef struct parent_message_buf parent_message_buf;
#endif
