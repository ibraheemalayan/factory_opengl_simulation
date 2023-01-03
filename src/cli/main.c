#include "local.h"
#include "../std.h"
#include "../include.h"

// TODOs
// 1. Fork & Exec UI | DONE
// 2. Rename symbols that has Typos
// 3. Color output & replace flush with reset_stdout
// 4. Threads performance
// 4. Clean Up | DONE
// 5. Why signals are used for children | DONE

//..........Fuctions........................
void printStartSimulationUntil8am();
void readInputFile();
int randomIntegerInRange(int lower, int upper);
void printAccessQueues();
void printRollingGatesQueues();
void start_simulation();
void displyGroupingAreaQueue();
void create_and_setup_message_queues(); // creates the message queue
void clean_up();                        // cleans up the message queues, and kills the gui and all children
void interrupt_sig_handler(int sig);

int ui_msgq_id, children_msgq_id; // id of the UI message queue

// run the GUI process
void run_gui();

// People creation
void create_people(void *numberOfPeople);

// Dequeue from Queues ->
struct personInformation dequeueNodeFromQueue(pthread_mutex_t *mutex, struct accessQueueNode **FrontQueue, int *numberOfPeopleInTheQueue);
void updateIndexOfQueue(struct accessQueueNode **FrontOfQueue);
void deleteListOfIDsAndKillAllProcesses();

// Enqueue To Queues ->
void enqueueToQueue(struct personInformation personInf, pthread_mutex_t *mutex, struct accessQueueNode **FrontQueue, struct accessQueueNode **RearQueue, int *numberOfPeopleInTheQueue);
void addProcessIDToListThatHaveAllProcessesIDs(struct PidNode **header, pid_t pid);

// remove Node From Queue Because Of Impatience
void removeNodeFromQueueDueToImpatience(struct accessQueueNode **FrontQueue, pid_t personIDKey);

// Threads
void creatThreads();

// security officers threads
void insertToMalesRollingGateQueue();
void insertToFemalesRollingGateQueue();
void insertToFemalesMetalDetector();
void insertToMalesMetalDetector();
void insertToMetalDetectors();
void insertToAccessQueuesAfter8am();

// teller employees threads
void insertToTellersQueues();
void insertToBirthCertificatesTeller();
void insertToTravelDocumentsTeller();
void insertToFamilyReunionDocumentsTeller();
void insertToIDRelatedProblemsTeller();

// finish OIM
void finishOIM();

//..........Gloable Variables................

// Number of Access people
int g_peopleUntil8am;
int g_peopleAfter8amUntil1pm;

pid_t gui_pid = 0;

// Gender Array
char g_gender[2] = {'F', 'M'};

// Pointers to The accessQueue for Males
struct accessQueueNode *FrontAccessQueueMales = NULL;
struct accessQueueNode *RearAccessQueueMales = NULL;

// Pointers to The accessQueue for Females
struct accessQueueNode *FrontAccessQueueFemales = NULL;
struct accessQueueNode *RearAccessQueueFemales = NULL;

// Pointers to the Queue of males path in the rolling gates
struct accessQueueNode *FrontRollingGateQueueMales = NULL;
struct accessQueueNode *RearRollingGateQueueMales = NULL;

// Pointers to the Queue of Females path in the rolling gates
struct accessQueueNode *FrontRollingGateQueueFemales = NULL;
struct accessQueueNode *RearRollingGateQueueFemales = NULL;

// Pointers to the grouping area Queue
struct accessQueueNode *FrontForGroupingAreaQueue = NULL;
struct accessQueueNode *RearForGroupingAreaQueue = NULL;

// Pointers to the birth certificates teller
struct accessQueueNode *FrontForBirthCertificatesTellerQueue = NULL;
struct accessQueueNode *RearForBirthCertificatesTellerQueue = NULL;

// Pointers to the travel documents teller
struct accessQueueNode *FrontForTravelDocumentsTellerQueue = NULL;
struct accessQueueNode *RearForTravelDocumentsTellerQueue = NULL;

// Pointers to the family reunion documents teller
struct accessQueueNode *FrontForFamilyReunionDocumentsTellerQueue = NULL;
struct accessQueueNode *RearForFamilyReunionDocumentsTellerQueue = NULL;

// Pointers to the ID-related problems  teller
struct accessQueueNode *FrontForIDRelatedProblemsTellerQueue = NULL;
struct accessQueueNode *RearForIDRelatedProblemsTellerQueue = NULL;

// Pointers to linke list that have all processes , to kill them
struct PidNode *headerForListHaveAllProcessesIDs = NULL;

// Threshold
#define g_threshold 8

// number of people that left OIM offices unhappy
int numberOfPeopleThatLeftOIMofficesUnhappy = 0;

// number of people that left OIM offices satisfied
int numberOfPeopleThatLeftOIMofficesSatisfied = 0;

// shifting In arrival time
int minShiftingInArrivalTime = 1;
int maxShiftingInArrivalTime = 3;

// Number of Females and males in the Access queues
int g_numberOfMalesInAccessQueue = 0;
int g_numberOfFemalesInAccessQueue = 0;

// Number of Feamles and males in Rolling gate queue
int g_numberOfFemaelsInTheRollingGateQueue = 0;
int g_numberOfMaelsInTheRollingGateQueue = 0;

// Number of people in the grouping area
int g_numberOfpeopleInGroupingArea = 0;

// Number of people in the Birth Certificates Teller Queue
int g_numberOfpeopleInBirthCertificatesTellerQueue = 0;

// Number of people in the Travel Documents Teller Queue
int g_numberOfpeopleInTravelDocumentsTellerQueue = 0;

