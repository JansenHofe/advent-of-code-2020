#include "readFile.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define INPUT_LINE_BUFFER_SIZE 50
#define HASH_SIZE 1000
#define MASK_SIZE 36

typedef struct {
    long prepareMask;
    long overwriteMask;
} BitMasks;

typedef struct {
    long address;
    long value;
} AddressValuePair;

typedef struct MemListItem{
    AddressValuePair mem;
    struct MemListItem* next;
} MemListItem;


void addToHashTable(MemListItem** table, int hashSize, long address, long value) {
    int index = address % hashSize;
    MemListItem* curr = table[index];

    if(curr == NULL) {
        MemListItem* toAdd = malloc(sizeof(MemListItem));
        toAdd->mem.address = address;
        toAdd->mem.value = value;
        toAdd->next = NULL;

        table[index] = toAdd;
        return;
    }

    while(curr != NULL) {
        if(curr->mem.address == address) {
            curr->mem.value = value;
            return;
        }
        if(curr->next == NULL) {
            MemListItem* toAdd = malloc(sizeof(MemListItem));
            toAdd->mem.address = address;
            toAdd->mem.value = value;
            toAdd->next = NULL;
            curr->next = toAdd;
        }
        curr = curr->next;
    }
}

long getSumOfValuesInHashTable(MemListItem** table, int hashSize) {
    long sum = 0;
    for(int i = 0; i < hashSize; i++) {
        MemListItem* curr = table[i];

        while(curr != NULL) {
            sum += curr->mem.value;
            curr = curr->next;
        }
    }
    return sum;
}

void clearHashTable(MemListItem** table, int hashSize) {
    for(int i = 0; i < hashSize; i++) {
        MemListItem* curr = table[i];

        while(curr != NULL) {
            MemListItem* temp = curr->next;
            free(curr);
            curr = temp;
        }
    }
}

BitMasks parseBitmasks(char* input) {
    char* maskStart = (input + 7);
    char prepareMaskString[MASK_SIZE + 1];
    char overwriteMaskString[MASK_SIZE + 1];

    for(int i = 0; i < MASK_SIZE; i++) {
        if(maskStart[i] == 'X') {
            prepareMaskString[i] = '1';
            overwriteMaskString[i] = '0';
        } else {
            prepareMaskString[i] = '0';
            overwriteMaskString[i] = maskStart[i];
        }
    }
    prepareMaskString[MASK_SIZE] = '\0';
    overwriteMaskString[MASK_SIZE] = '\0';

    BitMasks masks = {
        .overwriteMask=strtol(overwriteMaskString, NULL,2),
        .prepareMask=strtol(prepareMaskString, NULL, 2)};
    return masks;
}

AddressValuePair parseSetMemInstruction(char* input) {
    long addr;
    long value;
    sscanf(input, "mem[%li] = %li", &addr, &value);

    AddressValuePair instr = {.address=addr, .value=value};
    return instr;
}


// recursively create permutations of addresses and set the memory at these addresses to value
void setMemByMaskPermutations(char* maskString, long baseAddress, int* xPositions, int xPosLength, int currentArrayPos, MemListItem** memory, long value) {

    if(currentArrayPos == xPosLength) {
        long addr = (baseAddress | strtol(maskString, NULL, 2));
        addToHashTable(memory, HASH_SIZE, addr, value);
        return;
    }
    int currentXPos = xPositions[currentArrayPos];

    char perm1[37];
    char perm2[37];
    strcpy(perm1, maskString);
    strcpy(perm2, maskString);

    perm1[currentXPos] = '0';
    perm2[currentXPos] = '1';

    currentArrayPos++;

    setMemByMaskPermutations(perm1, baseAddress, xPositions, xPosLength, currentArrayPos, memory, value);
    setMemByMaskPermutations(perm2, baseAddress, xPositions, xPosLength, currentArrayPos, memory, value);
    
}

void setMaskedAddresses(char* maskLine, long address, MemListItem** memory, long value) {
    char* maskStart = (maskLine + 7);

    char origMask[37];

    // create prepareMask that sets all bits of address to 0 where mask has floating bit (X)
    // remember positions of floating bits in mask
    char prepareMaskString[37];
    int xPositions[36];
    int xPosCounter = 0;

    for(int i = 0; i < 36; i++) {
        if(maskStart[i] == 'X') {
            xPositions[xPosCounter] = i;
            xPosCounter++;
            prepareMaskString[i] = '0';
        } else {
            prepareMaskString[i] = '1';
        }
        origMask[i] = maskStart[i];
    }
    prepareMaskString[36] = '\0';
    origMask[36] = '\0';

    // mask with prepareMask
    long preMaskedAddress = (address & strtol(prepareMaskString, NULL,2));
    
    // set value at all permutations of addresses
    setMemByMaskPermutations(origMask, preMaskedAddress, xPositions, xPosCounter, 0, memory, value);
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

    BitMasks currentValueMasks = {.prepareMask=0, .overwriteMask=0};
    char* currentMaskString = "";

    // memory represented by hashtables
    MemListItem* memoryTable1[HASH_SIZE];
    MemListItem* memoryTable2[HASH_SIZE];
    for(int i = 0; i < HASH_SIZE; i++) {
        memoryTable1[i] = NULL;
        memoryTable2[i] = NULL;
    }

    for(int i = 0; i < file.lineCount; i++) {
        if(strncmp(file.lines[i], "mask", 4) == 0) {
            // save current mask
            currentMaskString = file.lines[i];
            currentValueMasks = parseBitmasks(currentMaskString);
        } else if(strncmp(file.lines[i], "mem", 3) == 0) {
            // parse instructions
            AddressValuePair instr = parseSetMemInstruction(file.lines[i]);
            // mask values and add to memory for puzzle 1
            long maskedVal = ((instr.value & currentValueMasks.prepareMask) | currentValueMasks.overwriteMask);
            addToHashTable(memoryTable1, HASH_SIZE, instr.address, maskedVal);

            // set value at all address permutations produced by masking for puzzle 2 
            setMaskedAddresses(currentMaskString, instr.address, memoryTable2, instr.value);
        }
    }

    printf("Puzzle 1 Answer: %li\n", getSumOfValuesInHashTable(memoryTable1, HASH_SIZE));
    printf("Puzzle 2 Answer: %li\n", getSumOfValuesInHashTable(memoryTable2, HASH_SIZE));

    clearHashTable(memoryTable1, HASH_SIZE);
    clearHashTable(memoryTable2, HASH_SIZE);
    closeFile(&file);
}