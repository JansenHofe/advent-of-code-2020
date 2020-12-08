#include "readFile.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INPUT_LINE_BUFFER_SIZE 12
#define ROW_STRING_LENGTH 7
#define COL_STRING_LENGTH 3

#define SEAT_ID_MULTIPLIER 8

// seat numbers are binary encoded strings, but instead of 0s and 1s, other chars are used
int seatStringToNumber(char* seatString, char toZeroChar) {
    for(int i  = 0; i < strlen(seatString); i++) {
        if(seatString[i] == toZeroChar){
            seatString[i] = '0';
        } else {
            seatString[i] = '1';
        }
    }

    return strtol(seatString, NULL, 2);
}

int calculateSeatId(char* input) {
    char rowString[ROW_STRING_LENGTH + 1] = "";
    char colString[COL_STRING_LENGTH + 1] = "";

    // split whole seat number string into row and column parts
    strncpy(rowString, input, ROW_STRING_LENGTH);
    strncpy(colString, (input + ROW_STRING_LENGTH), COL_STRING_LENGTH);

    // convert 'binary space partitioning' to integer seat id
    int row = seatStringToNumber(rowString, 'F');
    int col = seatStringToNumber(colString, 'L');

    return (row * SEAT_ID_MULTIPLIER + col);
}

int sortIntegerAsc (const void* a, const void* b) {
   return ( *(int*)a - *(int*)b );
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

    int seatIds[file.lineCount];
    int maxSeatId = 0;

    // calculate max seat number and put each seat number int array 'seatIds'
    for(int i = 0; i < file.lineCount; i++) {
        int currentSeatId = calculateSeatId(file.lines[i]);
        seatIds[i] = currentSeatId;

        if(currentSeatId > maxSeatId) {
            maxSeatId = currentSeatId;
        }   
    }

    // sort array 'seatIds'
    qsort(seatIds, file.lineCount, sizeof(int), sortIntegerAsc);

    // look for step of two between each subsequent seat IDs, this 'missing' ID is mine
    int mySeatId = -1;
    for(int i = 1; i < file.lineCount; i++) {
        if(seatIds[i] - seatIds[i -1] == 2) {
            mySeatId = seatIds[i] - 1;
            break;
        }
    }

    printf("Puzzle 1 Answer: %i\n", maxSeatId);
    printf("Puzzle 2 Answer: %i\n", mySeatId);

    closeFile(&file);
}