// Number of people in the Family Reunion Documents Teller Queue
int g_numberOfpeopleInFamilyReunionDocumentsTellerQueue = 0;

// Number of people in the ID Related Problems Teller Queue
int g_numberOfpeopleInIDRelatedProblemsTellerQueue = 0;

// Array of Official Document
char g_OfficialDocument[4][40] = {"Birth Certificates", "Travel Documents", "Family Reunion Documents", "ID-related Problems"};

// Array for status when leaving OIM either satisfied or unhappy
char g_leaving_OIM_Status[2][15] = {"Satisfied", "Unhappy"};

// Mutex on Queues
pthread_mutex_t malesAccessQueue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t femalesAccessQueue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t malesRollingGatQueue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t femalesRollingGatQueue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t groupingAreaQueue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t BirthCertificatesQueue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t TravelDocumentsQueue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t FamilyReunionDocumentsQueue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t IDRelatedProblemsQueue_mutex = PTHREAD_MUTEX_INITIALIZER;

// Index of the process inside the host Queue
// To pass it as argument when create the process
int IndexOfTheProcessInsideTheHostQueue = 0;

int main()
{

    create_and_setup_message_queues();

    run_gui();

    // register signal handler for SIGINT to clean up
    signal(SIGINT, interrupt_sig_handler);

    start_simulation();

    clean_up();

    return 0;
}

void printStartSimulationUntil8am()
{
    printf("\n\nIt's 8am:\n\n");
    reset_stdout();

    usleep(randomIntegerInRange(SLEEP_MIN, SLEEP_MAX));

    // printAccessQueues();

    printf("\n\nNumber of People Arrive Untial 8am : %d \n\n", g_peopleUntil8am);
    reset_stdout();

    // People Start Entering the Rolling Gate Dependent on their Gender
    usleep(randomIntegerInRange(SLEEP_MIN, SLEEP_MAX));
    printf("\n\nPeople Start Entering the Rolling Gate Dependes on their Gender >>>>>>\n\n");
}

void creatThreads()
{

    /* create a new thread that will keep move nodes from males access queue to Rolling Gate Queue "Males" */
    pthread_t p_thread1;
    pthread_create(&p_thread1, NULL, (void *)insertToMalesRollingGateQueue, NULL);

    /* create a new thread that will keep move nodes from Females access queue to Rolling Gate Queue "Females" */
    pthread_t p_thread2;
    pthread_create(&p_thread2, NULL, (void *)insertToFemalesRollingGateQueue, NULL);

    /* create a new thread that will keep move nodes from Males and Females rolling gate queues to the metal detectors*/
    pthread_t p_thread3;
    pthread_create(&p_thread3, NULL, (void *)insertToMetalDetectors, NULL);

    /* create a new thread that will keep move nodes from  Grouping area queue to tellers Queues*/
    pthread_t p_thread4;
    pthread_create(&p_thread4, NULL, (void *)insertToTellersQueues, NULL);

    /* create a new thread that will keep move nodes from  Birth Certificates Teller Queue to the Birth Certificates Teller*/
    pthread_t p_thread5;
    pthread_create(&p_thread5, NULL, (void *)insertToBirthCertificatesTeller, NULL);

    /* create a new thread that will keep move nodes from  Travel Documents Teller Queue to the Travel Documents Teller*/
    pthread_t p_thread6;
    pthread_create(&p_thread6, NULL, (void *)insertToTravelDocumentsTeller, NULL);

    /* create a new thread that will keep move nodes from  Family Reunion Documents Teller Queue to the Family Reunion Documents Teller*/
    pthread_t p_thread7;
    pthread_create(&p_thread7, NULL, (void *)insertToFamilyReunionDocumentsTeller, NULL);

    /* create a new thread that will keep move nodes from  ID Related Problems Teller Queue to the ID Related Problems Teller*/
    pthread_t p_thread8;
    pthread_create(&p_thread8, NULL, (void *)insertToIDRelatedProblemsTeller, NULL);

    /* add peopel after 8 am */
    pthread_t p_thread9;
    pthread_create(&p_thread9, NULL, (void *)create_people, (void *)&g_peopleAfter8amUntil1pm);

    /* finishOIM*/
    pthread_t p_thread10;
    pthread_create(&p_thread10, NULL, (void *)finishOIM, NULL);

    pthread_join(p_thread1, NULL);
    pthread_join(p_thread2, NULL);
    pthread_join(p_thread3, NULL);
    pthread_join(p_thread4, NULL);
    pthread_join(p_thread5, NULL);
    pthread_join(p_thread6, NULL);
    pthread_join(p_thread7, NULL);
    pthread_join(p_thread8, NULL);
    pthread_join(p_thread9, NULL);
    pthread_join(p_thread10, NULL);
}
void start_simulation()
{
    printf("\n\n\n\n.......5:00 AM: People Start coming.......\n\n");
    reset_stdout();

    // Read input file
    readInputFile();

    // Creat people until 8am
    create_people((void *)&g_peopleUntil8am);

    printStartSimulationUntil8am();

    // Create Threads
    creatThreads();

    clean_up();
    exit(0);
}

Location get_location_enum(struct accessQueueNode *queue_front)
{

    if (queue_front == FrontAccessQueueMales)
    {
        return MaleOutsideGatesArea;
    }
    if (queue_front == FrontAccessQueueFemales)
    {
        return FemaleOutsideGatesArea;
    }
    if (queue_front == FrontRollingGateQueueMales)
    {
        return MaleMetalDetectorQueue;
    }
    if (queue_front == FrontRollingGateQueueFemales)
    {
        return FemaleMetalDetectorQueue;
    }
    if (queue_front == FrontForGroupingAreaQueue)
    {
        return InnerGroupingArea;
    }
    if (queue_front == FrontForBirthCertificatesTellerQueue)
    {
        return BxTellerQ;
    }
    if (queue_front == FrontForTravelDocumentsTellerQueue)
    {
        return TxTellerQ;
    }
    if (queue_front == FrontForFamilyReunionDocumentsTellerQueue)
    {
        return RxTellerQ;
    }
    if (queue_front == FrontForIDRelatedProblemsTellerQueue)
    {
        return IxTellerQ;
    }

    printf("Error in get_location_enum");

    return 0; // TODO error
}

