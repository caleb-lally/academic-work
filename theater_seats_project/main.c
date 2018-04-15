#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <windows.h>
#include <semaphore.h>
#include <string.h>

#define theaterSize 100

typedef struct controlBlock { //declaration of the controlBlock structure
char theater[theaterSize];
sem_t semaphoreOne;
int ticketCount;
int agentID;
} controlBlock;

//Function Prototypes
void theaterInitializer(struct controlBlock *); //initialize array values
void salesAgent (struct controlBlock *); //main processing function
void theaterStatus (struct controlBlock *); //display status of theater
void salesAgency(struct controlBlock *); //control function used to ensure all threads finish

int _tmain(int argc, _TCHAR* argv[]) {

    //structure declaration and semaphore initialization
    struct controlBlock controlBlockOne;
    struct controlBlock *controlBlockPtr = &controlBlockOne; //struct pointer
    sem_init(&controlBlockPtr->semaphoreOne, 0, 1);

    theaterInitializer(controlBlockPtr);

    printf("Welcome to TicketMaster v1.0\n");
    theaterStatus(controlBlockPtr);

    salesAgency(controlBlockPtr);
    Sleep(13000); //wait 13 seconds

    printf("All tickets have been sold!\n");
    theaterStatus(controlBlockPtr);

    return 0;
}


void theaterInitializer(struct controlBlock *controlBlockPtr) { //initialize all values in the array to "A" for available.
    int i;
    for (i = 0; i < theaterSize; i++){
        (controlBlockPtr->theater[i]) = 'A';
    }
};

void salesAgent (struct controlBlock *controlBlockPtr) { //change values to "S" for sold
    controlBlockPtr->agentID++;
    const int agentTrack = controlBlockPtr->agentID; //bind const to agentID for tracking purposes
    printf("Sales agent %d has been activated.\n",controlBlockPtr->agentID);
    int j = 0;
    while (controlBlockPtr->ticketCount < theaterSize) {
        sem_wait(&controlBlockPtr->semaphoreOne);
        if (controlBlockPtr->theater[controlBlockPtr->ticketCount] == 'A') {
            controlBlockPtr->theater[controlBlockPtr->ticketCount] = 'S';
            printf("Ticket %d has been sold by agent %d!\n",controlBlockPtr->ticketCount+1,agentTrack);
            Sleep(100);
            j++;
            controlBlockPtr->ticketCount++;
            sem_post(&controlBlockPtr->semaphoreOne);
        } else {
            printf("Waiting...\n");
            Sleep(10);
        }
    }
    return;
};
void salesAgency(struct controlBlock *controlBlockPtr) { //master control structure
    controlBlockPtr->agentID = 0;
    controlBlockPtr->ticketCount = 0;
    _beginthread(&salesAgent,0,controlBlockPtr);
    _beginthread(&salesAgent,0,controlBlockPtr);
    _beginthread(&salesAgent,0,controlBlockPtr);
    _beginthread(&salesAgent,0,controlBlockPtr);

    return;
};

void theaterStatus(struct controlBlock *controlBlockPtr) {
    printf("Here is the status of the theater (A for available, S for sold): \n");
    int i;
    for (i = 0; i < theaterSize; i++) {
        if ( i % 20 == 0) {
            printf("\n"); //carriage return for every fifth row of characters
        } else {
            printf("%c", controlBlockPtr->theater[i]);
        }
    }
    printf("\n");
    return;
};
