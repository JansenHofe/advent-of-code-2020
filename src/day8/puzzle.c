#include "readFile.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INPUT_LINE_BUFFER_SIZE 10

#define INSTRUCTION_JUMP            "jmp"
#define INSTRUCTION_ACCUMULATOR     "acc"
#define INSTRUCTION_NO_OPERATION    "nop"
#define INSTRUCTION_LENGTH 3

typedef enum {
    REGULAR_TERMINATION,
    INFINITE_LOOP,
    PC_OUT_OF_BOUNDS
} ProgramEndReason;

typedef struct {
    ProgramEndReason reason;
    int accumulator; 
} ProgramEnd;

void processInstruction(char* instr, int* progCounter, int* accumulator) {
    int value = strtol((instr + INSTRUCTION_LENGTH), NULL, 10);

    if(strncmp(instr, INSTRUCTION_JUMP, INSTRUCTION_LENGTH) == 0) {
        *progCounter += value;
        return;
    } else if(strncmp(instr, INSTRUCTION_ACCUMULATOR, INSTRUCTION_LENGTH) == 0) {
        *accumulator += value;
    }
    (*progCounter)++;
}

ProgramEnd runProgram(char** instructions, int numInstructions) {
    int progCounter = 0;
    int accumulator = 0;
    // counts how often each instruction is run
    int executionCounter [numInstructions];
    memset(executionCounter, 0, sizeof executionCounter);

    while(1) {
        // check end conditions
        if(progCounter == numInstructions) {
            ProgramEnd end = {.accumulator = accumulator, .reason = REGULAR_TERMINATION};
            return end;
        } else if(progCounter < 0 || progCounter > numInstructions) {
            ProgramEnd end = {.accumulator = accumulator, .reason = PC_OUT_OF_BOUNDS};
            return end;
        } else if(executionCounter[progCounter] > 0) {
            ProgramEnd end = {.accumulator = accumulator, .reason = INFINITE_LOOP};
            return end;
        }

        char *currInstr = instructions[progCounter];
        executionCounter[progCounter]++;
        processInstruction(currInstr, &progCounter, &accumulator);
    }
}

int runProgramWithReplacement(char** program, int programLength, int* acc, char* instrToReplace, char* initial, char* replacement) {
    ProgramEnd endAfterReplacement;
    // replace instruction, run program and undo the replacement afterwards
    strncpy(instrToReplace, replacement, INSTRUCTION_LENGTH);
    endAfterReplacement = runProgram(program, programLength);
    strncpy(instrToReplace, initial, INSTRUCTION_LENGTH);
    // if program terminated regularly, save accumulator and return true
    if(endAfterReplacement.reason == REGULAR_TERMINATION) {
        *acc = endAfterReplacement.accumulator;
        return 1;
    }
    return 0;
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
    // initially the program will run into infinite loop and end
    ProgramEnd inifiniteLoopEnd = runProgram(file.lines, file.lineCount);

    int accAfterSingleChange = 0;
    // run program again for every NOP or JMP...
    for(int i = 0; i < file.lineCount; i++) {
        char *currInstr = file.lines[i];
        // ..if current instruction is NOP or JMP replace it with the other and check if program terminated regularly
        if((strncmp(currInstr, INSTRUCTION_JUMP, INSTRUCTION_LENGTH) == 0
            && runProgramWithReplacement(file.lines, file.lineCount, &accAfterSingleChange, currInstr, INSTRUCTION_JUMP, INSTRUCTION_NO_OPERATION))
          || (strncmp(currInstr, INSTRUCTION_NO_OPERATION, INSTRUCTION_LENGTH) == 0
            && runProgramWithReplacement(file.lines, file.lineCount, &accAfterSingleChange, currInstr, INSTRUCTION_NO_OPERATION, INSTRUCTION_JUMP))) {
            break;
        }
    }

    printf("Puzzle 1 Answer: %i\n", inifiniteLoopEnd.accumulator);
    printf("Puzzle 2 Answer: %i\n", accAfterSingleChange);

    closeFile(&file);
}