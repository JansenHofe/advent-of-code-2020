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

int* parseTicket(char* input) {
    int* ticket = malloc(sizeof(int) * NUM_TICKET_FIELDS);
    char* valueString = strtok(input, ",\n");
    int idx = 0;
    while(valueString != NULL) {
        int value = atoi(valueString);
        ticket[idx] = value;
        idx++; 
        valueString = strtok(NULL, ",\n");
    }
    return ticket;
}

int isWithinFieldRanges(int value, TicketField* field) {
    if((value >= field->ranges[0][0] && value <= field->ranges[0][1]) || (value >= field->ranges[1][0] && value <= field->ranges[1][1])) {
        return 1;
    }
    return 0;
}

int getTicketErrorRate(TicketField** ticketfields, int* ticket) {
    int errorRate = 0;
    for(int i = 0; i < NUM_TICKET_FIELDS; i++) {
        int currTicketFieldValid = 0;
        for(int j = 0; j < NUM_TICKET_FIELDS; j++) {
            if(isWithinFieldRanges(ticket[i], ticketfields[j])) {
                currTicketFieldValid = 1;
                break;
            }
        }
        if(!currTicketFieldValid) {
            errorRate += ticket[i];
        }
    }
    return errorRate;
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

    // parse field definitions with its ranges
    TicketField* fields[NUM_TICKET_FIELDS];
    for(int i = 0; i < NUM_TICKET_FIELDS; i++) {
        fields[i] = parseTicketFields(file.lines[i]);

    }
    // parse my tickets
    int* myTicket = parseTicket(file.lines[MY_TICKET_LINE]);

    // create field-to-position matrix, that defines which field definition matches which position in a ticket
    // initialize it with all 1
    int fieldToPosMatrix[NUM_TICKET_FIELDS][NUM_TICKET_FIELDS];
    for(int i = 0; i < NUM_TICKET_FIELDS; i++) {
        for(int j = 0; j < NUM_TICKET_FIELDS; j++) {
            fieldToPosMatrix[i][j] = 1;
        }
    }

    int scanningErrorRate = 0;
    for(int i = NEARBY_TICKETS_START_LINE; i < file.lineCount; i++) {
        // parse each ticket and get its error rate
        int* ticket = parseTicket(file.lines[i]);
        int errorRate = getTicketErrorRate(fields, ticket);
        scanningErrorRate += errorRate;

        // if this ticket is valid, update the field-to-position matrix to remove impossible combinations
        if(!errorRate) {
            for(int availableFieldsIdx = 0; availableFieldsIdx < NUM_TICKET_FIELDS; availableFieldsIdx++) {
                for(int currTicketFieldIdx = 0; currTicketFieldIdx < NUM_TICKET_FIELDS; currTicketFieldIdx++) {
                    if(!isWithinFieldRanges(ticket[currTicketFieldIdx], fields[availableFieldsIdx])) {
                        fieldToPosMatrix[availableFieldsIdx][currTicketFieldIdx] = 0;
                    }
                }
            }
        }
        free(ticket);
    }

    // create array that defines which field definition (array index) belongs to which position in ticket (value)
    int positionsInTicket [NUM_TICKET_FIELDS];
    for(int i = 0; i < NUM_TICKET_FIELDS; i++) {
        positionsInTicket[i] = -1;
    }

    // iterate the field-to-position matrix
    for(int i = 0; i < NUM_TICKET_FIELDS; i++) {
        int possibilitiesForField = 0;
        int posLastPossibility = -1;
        // count how many positions are possible for the current field
        // and remember the last possible position
        for(int j = 0; j < NUM_TICKET_FIELDS; j++) {
            if(fieldToPosMatrix[i][j] == 1) {
                 possibilitiesForField += 1;
                 posLastPossibility = j;
            }
        }
        // if the field has only 1 possible position and is not already known, save it
        // -> the position for this field is now known
        if(possibilitiesForField == 1 && positionsInTicket[i] == -1) {
            positionsInTicket[i] = posLastPossibility;

            // as this position is reserved, it is erased as a possibility for all other fields 
            for(int k = 0; k < NUM_TICKET_FIELDS; k++) {
                if(k != i) {
                    fieldToPosMatrix[k][posLastPossibility] = 0;
                }
            }
            // then we start the outer iteration from the beginning
            i = -1;
        }
    }

    // multiply all values of fields in my ticket, that contain "departure"
    long myTicketDeptartureValueProduct = 1;
    for(int i = 0; i < NUM_TICKET_FIELDS; i++) {
        if(strstr(fields[i]->name, "departure") != NULL) {
            int fieldId = positionsInTicket[i];
            myTicketDeptartureValueProduct *= myTicket[fieldId];
        }
    }


    printf("Puzzle 1 Answer: %i\n", scanningErrorRate);
    printf("Puzzle 2 Answer: %li\n", myTicketDeptartureValueProduct);
    for(int i = 0; i < NUM_TICKET_FIELDS; i++) {
        free(fields[i]);
    }
    free(myTicket);
    closeFile(&file);
}