void readInputFile()
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("cli/inputfile.txt", "r");
    if (fp == NULL)
    {
        clean_up();
        exit(EXIT_FAILURE);
    }
    int lineNumber = 0;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        // printf("Retrieved line of length %zu:\n", read);

        lineNumber++;

        if (lineNumber == 1)
        { // read peopleUntil8am
            // printf("%s", line);

            // Splite the line
            char delim[] = " ";
            char *ptr = strtok(line, delim);

            ptr = strtok(NULL, delim);
            int l_range1 = atoi(ptr);

            ptr = strtok(NULL, delim);
            int l_range2 = atoi(ptr);

            // make a randome number of people within the range

            g_peopleUntil8am = randomIntegerInRange(l_range1, l_range2);
            // printf("\ng_peopleUntil8am:%d\n", g_peopleUntil8am);
        }

        if (lineNumber == 2)
        { // read peopleAfter8amUntil1pm
            // printf("%s", line);

            // Splite the line
            char delim[] = " ";
            char *ptr = strtok(line, delim);

            ptr = strtok(NULL, delim);
            int l_range1 = atoi(ptr);

            ptr = strtok(NULL, delim);
            int l_range2 = atoi(ptr);

            // make a randome number of people within the range

            g_peopleAfter8amUntil1pm = randomIntegerInRange(l_range1, l_range2);
            // printf("\npeopleAfter8amUntil1pm:%d\n", g_peopleAfter8amUntil1pm);
        }
    }

    fclose(fp);
}

int randomIntegerInRange(int lower, int upper)
{
    srand(time(NULL)); // randomize seed
    return (rand() % (upper - lower + 1)) + lower;
}

void addProcessIDToListThatHaveAllProcessesIDs(struct PidNode **header, pid_t pid)
{
    // create a new node
    struct PidNode *ptr = (struct PidNode *)malloc(sizeof(struct PidNode));
    if (ptr == NULL)
    {
        printf("\nOVERFLOW \n");
        return;
    }
    else
    {
        ptr->personID = pid;
        ptr->nextPid = *header;
        *header = ptr;
    }
}

void deleteListOfIDsAndKillAllProcesses()
{
    struct PidNode *temp;

    while (headerForListHaveAllProcessesIDs != NULL)
    {
        temp = headerForListHaveAllProcessesIDs;
        kill(temp->personID, SIGKILL);
        waitpid(temp->personID, NULL, 0);
        headerForListHaveAllProcessesIDs = headerForListHaveAllProcessesIDs->nextPid;

        free(temp);
    }

    printf("\n\nSUCCESSFULLY DELETED ALL Processes\n\n");
    reset_stdout();
}

void enqueueToQueue(struct personInformation personInf, pthread_mutex_t *mutex, struct accessQueueNode **FrontQueue, struct accessQueueNode **RearQueue, int *numberOfPeopleInTheQueue)
{
    pthread_mutex_lock(mutex);
    struct personInformation person = personInf;
    struct accessQueueNode *ptr = (struct accessQueueNode *)malloc(sizeof(struct accessQueueNode));
    if (ptr == NULL)
    {
        printf("\nOVERFLOW \n");
        return;
    }
    else
    {
        person.indexLocationInTheHostQueue = (*numberOfPeopleInTheQueue); // update Index
        ptr->personInfo = person;
        if ((*FrontQueue) == NULL)
        {
            (*FrontQueue) = ptr;
            (*RearQueue) = ptr;
            (*FrontQueue)->nextPesron = NULL;
            (*RearQueue)->nextPesron = NULL;
        }
        else
        {
            (*RearQueue)->nextPesron = ptr;
            (*RearQueue) = ptr;
            (*RearQueue)->nextPesron = NULL;
        }
        usleep(randomIntegerInRange(SLEEP_MIN, SLEEP_MAX));
        (*numberOfPeopleInTheQueue)++;

        Location l = get_location_enum(*FrontQueue);

        person.currentLocation = l;

        if (person.currentLocation >= 11) // teller
        {
            person.indexLocationInTheHostQueue = 0;
        }
        printf("\n\n2 - NQUEU pid: %d, index: %d, location: %d\n", person.personID, person.indexLocationInTheHostQueue, person.currentLocation);

        notify_person(&person); // Signal the process to move inside the host Queue
    }
    pthread_mutex_unlock(mutex);
}

struct personInformation dequeueNodeFromQueue(pthread_mutex_t *mutex, struct accessQueueNode **FrontQueue, int *numberOfPeopleInTheQueue)
{
    pthread_mutex_lock(mutex);
    struct accessQueueNode *temp = NULL;
    struct personInformation person = {0};
    person.indexLocationInTheHostQueue = -1; // update Index
    if ((*FrontQueue) == NULL)
    {
        printf("Underflow\n");
        return person;
    }
    else
    {
        temp = (*FrontQueue);
        person = temp->personInfo;
        person.indexLocationInTheHostQueue = -1; // update Index
        // notify_person(&person);                  // Signal the process to move inside the host Queue
        (*FrontQueue) = (*FrontQueue)->nextPesron;
        free(temp);
        (*numberOfPeopleInTheQueue)--;
    }
    pthread_mutex_unlock(mutex);
    return person;
}

