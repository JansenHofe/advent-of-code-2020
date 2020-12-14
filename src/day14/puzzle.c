#include "readFile.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define INPUT_LINE_BUFFER_SIZE 50

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

MemListItem* findAddress(MemListItem* listHead, long address) {
    MemListItem* curr = listHead;
    while (curr != NULL) {
        if(curr->mem.address == address) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

MemListItem* setMem(MemListItem* listHead, long address, long value) {
    MemListItem* memItem = findAddress(listHead, address);
    if(memItem != NULL) {
        memItem->mem.value = value;
        return listHead;
    }

    MemListItem* newItem = malloc(sizeof(MemListItem));
    newItem->mem.address = address;
    newItem->mem.value = value;
    newItem->next = listHead;
    return newItem;
}

BitMasks parseBitmasks(char* input) {
    char* maskStart = (input + 7);
    char prepareMaskString[37];
    char overwriteMaskString[37];
    for(int i = 0; i < 36; i++) {
        if(maskStart[i] == 'X') {
            prepareMaskString[i] = '1';
            overwriteMaskString[i] = '0';
        } else {
            prepareMaskString[i] = '0';
            overwriteMaskString[i] = maskStart[i];
        }
    }
    prepareMaskString[36] = '\0';
    overwriteMaskString[36] = '\0';

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

void recursive(char* maskString, long address, int* xPositions, int xPosLength, int currentArrayPos, long* addresses, int addressesSize) {

    if(currentArrayPos == xPosLength) {
        long addr = (address | strtol(maskString, NULL, 2));
        for(int i = 0;i < addressesSize; i++) {
            if(addresses[i] == -1) {
                addresses[i] = addr;
                return;
            }
        }
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

    recursive(perm1, address, xPositions, xPosLength, currentArrayPos, addresses, addressesSize);
    recursive(perm2, address, xPositions, xPosLength, currentArrayPos, addresses, addressesSize);
    
}

long* getMaskedAddresses(char* maskLine, long address, int* returnedAddressCount) {
    char* maskStart = (maskLine + 7);

    char origMask[37];
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

    long preMaskedAddress = (address & strtol(prepareMaskString, NULL,2));
    
    int numPermutations = pow(2, xPosCounter); 

    long* addresses = malloc(numPermutations * sizeof(long));
    memset(addresses, -1, numPermutations * sizeof(long));

    if(numPermutations == 1) {
        addresses[0] = (address | strtol(origMask, NULL, 2));
        (*returnedAddressCount) = 1;
        return addresses;

    }
    recursive(origMask, preMaskedAddress, xPositions, xPosCounter, 0, addresses, numPermutations);

    (*returnedAddressCount) = numPermutations;
    return addresses;
}

long getSumOfValuesInMemory(MemListItem* memory) {
    MemListItem* curr = memory;
    long sum = 0;
    while (curr != NULL) {
        sum += curr->mem.value;
        curr = curr->next;
    }
    return sum;
}

void clearMem(MemListItem *memory) {
    while(memory != NULL) {
        MemListItem* temp = memory->next;
        free(memory);
        memory = temp;
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

    BitMasks currentValueMasks = {.prepareMask=0, .overwriteMask=0};
    char* currentMaskString = "";
    MemListItem* memory = NULL;
    MemListItem* memory2 = NULL;

    for(int i = 0; i < file.lineCount; i++) {
        if(strncmp(file.lines[i], "mask", 4) == 0) {
            currentMaskString = file.lines[i];
            currentValueMasks = parseBitmasks(currentMaskString);
        } else if(strncmp(file.lines[i], "mem", 3) == 0) {
            AddressValuePair instr = parseSetMemInstruction(file.lines[i]);
            long maskedVal = ((instr.value & currentValueMasks.prepareMask) | currentValueMasks.overwriteMask);
            memory = setMem(memory, instr.address, maskedVal);

            int returnedAddressesCount;
            long* addresses = getMaskedAddresses(currentMaskString, instr.address, &returnedAddressesCount);
            for(int j = 0; j < returnedAddressesCount; j++) {
                memory2 = setMem(memory2, addresses[j], instr.value);
            }
            free(addresses);
        }
    }

    printf("Puzzle 1 Answer: %li\n", getSumOfValuesInMemory(memory));
    printf("Puzzle 2 Answer: %li\n", getSumOfValuesInMemory(memory2));

    clearMem(memory);
    clearMem(memory2);
    closeFile(&file);
}