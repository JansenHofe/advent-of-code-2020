#include "readFile.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INPUT_LINE_BUFFER_SIZE 150

#define PARENT_BAG_STRING_DELIMITER " bags contain"
#define PARENT_BAG_STRING_DELIMITER_SIZE 13
#define CHILD_BAG_DELIMITER " bag"
#define CHILD_DELIMITER ",\n"

typedef struct ChildBagListItem {
    char* name;
    int count;
    struct ChildBagListItem* next;
} ChildBagListItem;

typedef struct ParentBag {
    char* name;
    ChildBagListItem *childListHead;
    struct ParentBag* next;

} ParentBag;

// hash algorith "djb2" (modified), see http://www.cse.yorku.ca/~oz/hash.html
unsigned long hash(char *str, int size)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++) != 0)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return (hash % size);
}


void addToHashTable(ParentBag** table, int hashSize, ParentBag* bagToAdd) {
    int index = hash(bagToAdd->name, hashSize);
    ParentBag* curr = table[index];
    bagToAdd->next = curr;
    table[index] = bagToAdd;
}

ParentBag* findInHashTable(ParentBag** table, int hashSize, char* name) {
    int index = hash(name, hashSize);
    ParentBag* curr = table[index];
    while(curr != NULL) {
        if(strcmp(name, curr->name) == 0) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

ParentBag* parseBagRule(char *inputLine) {

    ParentBag* bag = malloc(sizeof(ParentBag));
    // terminate parent bag name
    char* parentBagEnd = strstr(inputLine, PARENT_BAG_STRING_DELIMITER);
    *parentBagEnd = '\0';
    bag->name = inputLine;
    bag->childListHead = NULL;

    // start point of children string
    char* childrenLine = parentBagEnd + PARENT_BAG_STRING_DELIMITER_SIZE;

    // return if no child bags are allowed
    if(strstr(childrenLine, "no other bags") != NULL) {
        return bag;
    }
    // iterate each child bag token
    char* childToken = strtok(childrenLine, CHILD_DELIMITER);
    while(childToken != NULL) {

        ChildBagListItem* currentItem = malloc(sizeof(ChildBagListItem));
        // parse count
        char* childNameToken;
        currentItem->count = strtol(childToken, &childNameToken, 10);
        
        //skip single whitespace
        childNameToken++;

        // terminate child name
        char* childNameEnd = strstr(childNameToken, CHILD_BAG_DELIMITER);
        *childNameEnd = '\0';

        //add child to list
        currentItem->name = childNameToken;
        currentItem->next = bag->childListHead;
        bag->childListHead = currentItem;

        childToken = strtok(NULL, CHILD_DELIMITER);
    }
    return bag;
}

void clearBagList(ParentBag** bags, int bagCount) {
    for(int i = 0; i < bagCount; i++) {
        ChildBagListItem *curr = bags[i]->childListHead;
        while (curr != NULL) {
            ChildBagListItem *toFree = curr;
            curr = curr->next;
            free(toFree);
        }
        free(bags[i]);
    }
}

int checkBagsContaining(ParentBag* bag, ParentBag** bagHashTable, int hashSize, char* searchedBagName) {
    
    ChildBagListItem *currChild = bag->childListHead;

    while(currChild != NULL) {

        // if the one child is the searched child return true
        if(strcmp(currChild->name, searchedBagName) == 0) {
            return 1;
        }
        ParentBag* searchedBag = findInHashTable(bagHashTable, hashSize, currChild->name);

        // search each bag recursively
        if(checkBagsContaining(searchedBag, bagHashTable, hashSize, searchedBagName)) {
            return 1;
        }
        currChild = currChild->next;
    }
    // if there are no children or none of them is the searched one, return false
    return 0;
}


int countTotalBags(ParentBag *bag, ParentBag** bagHashTable, int hashSize) {
    ChildBagListItem *currChild = bag->childListHead;
    int bagCounter = 1;

    // count in each sub bag recursively
    while(currChild != NULL) {
        ParentBag* searchedBag = findInHashTable(bagHashTable, hashSize, currChild->name);
        bagCounter +=  (currChild->count * countTotalBags(searchedBag, bagHashTable, hashSize));
        currChild = currChild->next;
    }
    return bagCounter;
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

    // create empty hashTable (here the table size equals the line count)
    int hashSize = file.lineCount;
    ParentBag* parentBagsTable[hashSize];
    for(int i = 0; i < hashSize; i++) {
        parentBagsTable[i] = NULL;
    }

    // add each parsed rule into the hashTable AND into a plain array (for better iteration)
    ParentBag* parentBags[file.lineCount];
    for(int i = 0; i < file.lineCount; i++) {
        ParentBag* current = parseBagRule(file.lines[i]);
        addToHashTable(parentBagsTable, hashSize, current);
        parentBags[i] = current;
    }

    int bagsWithShinyGoldSubBag = 0;

    for(int i = 0; i < file.lineCount; i++) {
        // count bags recursively containing "shiny gold" bag
        bagsWithShinyGoldSubBag += checkBagsContaining(parentBags[i], parentBagsTable, hashSize, "shiny gold");
    }
    
    // count total bags of bag "shiny gold" and substact 1, as the question does not ask for the "root" bag itself
    ParentBag* shinyGoldBag = findInHashTable(parentBagsTable, hashSize, "shiny gold");
    int subBagsOfShinyGold = (countTotalBags(shinyGoldBag, parentBagsTable, hashSize) - 1);

    printf("Puzzle 1 Answer: %i\n", bagsWithShinyGoldSubBag);
    printf("Puzzle 2 Answer: %i\n", subBagsOfShinyGold);

    clearBagList(parentBags, file.lineCount);
    closeFile(&file);
}