void notify_person(struct personInformation *person)
{

    parent_message_buf buf;

    buf.pid = (long)person->personID;

    buf.payload.current_location = person->currentLocation;
    buf.payload.index_in_queue = person->indexLocationInTheHostQueue;

    if (msgsnd(children_msgq_id, &buf, sizeof(buf), 0) == -1)
    {
        perror("children msgsnd in parent");
        exit(3);
    }

    kill(person->personID, SIGUSR1);
}

void removeNodeFromQueueDueToImpatience(struct accessQueueNode **FrontQueue, pid_t personIDKey)
{

    // Store head node
    struct accessQueueNode *temp = *FrontQueue, *prev;

    // If head node itself holds the key to be deleted
    if (temp != NULL && temp->personInfo.personID == personIDKey)
    {
        (*FrontQueue) = temp->nextPesron; // Changed head
        free(temp);                       // free old head
        return;
    }

    // Search for the personIDKey to be deleted, keep track of the
    // previous node as we need to change 'prev->nextPesron'
    while (temp != NULL && temp->personInfo.personID != personIDKey)
    {
        prev = temp;
        temp = temp->nextPesron;
    }

    // If personIDKey was not present in linked list
    if (temp == NULL)
        return;

    // Unlink the node from linked list
    prev->nextPesron = temp->nextPesron;

    free(temp); // Free memory
}

void updateIndexOfQueue(struct accessQueueNode **FrontOfQueue)
{
    struct accessQueueNode *tmp = *FrontOfQueue;
    while (tmp != NULL)
    {
        tmp->personInfo.indexLocationInTheHostQueue = tmp->personInfo.indexLocationInTheHostQueue - 1;
        notify_person(&(tmp->personInfo)); // Signal the process to move
        tmp = tmp->nextPesron;
    }
}

void displyGroupingAreaQueue()
{

    struct accessQueueNode *temp = NULL;
    if ((FrontForGroupingAreaQueue == NULL) && (RearForGroupingAreaQueue == NULL))
    {
        printf("\n\nMales Access Queue is Empty\n");
    }
    else
    {
        printf("\n\nThe Access queue for Males is :\n\n");
        temp = FrontForGroupingAreaQueue;
        while (temp)
        {
            printf("%d--%c--%d\n", temp->personInfo.personID, temp->personInfo.gender, temp->personInfo.officialDocumentNeeded);
            temp = temp->nextPesron;
        }
    }
}

void printAccessQueues()
{

    struct accessQueueNode *temp = NULL;
    if ((FrontAccessQueueMales == NULL) && (RearAccessQueueMales == NULL))
    {
        printf("\n\nMales Access Queue is Empty\n");
        reset_stdout();
    }
    else
    {
        printf("\n\nThe Access queue for Males is :\n\n");
        temp = FrontAccessQueueMales;
        while (temp)
        {
            sleep(1); // simulation for time delaying
            printf("%d--%c--%d\n", temp->personInfo.personID, temp->personInfo.gender, temp->personInfo.officialDocumentNeeded);

            reset_stdout();
            temp = temp->nextPesron;
        }
    }

    if ((FrontAccessQueueFemales == NULL) && (RearAccessQueueFemales == NULL))
    {
        printf("\n\nFemales Access Queue is Empty\n");
        reset_stdout();
    }
    else
    {
        printf("\n\nThe Access queue for Females is :\n\n");
        temp = FrontAccessQueueFemales;
        while (temp)
        {
            printf("%d--%c--%d\n", temp->personInfo.personID, temp->personInfo.gender, temp->personInfo.officialDocumentNeeded);
            reset_stdout();
            temp = temp->nextPesron;
        }
    }
}

void printRollingGatesQueues()
{
    struct accessQueueNode *temp = NULL;
    if ((FrontRollingGateQueueMales == NULL) && (RearRollingGateQueueMales == NULL))
    {
        printf("\n\nMales Rolling Gate Queue is Empty\n");
    }
    else
    {
        printf("\n\nThe Rolling Gate Queue for Males is :\n\n");
        temp = FrontRollingGateQueueMales;
        while (temp)
        {
            printf("%d--%c--%d\n", temp->personInfo.personID, temp->personInfo.gender, temp->personInfo.officialDocumentNeeded);
            temp = temp->nextPesron;
        }
    }
    if ((FrontRollingGateQueueFemales == NULL) && (RearRollingGateQueueFemales == NULL))
    {
        printf("\n\nFemales Rolling Gate Queue is Empty\n");
    }
    else
    {
        printf("\n\nThe Rolling Gate Queue for Females is :\n\n");
        temp = FrontRollingGateQueueFemales;
        while (temp)
        {
            printf("%d--%c--%d\n", temp->personInfo.personID, temp->personInfo.gender, temp->personInfo.officialDocumentNeeded);
            temp = temp->nextPesron;
        }
    }
}

void insertToMalesRollingGateQueue()
{
    struct personInformation Person;
    while (1)
    {
        if (g_numberOfMaelsInTheRollingGateQueue < g_threshold && g_numberOfMalesInAccessQueue > 0)
        {

            usleep(randomIntegerInRange(SLEEP_MIN, SLEEP_MAX)); // simulation for dealy time for  moving from Access queue to Rolling gate queue
            Person = dequeueNodeFromQueue(&malesAccessQueue_mutex, &FrontAccessQueueMales, &g_numberOfMalesInAccessQueue);
            enqueueToQueue(Person, &malesRollingGatQueue_mutex, &FrontRollingGateQueueMales, &RearRollingGateQueueMales, &g_numberOfMaelsInTheRollingGateQueue);
            updateIndexOfQueue(&FrontAccessQueueMales); // updateIndexOfQueue for AccessQueueMales
        }
    }
}

