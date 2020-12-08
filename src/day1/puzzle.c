#include "readFile.h"
#include <stdio.h>
#include <stdlib.h>

#define EXPENSES_TARGET_SUM 2020
#define INPUT_LINE_BUFFER_SIZE 10

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
    
    int expenses[file.lineCount];
    for(int i = 0; i < file.lineCount; i++) {
        expenses[i] = atoi(file.lines[i]);
    }

    int firstAnswerFound = 0, secondAnswerFound = 0;

    for(int i = 0; i < file.lineCount; i++) {
        for(int j = i; j < file.lineCount; j++) {
            if((expenses[i] + expenses[j]) == EXPENSES_TARGET_SUM) {
                printf("Puzzle 1 answer: %i\n", (expenses[i] * expenses[j]));
                firstAnswerFound = 1;
                if(secondAnswerFound) {
                    closeFile(&file);
                    return 0;
                }
            }

            for(int k = j; k < file.lineCount; k++) {
                if((expenses[i] + expenses[j] + expenses[k]) == EXPENSES_TARGET_SUM) {
                    printf("Puzzle 2 answer: %i\n", (expenses[i] * expenses[j] * expenses[k]));
                }
                secondAnswerFound = 1;
                if(firstAnswerFound) {
                    closeFile(&file);
                    return 0;
                }
            }
        }
    }
}