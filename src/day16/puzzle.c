#include "readFile.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INPUT_LINE_BUFFER_SIZE 100
#define NUM_TICKET_FIELDS 20
#define MY_TICKET_LINE (NUM_TICKET_FIELDS + 2)
#define NEARBY_TICKETS_START_LINE (MY_TICKET_LINE + 3)

typedef struct {
    char* name;
    int ranges[2][2];

} TicketField;

TicketField* parseTicketFields(char* input) {
    char* nameEnd = strchr(input, ':');
    char* rangesBegin = nameEnd + 1;
    *nameEnd = '\0';

    TicketField* ticketField = malloc(sizeof(TicketField));
    ticketField->name = input;
    sscanf(rangesBegin, "%i-%i or %i-%i", &ticketField->ranges[0][0], &ticketField->ranges[0][1], &ticketField->ranges[1][0], &ticketField->ranges[1][1]);
    return ticketField;
}

int isWithinFieldRanges(int value, TicketField* field) {
    //printf("check if %i is in (%i-%i) or (%i-%i)", value, field->ranges[0][0], field->ranges[0][1], field->ranges[1][0], field->ranges[1][1]);
    if((value >= field->ranges[0][0] && value <= field->ranges[0][1]) || (value >= field->ranges[1][0] && value <= field->ranges[1][1])) {
        //printf("--> true\n");
        return 1;
    }
    //printf("--> false\n");
    return 0;
}

static int comp(const void* p1, const void* p2) {
  int* arr1 = (int*)p1;
  int* arr2 = (int*)p2;
  int diff1 = arr1[0] - arr2[0];
  if (diff1) return diff1;
  return arr1[1] - arr2[1];
}

int** unionizeRanges(int numRanges, int ranges[numRanges][2], int* numUnionizedRange ) {
    qsort(ranges, numRanges, 2 * sizeof(int), comp);

    int** unionRanges = malloc(sizeof(int) * 2 * numRanges);
    unionRanges[0][0] = ranges[0][0];
    unionRanges[0][1] = ranges[0][1];
    int unionRangesCount = 1;
    for(int i = 1; i < numRanges; i++) {
        for(int j = 0; j < unionRangesCount; j++) {
            if(ranges[i][0] <= unionRanges[j][1]) {
                if(ranges[i][1] > unionRanges[j][1]) {
                    unionRanges[j][1] = ranges[i][1];
                }
            } else {
                unionRanges[unionRangesCount][0] = ranges[i][0];
                unionRanges[unionRangesCount][1] = ranges[i][1];
                unionRangesCount++;
            }
        }
    }
    *numUnionizedRange = unionRangesCount;
    return unionRanges;
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
    TicketField* fields[NUM_TICKET_FIELDS];
    int numRanges = NUM_TICKET_FIELDS * 2;
    int allRanges[numRanges][2];

    for(int i = 0; i < NUM_TICKET_FIELDS; i++) {
        fields[i] = parseTicketFields(file.lines[i]);
        allRanges[i][0] = fields[i]->ranges[0][0];
        allRanges[i][1] = fields[i]->ranges[0][1];
        allRanges[NUM_TICKET_FIELDS + i][0] = fields[i]->ranges[1][0];
        allRanges[NUM_TICKET_FIELDS + i][1] = fields[i]->ranges[1][1];

    }

    int numUnionizedRanges = 0;
    int** unionizedRanges = unionizeRanges(numRanges, allRanges, &numUnionizedRanges);

    int scanningErrorRate = 0;

    int fieldToPosMatrix[NUM_TICKET_FIELDS][NUM_TICKET_FIELDS];
    for(int i = 0; i < NUM_TICKET_FIELDS; i++) {
        for(int j = 0; j < NUM_TICKET_FIELDS; j++) {
            fieldToPosMatrix[i][j] = 1;
        }
    }

    for(int i = NEARBY_TICKETS_START_LINE; i < file.lineCount; i++) {
        char* valueString = strtok(file.lines[i], ",\n");
        int idx = 0;
        int valid = 1;
        int ticket[NUM_TICKET_FIELDS];
        while(valueString != NULL) {
            int value = atoi(valueString);
            ticket[idx] = value;
            idx++; 

            for(int j = 0; j < numUnionizedRanges; j++) {
                if(value >= unionizedRanges[j][0] && value <= unionizedRanges[j][1]) {
                    break;
                }
                scanningErrorRate += value;
                valid = 0;
            }
            valueString = strtok(NULL, ",\n");
        }

        if(valid) {
            for(int availableFieldsIdx = 0; availableFieldsIdx < NUM_TICKET_FIELDS; availableFieldsIdx++) {
                for(int currTicketFieldIdx = 0; currTicketFieldIdx < NUM_TICKET_FIELDS; currTicketFieldIdx++) {
                    if(!isWithinFieldRanges(ticket[currTicketFieldIdx], fields[availableFieldsIdx])) {
                        fieldToPosMatrix[availableFieldsIdx][currTicketFieldIdx] = 0;
                    }
                }
            }
        }
    }

    int positionsInTicket [NUM_TICKET_FIELDS];
    for(int i = 0; i < NUM_TICKET_FIELDS; i++) {
        positionsInTicket[i] = -1;
    }

    for(int i = 0; i < NUM_TICKET_FIELDS; i++) {
        int possibilitiesForField = 0;
        int posLastPossibility = -1;
        for(int j = 0; j < NUM_TICKET_FIELDS; j++) {
            if(fieldToPosMatrix[i][j] == 1) {
                 possibilitiesForField += 1;
                 posLastPossibility = j;
            }
        }
        if(possibilitiesForField == 1 && positionsInTicket[i] == -1) {

            positionsInTicket[i] = posLastPossibility;
            for(int k = 0; k < NUM_TICKET_FIELDS; k++) {
                if(k != i) {
                    fieldToPosMatrix[k][posLastPossibility] = 0;
                }
            }
            i = -1;
        }
    }

    int myTicket[NUM_TICKET_FIELDS];
    int idx = 0;

    char* myTicketValueString = strtok(file.lines[MY_TICKET_LINE], ",\n");
    while (myTicketValueString != NULL) {
        int value = atoi(myTicketValueString);
        myTicket[idx] = value;
        idx++;
        myTicketValueString = strtok(NULL, ",\n");
    }

    long myTicketDeptartureValueProduct = 1;
    for(int i = 0; i < NUM_TICKET_FIELDS; i++) {
        if(strstr(fields[i]->name, "departure") != NULL) {
            int fieldId = positionsInTicket[i];
            myTicketDeptartureValueProduct *= myTicket[fieldId];
        }
    }


    printf("Puzzle 1 Answer: %i\n", scanningErrorRate);
    printf("Puzzle 2 Answer: %li\n", myTicketDeptartureValueProduct);
}