void insertToFemalesRollingGateQueue()
{
    struct personInformation Person;
    while (1)
    {
        if (g_numberOfFemaelsInTheRollingGateQueue < g_threshold && g_numberOfFemalesInAccessQueue > 0)
        {
            usleep(randomIntegerInRange(SLEEP_MIN, SLEEP_MAX)); // simulation for dealy time for  moving from Access queue to Rolling gate queue
            Person = dequeueNodeFromQueue(&femalesAccessQueue_mutex, &FrontAccessQueueFemales, &g_numberOfFemalesInAccessQueue);
            enqueueToQueue(Person, &femalesRollingGatQueue_mutex, &FrontRollingGateQueueFemales, &RearRollingGateQueueFemales, &g_numberOfFemaelsInTheRollingGateQueue);
            updateIndexOfQueue(&FrontAccessQueueFemales); // updateIndexOfQueue for AccessQueueFemales
            // printRollingGatesQueues();
            // printAccessQueues();
        }
    }
}

void insertToMalesMetalDetector()
{
    struct personInformation malePersonInMetalDetectorForMales;
    if (g_numberOfMaelsInTheRollingGateQueue > 0)
    {
        usleep(randomIntegerInRange(SLEEP_MIN, SLEEP_MAX)); // simulation for delay

        malePersonInMetalDetectorForMales = dequeueNodeFromQueue(&malesRollingGatQueue_mutex, &FrontRollingGateQueueMales, &g_numberOfMaelsInTheRollingGateQueue);

        malePersonInMetalDetectorForMales.indexLocationInTheHostQueue = 0;
        malePersonInMetalDetectorForMales.currentLocation = MetalDetector;

        notify_person(&malePersonInMetalDetectorForMales);

        printf("\n\nPerson %d Enter the Metal Detector For Males, Gernder %c, Official Document Needed is %s\n\n", malePersonInMetalDetectorForMales.personID, malePersonInMetalDetectorForMales.gender, g_OfficialDocument[malePersonInMetalDetectorForMales.officialDocumentNeeded]);
        reset_stdout();

        // random delay in metal detector
        usleep(randomIntegerInRange(SLEEP_MIN * 2, SLEEP_MAX * 2)); // simulation for delay in the Metal Detector as sleep between 5 to 8 seconds

        enqueueToQueue(malePersonInMetalDetectorForMales, &groupingAreaQueue_mutex, &FrontForGroupingAreaQueue, &RearForGroupingAreaQueue, &g_numberOfpeopleInGroupingArea);
        updateIndexOfQueue(&FrontRollingGateQueueMales); // updateIndexOfQueue for RollingGateQueueMales
                                                         // printf("Person %d leave the Metal Detector For Males and Enter the Grouping Area, Gernder %c, Official Document Needed is %s\n",malePersonInMetalDetectorForMales.personID, malePersonInMetalDetectorForMales.gender,g_OfficialDocument[malePersonInMetalDetectorForMales.officialDocumentNeeded]);
    }
}

void insertToFemalesMetalDetector()
{
    struct personInformation femalePersonInMetalDetectorForMales;
    if (g_numberOfFemaelsInTheRollingGateQueue > 0)
    {
        usleep(randomIntegerInRange(SLEEP_MIN, SLEEP_MAX)); // simulation for delay

        femalePersonInMetalDetectorForMales = dequeueNodeFromQueue(&femalesRollingGatQueue_mutex, &FrontRollingGateQueueFemales, &g_numberOfFemaelsInTheRollingGateQueue);

        femalePersonInMetalDetectorForMales.indexLocationInTheHostQueue = 0;
        femalePersonInMetalDetectorForMales.currentLocation = MetalDetector;

        notify_person(&femalePersonInMetalDetectorForMales);

        printf("\n\nPerson %d Enter the Metal Detector For Females, Gernder %c, Official Document Needed is %s\n\n", femalePersonInMetalDetectorForMales.personID, femalePersonInMetalDetectorForMales.gender, g_OfficialDocument[femalePersonInMetalDetectorForMales.officialDocumentNeeded]);
        reset_stdout();
        // random delay in metal detector
        usleep(randomIntegerInRange(SLEEP_MIN * 2, SLEEP_MAX * 2)); // simulation for delay in the Metal Detector as sleep between 5 to 8 seconds
        enqueueToQueue(femalePersonInMetalDetectorForMales, &groupingAreaQueue_mutex, &FrontForGroupingAreaQueue, &RearForGroupingAreaQueue, &g_numberOfpeopleInGroupingArea);
        updateIndexOfQueue(&FrontRollingGateQueueFemales); // updateIndexOfQueue for RollingGateQueueFemales
                                                           // printf("Person %d leave the Metal Detector For Males and Enter the Grouping Area, Gernder %c, Official Document Needed is %s\n",malePersonInMetalDetectorForMales.personID, malePersonInMetalDetectorForMales.gender,g_OfficialDocument[malePersonInMetalDetectorForMales.officialDocumentNeeded]);
                                                           // reset_stdout();
    }
}

void insertToMetalDetectors()
{

    int randomChoicFromRollingGates;
    int ToMalesMetalDetector = 0;
    int ToFemalesMetalDetector = 1;
    while (1)
    {
        usleep(100);
        randomChoicFromRollingGates = randomIntegerInRange(ToMalesMetalDetector, ToFemalesMetalDetector);
        if (randomChoicFromRollingGates == 0)
            insertToMalesMetalDetector();
        else
            insertToFemalesMetalDetector();
    }
}

