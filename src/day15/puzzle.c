#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define START_SEQUENCE_SIZE 6

#define TARGET_PUZZLE_1 2020
#define TARGET_PUZZLE_2 30000000
#define HASH_TABLE_SIZE 10000000

typedef struct IntListNode {
    int time;
    int number;
    struct IntListNode* next;
} IntListNode;


IntListNode* findInHashTable(IntListNode** table, int hashSize, int number) {
    int tableIdx = number % hashSize;
    IntListNode* curr = table[tableIdx];
    while(curr != NULL) {
        if(curr->number == number) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

void addToHashTable(IntListNode** table, int hashSize, int time, int number) {
    int tableIdx = number % hashSize;

    IntListNode* newNode = malloc(sizeof(IntListNode));
    newNode->time = time;
    newNode->number = number;

    IntListNode* listHead = table[tableIdx];
    newNode->next = listHead;
    table[tableIdx] = newNode;
}

void clearHashTable(IntListNode ** table, int hashSize) {
    for(int i = 0; i < hashSize; i++) {
        IntListNode* curr = table[i];
        while(curr != NULL) {
            IntListNode* temp = curr->next;
            free(curr);
            curr = temp;
        }
    }
}

int main() {

    int hashSize = HASH_TABLE_SIZE;
    IntListNode** numberHashTable = malloc(sizeof(IntListNode) * hashSize);
    for(int i = 0; i < hashSize; i++) {
        numberHashTable[i] = NULL;
    }

    int startSequence[START_SEQUENCE_SIZE] = {6,3,15,13,1,0};

    int lastNumber = startSequence[0];

    for(int i = 1; i < START_SEQUENCE_SIZE; i++) {
        addToHashTable(numberHashTable, hashSize, (i - 1), lastNumber);
        lastNumber = startSequence[i];
    }
    for(int i = START_SEQUENCE_SIZE; i < TARGET_PUZZLE_2; i++) {
        int nextNumber;
        IntListNode* lastNumberNode = findInHashTable(numberHashTable, hashSize, lastNumber);
        if(lastNumberNode == NULL) {
            nextNumber = 0;
            addToHashTable(numberHashTable, hashSize, (i - 1), lastNumber);
        } else {
            int diff = (i - 1) - lastNumberNode->time;
            nextNumber = diff;
            lastNumberNode->time = (i - 1);
            
        }
        if(i == (TARGET_PUZZLE_1)) {
            printf("Puzzle 1 Answer: %i\n", lastNumber);   
        }

        lastNumber = nextNumber;
    }

    clearHashTable(numberHashTable, hashSize);
    free(numberHashTable);
    printf("Puzzle 2 Answer: %i\n", lastNumber);

}