#include "readFile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_LINE_BUFFER_SIZE 5
#define JOLTAGE_TOLCERANCE 3


int sortIntegerAsc (const void* a, const void* b) {
   return ( *(int*)a - *(int*)b );
}

int useAllAdapters(int* joltages, int numJoltages) {

    int joltageDiffs[JOLTAGE_TOLCERANCE + 1];
    memset(joltageDiffs, 0, sizeof joltageDiffs);

    // count every diff between each consecutive two adapters
    for(int i = 1; i < numJoltages; i++) {
        int diff = joltages[i] - joltages[i -1];

        joltageDiffs[diff]++;
    }

    // return the number of joltageDiffs with value 1 multiplied by the joltageDiffs with value 3
    return (joltageDiffs[1] * joltageDiffs[3]);
}

long findDistinctAdapterCombinations(int* joltages, int numJoltages, int startIdx, long* cominationsMem) {
    
    // if the last item is reached (the device itself), this combination is complete
    if(startIdx == numJoltages - 1) {
        return 1;
    }

    // if the number of following combinations from the current position was already calculated, return it
    if(cominationsMem[startIdx] != -1) {
        return cominationsMem[startIdx];
    } else {

        long count = 0;
        // recursively find the number of combinations for each of the following possible adapters (if they are in tolerance)
        for(int i = (startIdx + 1); (i <= (startIdx + JOLTAGE_TOLCERANCE) && i < numJoltages); i++) {
            if(joltages[i] - joltages[startIdx] <= JOLTAGE_TOLCERANCE) {
                count += findDistinctAdapterCombinations(joltages, numJoltages, i, cominationsMem);
            }
        }
        // save the number of combinations for the current index, so it does not have to be recalculated the next time it is reached
        cominationsMem[startIdx] = count;
        return count;
    }

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

    // put all joltages in array...
    int adapterJoltageCount = file.lineCount + 2;
    int adapterJoltages[adapterJoltageCount];
    for(int i = 0; i < file.lineCount; i++) {
        adapterJoltages[i + 1] = atoi(file.lines[i]);
    }
    // sort it...
    qsort((adapterJoltages + 1), file.lineCount, sizeof(int), sortIntegerAsc);
    // and add the outlet joltage at the start and device joltage at the end
    adapterJoltages[0] = 0;
    adapterJoltages[adapterJoltageCount - 1] = adapterJoltages[adapterJoltageCount - 2] + JOLTAGE_TOLCERANCE;

    long combinationsMemory[adapterJoltageCount];
    memset(combinationsMemory, -1, sizeof combinationsMemory);

    int allAdapterResult = useAllAdapters(adapterJoltages, adapterJoltageCount);
    long distinctAdapterCombinations = findDistinctAdapterCombinations(adapterJoltages, adapterJoltageCount, 0, combinationsMemory);
    
    printf("Puzzle 1 Answer: %i\n", allAdapterResult);
    printf("Puzzle 2 Answer: %li\n", distinctAdapterCombinations);
    closeFile(&file);
}