void insertToTellersQueues()
{

    struct personInformation Person;
    while (1)
    {
        if (g_numberOfpeopleInGroupingArea > 0)
        {
            usleep(randomIntegerInRange(SLEEP_MIN, SLEEP_MAX)); // simulation for delay
            Person = dequeueNodeFromQueue(&groupingAreaQueue_mutex, &FrontForGroupingAreaQueue, &g_numberOfpeopleInGroupingArea);
            printf("\n\nPerson %d leave the Grouping Area Queue, Gernder %c, Official Document Needed is %s\n\n", Person.personID, Person.gender, g_OfficialDocument[Person.officialDocumentNeeded]);
            reset_stdout();
            usleep(randomIntegerInRange(SLEEP_MIN, SLEEP_MAX)); // simulation for delaying while moving to Tellers Queuesas sleep between 5 to 8 seconds
            if (Person.officialDocumentNeeded == 0)
                enqueueToQueue(Person, &BirthCertificatesQueue_mutex, &FrontForBirthCertificatesTellerQueue, &RearForBirthCertificatesTellerQueue, &g_numberOfpeopleInBirthCertificatesTellerQueue);
            else if (Person.officialDocumentNeeded == 1)
                enqueueToQueue(Person, &TravelDocumentsQueue_mutex, &FrontForTravelDocumentsTellerQueue, &RearForTravelDocumentsTellerQueue, &g_numberOfpeopleInTravelDocumentsTellerQueue);
            else if (Person.officialDocumentNeeded == 2)
                enqueueToQueue(Person, &FamilyReunionDocumentsQueue_mutex, &FrontForFamilyReunionDocumentsTellerQueue, &RearForFamilyReunionDocumentsTellerQueue, &g_numberOfpeopleInFamilyReunionDocumentsTellerQueue);
            else if (Person.officialDocumentNeeded == 3)
                enqueueToQueue(Person, &IDRelatedProblemsQueue_mutex, &FrontForIDRelatedProblemsTellerQueue, &RearForIDRelatedProblemsTellerQueue, &g_numberOfpeopleInIDRelatedProblemsTellerQueue);

            updateIndexOfQueue(&FrontForGroupingAreaQueue); // updateIndexOfQueue for GroupingAreaQueue
        }
    }
}

void insertToBirthCertificatesTeller()
{

    struct personInformation Person;
    int leaving_OIM_Status; // 0:Satisfied ,  1:Unhappy
    while (1)
    {
        if (g_numberOfpeopleInBirthCertificatesTellerQueue > 0)
        {
            // sleep(randomIntegerInRange(3,6));//simulation for delay
            Person = dequeueNodeFromQueue(&BirthCertificatesQueue_mutex, &FrontForBirthCertificatesTellerQueue, &g_numberOfpeopleInBirthCertificatesTellerQueue);
            printf("\n\nPerson %d achieve the Birth Certificates Teller, Gernder %c, Official Document Needed is %s\n\n", Person.personID, Person.gender, g_OfficialDocument[Person.officialDocumentNeeded]);
            reset_stdout();

            Person.currentLocation = BxTeller;
            Person.indexLocationInTheHostQueue = 0;

            notify_person(&Person);

            usleep(randomIntegerInRange(SLEEP_MIN * 3, SLEEP_MAX * 6)); // simulation for delay in the Birth Certificates Teller as sleep between 10 to 15 seconds

            leaving_OIM_Status = randomIntegerInRange(0, 1); // 0:Satisfied ,  1:Unhappy
            printf("Person %d leave the Birth Certificates Teller, Gernder %c, Official Document Needed is %s, leaving OIM Status :%s\n", Person.personID, Person.gender, g_OfficialDocument[Person.officialDocumentNeeded], g_leaving_OIM_Status[leaving_OIM_Status]);
            reset_stdout();
            if (leaving_OIM_Status == 0)
            {
                numberOfPeopleThatLeftOIMofficesSatisfied++;
                Person.currentLocation = LeaveHappy;
            }
            else
            {
                numberOfPeopleThatLeftOIMofficesUnhappy++;
                Person.currentLocation = LeaveUnhappy;
            }

            Person.indexLocationInTheHostQueue = 0;
            notify_person(&Person);

            updateIndexOfQueue(&FrontForBirthCertificatesTellerQueue); // updateIndexOfQueue for BirthCertificatesTellerQueue
        }
    }
}

void insertToTravelDocumentsTeller()
{

    struct personInformation Person;
    int leaving_OIM_Status; // 0:Satisfied ,  1:Unhappy
    while (1)
    {
        if (g_numberOfpeopleInTravelDocumentsTellerQueue > 0)
        {
            // sleep(randomIntegerInRange(3,6));//simulation for delay
            Person = dequeueNodeFromQueue(&TravelDocumentsQueue_mutex, &FrontForTravelDocumentsTellerQueue, &g_numberOfpeopleInTravelDocumentsTellerQueue);
            printf("\n\nPerson %d achieve the Travel Documents Teller, Gernder %c, Official Document Needed is %s\n\n", Person.personID, Person.gender, g_OfficialDocument[Person.officialDocumentNeeded]);
            reset_stdout();

            Person.currentLocation = TxTeller;
            Person.indexLocationInTheHostQueue = 0;

            notify_person(&Person);

            usleep(randomIntegerInRange(SLEEP_MIN * 3, SLEEP_MAX * 6)); // simulation for delay in the Birth Certificates Teller as sleep between 10 to 15 seconds

            leaving_OIM_Status = randomIntegerInRange(0, 1); // 0:Satisfied ,  1:Unhappy
            printf("Person %d leave the Travel Documents Teller, Gernder %c, Official Document Needed is %s, leaving OIM Status :%s\n", Person.personID, Person.gender, g_OfficialDocument[Person.officialDocumentNeeded], g_leaving_OIM_Status[leaving_OIM_Status]);
            reset_stdout();
            if (leaving_OIM_Status == 0)
            {
                numberOfPeopleThatLeftOIMofficesSatisfied++;
                Person.currentLocation = LeaveHappy;
            }
            else
            {
                numberOfPeopleThatLeftOIMofficesUnhappy++;
                Person.currentLocation = LeaveUnhappy;
            }

            Person.indexLocationInTheHostQueue = 0;
            notify_person(&Person);
            updateIndexOfQueue(&FrontForTravelDocumentsTellerQueue); // updateIndexOfQueue for TravelDocumentsTellerQueue
        }
    }
}

