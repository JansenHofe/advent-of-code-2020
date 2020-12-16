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

    for(int i = NEARBY_TICKETS_START_LINE; i < file.lineCount; i++) {
        char* valueString = strtok(file.lines[i], ",\n");
        int idx = 0;
        while(valueString != NULL) {
            int value = atoi(valueString);
            for(int j = 0; j < numUnionizedRanges; j++) {
                if(value >= unionizedRanges[j][0] && value <= unionizedRanges[j][1]) {
                    break;
                }
                scanningErrorRate += value;
            }
            idx++; 
            valueString = strtok(NULL, ",\n");
        }
    }


    printf("Puzzle 1 Answer: %i\n", scanningErrorRate);
}