#include "readFile.h"
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#define INPUT_LINE_BUFFER_SIZE 20
#define PREAMBLE_SIZE 25

int checkDataValid(long* data, int currentIndex) {
    // check if the sum of any pair of the previous x values equals value at currentIndex, with x = PREAMBLE_SIZE
    for(int i = (currentIndex - PREAMBLE_SIZE); i < currentIndex; i++) {
        for(int j = (i + 1); j < currentIndex; j++) {
            if((data[i] + data[j]) == data[currentIndex]) {
                return 1;
            }
        }
    }
    return 0;
}

long findWeakness(long* data, int invalidNumberIndex) {
    // find a contigous set of numbers in the previous values that adds up to the value at invalidNumberIndex
    // return the sum of min and max values in this set
    for(int i = 0; i < invalidNumberIndex; i++) {
        long sum = 0;
        long min = LONG_MAX;
        long max = LONG_MIN;

        for(int j = i; sum < data[invalidNumberIndex]; j++) {
            min = (data[j] < min ? data[j] : min);
            max = (data[j] > max ? data[j] : max);
            sum += data[j];
            if(sum == data[invalidNumberIndex]) {
                return (min + max);
            }
        }
    }
    return -1;
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

    long data[file.lineCount];
    long invalidNumber;
    long weakness;
    for(int i = 0; i < file.lineCount; i++) {
        data[i] = strtol(file.lines[i], NULL, 10);
        if(i >=PREAMBLE_SIZE && checkDataValid(data, i) == 0) {
            invalidNumber = data[i];
            weakness = findWeakness(data, i);
            break;
        }
    }

    printf("Puzzle 1 Answer: %li\n", invalidNumber);
    printf("Puzzle 2 Answer: %li\n", weakness);

    closeFile(&file);
}