void insertToFamilyReunionDocumentsTeller()
{

    struct personInformation Person;
    int leaving_OIM_Status; // 0:Satisfied ,  1:Unhappy
    while (1)
    {
        if (g_numberOfpeopleInFamilyReunionDocumentsTellerQueue > 0)
        {
            // sleep(randomIntegerInRange(3,6));//simulation for delay
            Person = dequeueNodeFromQueue(&FamilyReunionDocumentsQueue_mutex, &FrontForFamilyReunionDocumentsTellerQueue, &g_numberOfpeopleInFamilyReunionDocumentsTellerQueue);
            printf("\n\nPerson %d achieve the Family Reunion Documents Teller, Gernder %c, Official Document Needed is %s\n\n", Person.personID, Person.gender, g_OfficialDocument[Person.officialDocumentNeeded]);
            reset_stdout();

            Person.currentLocation = RxTeller;
            Person.indexLocationInTheHostQueue = 0;

            notify_person(&Person);

            usleep(randomIntegerInRange(SLEEP_MIN * 3, SLEEP_MAX * 6)); // simulation for delay in the Birth Certificates Teller as sleep between 10 to 15 seconds

            leaving_OIM_Status = randomIntegerInRange(0, 1); // 0:Satisfied ,  1:Unhappy
            printf("Person %d leave the Family Reunion Documents Teller, Gernder %c, Official Document Needed is %s, leaving OIM Status :%s\n", Person.personID, Person.gender, g_OfficialDocument[Person.officialDocumentNeeded], g_leaving_OIM_Status[leaving_OIM_Status]);
            reset_stdout();

            if (leaving_OIM_Status == 0)
            {
                numberOfPeopleThatLeftOIMofficesSatisfied++;
                Person.currentLocation = LeaveHappy;
            }
            else
            {
                numberOfPeopleThatLeftOIMofficesUnhappy++;
                Person.currentLocation = LeaveUnhappy;
            }

            Person.indexLocationInTheHostQueue = 0;
            notify_person(&Person);

            updateIndexOfQueue(&FrontForFamilyReunionDocumentsTellerQueue); // updateIndexOfQueue for FamilyReunionDocumentsTellerQueue
        }
    }
}

void insertToIDRelatedProblemsTeller()
{

    struct personInformation Person;
    int leaving_OIM_Status; // 0:Satisfied ,  1:Unhappy
    while (1)
    {
        if (g_numberOfpeopleInIDRelatedProblemsTellerQueue > 0)
        {
            // sleep(randomIntegerInRange(3,6));//simulation for delay
            Person = dequeueNodeFromQueue(&IDRelatedProblemsQueue_mutex, &FrontForIDRelatedProblemsTellerQueue, &g_numberOfpeopleInIDRelatedProblemsTellerQueue);
            printf("\n\nPerson %d achieve the ID Related Problems Teller, Gernder %c, Official Document Needed is %s\n\n", Person.personID, Person.gender, g_OfficialDocument[Person.officialDocumentNeeded]);
            reset_stdout();

            Person.currentLocation = IxTeller;
            Person.indexLocationInTheHostQueue = 0;

            notify_person(&Person);

            usleep(randomIntegerInRange(SLEEP_MIN * 3, SLEEP_MAX * 6)); // simulation for delay in the Birth Certificates Teller as sleep between 10 to 15 seconds

            leaving_OIM_Status = randomIntegerInRange(0, 1); // 0:Satisfied ,  1:Unhappy
            printf("Person %d leave the ID Related Problems Teller, Gernder %c, Official Document Needed is %s, leaving OIM Status :%s\n", Person.personID, Person.gender, g_OfficialDocument[Person.officialDocumentNeeded], g_leaving_OIM_Status[leaving_OIM_Status]);
            reset_stdout();
            if (leaving_OIM_Status == 0)
            {
                numberOfPeopleThatLeftOIMofficesSatisfied++;
                Person.currentLocation = LeaveHappy;
            }
            else
            {
                numberOfPeopleThatLeftOIMofficesUnhappy++;
                Person.currentLocation = LeaveUnhappy;
            }

            Person.indexLocationInTheHostQueue = 0;
            notify_person(&Person);

            updateIndexOfQueue(&FrontForIDRelatedProblemsTellerQueue); // updateIndexOfQueue for IDRelatedProblemsTellerQueue
        }
    }
}

