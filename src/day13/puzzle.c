#include "readFile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define INPUT_LINE_BUFFER_SIZE 220
#define BUS_ID_DELIMITERS "\n,"

typedef struct BusIdListItem{
    int id;
    int listPos;
    struct BusIdListItem* next;
} BusIdListItem;

BusIdListItem *readBusIdsIntoList(char* busIdsString) {
    BusIdListItem *head = NULL;

    // split list of bus IDs
    char* busIdString = strtok(busIdsString, BUS_ID_DELIMITERS);
    int counter = 0;
    while(busIdString != NULL) {
        // create a new ListItem if busId is not "x"
        if(strcmp(busIdString, "x") != 0) {
            BusIdListItem *newItem = malloc(sizeof(BusIdListItem));
            newItem->id = atoi(busIdString);
            newItem->listPos = counter;
            newItem->next = NULL;

            // append at end of list
            if(head == NULL) {
                head = newItem;
            }
            else {
                BusIdListItem* curr = head;
                while (1) {
                    if(curr->next == NULL) {
                        curr->next = newItem;
                        break;
                    }
                    curr = curr->next;
                }
                
            }
            

        }
        busIdString = strtok(NULL, BUS_ID_DELIMITERS);
        counter++;
    }
    return head;
}

void clearList(BusIdListItem* list) {
    while (list != NULL)
    {
        BusIdListItem* temp = list->next;
        free(list);
        list = temp;
    }
}

int getEarliestDepartureBus(int earliestDepartTime, BusIdListItem* busIds) {
    int minTimeNextDeparture = INT_MAX;
    int returnVal = -1;
    BusIdListItem *currentBusId = busIds;

    // iterate all buses
    while(currentBusId != NULL) {
        // calculate time until next departure relative to earliestDepartTime
        int currTimeTillNextDeparture =  currentBusId->id - (earliestDepartTime % currentBusId->id);

        // return if the minimum of these times is found 
        if(currTimeTillNextDeparture < minTimeNextDeparture) {
            minTimeNextDeparture = currTimeTillNextDeparture;
            returnVal = (currentBusId->id * currTimeTillNextDeparture);
        }

        currentBusId = currentBusId->next;
    }

    return returnVal;
}


long solveContest(BusIdListItem* busIds) {
    BusIdListItem* currBus = busIds;

    // set initial tested time and step width by bus id of first bus in list 
    long currentTestedTime = currBus->id;
    long step = currBus->id;
    // step over the first as this is already correct with the current time
    currBus = currBus->next;
    // iterate all other buses
    while(currBus != NULL) {
        // increase time by step until the buses depart time matches time + list position
        while((currentTestedTime + currBus->listPos) % currBus->id != 0) {
            currentTestedTime += step;
        }
        // multiply the step with the bus ID as this guarantees that all previous depart times still match
        step *= currBus->id;
        currBus = currBus->next;
    }
    return currentTestedTime;
}

int main(int argc, char *argv[]) {

    if(argc != 2) {
        fprintf(stderr, "Error: exactly one argument needed, which is the path to the input file \n");
        return -1;
    }

    AocFile file;
    if(readFile(argv[1], INPUT_LINE_BUFFER_SIZE, &file) != 0) {
        perror("Error reading file: ");
        return -1;
    }
    int earliestDepartTime =  atoi(file.lines[0]);
    BusIdListItem *busIdList = readBusIdsIntoList(file.lines[1]);
    
    printf("Puzzle 1 Answer: %i\n", getEarliestDepartureBus(earliestDepartTime, busIdList));
    printf("Puzzle 2 Answer: %li\n", solveContest(busIdList));

    clearList(busIdList);
    closeFile(&file);
}