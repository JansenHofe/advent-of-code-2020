#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define START_SEQUENCE_SIZE 6

#define TARGET_PUZZLE_1 2020
#define TARGET_PUZZLE_2 30000000

int main() {
    int* memory = malloc(sizeof(int) * TARGET_PUZZLE_2);
    memset(memory, -1, sizeof(int) * TARGET_PUZZLE_2);

    int startSequence[START_SEQUENCE_SIZE] = {6,3,15,13,1,0};

    int lastNumber = startSequence[0];

    for(int i = 1; i < START_SEQUENCE_SIZE; i++) {
        memory[lastNumber] = (i-1);
        lastNumber = startSequence[i];
    }
    for(int i = START_SEQUENCE_SIZE; i < TARGET_PUZZLE_2; i++) {
        int nextNumber;

        if(memory[lastNumber] == -1) {
            nextNumber = 0;
        } else {
            int diff = (i - 1) - memory[lastNumber];
            nextNumber = diff;
            
        }
        memory[lastNumber] = (i - 1);
        if(i == (TARGET_PUZZLE_1)) {
            printf("Puzzle 1 Answer: %i\n", lastNumber);   
        }

        lastNumber = nextNumber;
    }
    printf("Puzzle 2 Answer: %i\n", lastNumber);
    free(memory);
}