void create_people(void *numberOfPeople)
{
    int number_of_people = *((int *)numberOfPeople);
    int i, pid;
    struct personInformation person = {0};
    for (i = 0; i < number_of_people; i++)
    {
        person.personID = 0;
        person.gender = g_gender[randomIntegerInRange(0, 1)];       // person.gender = g_gender[randomIntegerInRange(0,1)];
        person.officialDocumentNeeded = randomIntegerInRange(0, 3); // person.officialDocumentNeeded  = randomIntegerInRange(0,3);
        person.indexLocationInTheHostQueue = -1;
        person.tiketNumberInGroupingArea = -1;
        if (person.gender == 'M')
        {

            IndexOfTheProcessInsideTheHostQueue = g_numberOfMalesInAccessQueue;
            person.currentLocation = MaleOutsideGatesArea;
        }
        else
        {
            IndexOfTheProcessInsideTheHostQueue = g_numberOfFemalesInAccessQueue;
            person.currentLocation = FemaleOutsideGatesArea;
        }
        pid = fork();
        if (pid == -1)
        {
            printf("fork failure\n");
            clean_up();
            exit(-1);
        }
        else if (pid > 0)
        {
            // Print the info for the arrival person:
            person.personID = pid;
            usleep(randomIntegerInRange(SLEEP_MIN / 3, SLEEP_MAX / 3)); // simulation for time delaying for arrival time
            if (person.gender == 'M')
                enqueueToQueue(person, &malesAccessQueue_mutex, &FrontAccessQueueMales, &RearAccessQueueMales, &g_numberOfMalesInAccessQueue);
            else
                enqueueToQueue(person, &femalesAccessQueue_mutex, &FrontAccessQueueFemales, &RearAccessQueueFemales, &g_numberOfFemalesInAccessQueue);

            addProcessIDToListThatHaveAllProcessesIDs(&headerForListHaveAllProcessesIDs, pid); // add to ListHaveAllProcessesIDs
        }
        else if (pid == 0)
        {

            // Passing argument
            char indexLocationInTheHostQueue[10];
            sprintf(indexLocationInTheHostQueue, "%d", IndexOfTheProcessInsideTheHostQueue + 1);

            char officialDocumentNeeded[10];
            sprintf(officialDocumentNeeded, "%d", person.officialDocumentNeeded);

            char gen[3];
            if (person.gender == 'M')
            {
                strcpy(gen, "1");
            }
            else
            {
                strcpy(gen, "2");
            }

            execlp("./bin/child.o", "child.o", gen, officialDocumentNeeded, indexLocationInTheHostQueue, NULL);
            perror("\n> child: exec\n");
            clean_up();
            exit(-2);
        }
    }
}

void finishOIM()
{
    while (1)
    {
        usleep(100);
        if (numberOfPeopleThatLeftOIMofficesSatisfied > g_threshold || numberOfPeopleThatLeftOIMofficesUnhappy > g_threshold)
        {
            green_stdout();
            printf("\n\nUser Specified Threshold Reached\n");
            reset_stdout();

            clean_up();
            exit(0);
        }
    }
}

void run_gui()
{

    gui_pid = fork();
    if (gui_pid == -1)
    {
        printf("fork gui failure\n");
        clean_up();
        exit(-4);
    }
    else if (gui_pid == 0)
    {
        execlp("./bin/gui.o", "gui.o", NULL);
        perror("\n> gui: exec failure\n");
        clean_up();
        exit(-2);
    }
}

void create_and_setup_message_queues()
{

    key_t ui_queue_key, children_queue_key;

    // remove queue if exists
    remove("ui_queue.bin");

    // remove queue if exists
    remove("children_queue.bin");

    // create file to use as message queue key
    system("touch ui_queue.bin");
    system("chmod 666 ui_queue.bin");

    // create file to use as message queue key
    system("touch children_queue.bin");
    system("chmod 666 children_queue.bin");

    if ((ui_queue_key = ftok("ui_queue.bin", 30)) == -1)
    {
        perror("ftok");
        clean_up();
        exit(1);
    }
    if ((children_queue_key = ftok("children_queue.bin", 30)) == -1)
    {
        perror("ftok");
        clean_up();
        exit(1);
    }

    ui_msgq_id = msgget(ui_queue_key, 0666 | IPC_CREAT);
    if (ui_msgq_id == -1)
    {
        perror("msgget ui queue");
        clean_up();
        exit(2);
    }

    children_msgq_id = msgget(children_queue_key, 0666 | IPC_CREAT);
    if (children_msgq_id == -1)
    {
        perror("msgget children queue");
        clean_up();
        exit(2);
    }

    struct msqid_ds ui_queue_info;
    // read existing message queue info into ui_queue_info
    if (msgctl(ui_msgq_id, IPC_STAT, &ui_queue_info) == -1)
    {
        perror("Can not read message queue info");
        clean_up();
        exit(5);
    }

    ui_queue_info.msg_qbytes = 20480;

    // increase buffer size
    msgctl(ui_msgq_id, IPC_SET, &ui_queue_info);

    struct msqid_ds children_queue_info;
    // read existing message queue info into ui_queue_info
    if (msgctl(children_msgq_id, IPC_STAT, &children_queue_info) == -1)
    {
        perror("Can not read message queue info");
        clean_up();
        exit(5);
    }

    children_queue_info.msg_qbytes = 20480;

    // increase buffer size
    msgctl(children_msgq_id, IPC_SET, &children_queue_info);

    green_stdout();
    printf("Message queues have been created\n");
    reset_stdout();
}

void clean_up()
{

    if (gui_pid != 0)
    {
        kill(gui_pid, SIGKILL);
        waitpid(gui_pid, NULL, 0);
    }

    // TODO kill and wait all children
    deleteListOfIDsAndKillAllProcesses();

    // remove the message queue from the System V IPC
    msgctl(ui_msgq_id, IPC_RMID, NULL);
    msgctl(children_msgq_id, IPC_RMID, NULL);

    // remove the queue file
    remove("ui_queue.bin");
    remove("children_queue.bin");
}

void interrupt_sig_handler(int sig)
{
    red_stdout();
    printf("\n\nInterrupt signal received, cleaning up queues.\n");
    clean_up();
    reset_stdout();
    exit(0);
}
