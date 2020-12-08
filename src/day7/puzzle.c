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

} ParentBag;

void parseBagRule(char *inputLine, ParentBag* bag) {

        // terminate parent bag name
        char* parentBagEnd = strstr(inputLine, PARENT_BAG_STRING_DELIMITER);
        *parentBagEnd = '\0';
        bag->name = inputLine;
        bag->childListHead = NULL;

        // start point of children string
        char* childrenLine = parentBagEnd + PARENT_BAG_STRING_DELIMITER_SIZE;

        // return if no child bags are allowed
        if(strstr(childrenLine, "no other bags") != NULL) {
            return;
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
}

void clearBagList(ParentBag *bags, int bagCount) {
     for(int i = 0; i < bagCount; i++) {
        ChildBagListItem *curr = bags[i].childListHead;
        while (curr != NULL) {
            ChildBagListItem *toFree = curr;
            curr = curr->next;
            free(toFree);
        }
    }
}

int checkBagsContaining(ParentBag *bag, ParentBag* allBags, int allBagsCount, char* searchedBag) {
    
    ChildBagListItem *currChild = bag->childListHead;

    while(currChild != NULL) {

        // if the one child is the searched child return true
        if(strcmp(currChild->name, searchedBag) == 0) {
            return 1;
        }
        for(int i = 0; i < allBagsCount; i++) {
            // search current child in parent list and check recurively for that one
            if(strcmp(currChild->name, allBags[i].name) == 0) {
                if(checkBagsContaining(&allBags[i], allBags, allBagsCount, searchedBag)) {
                    return 1;
                }
                break;
            }
        }
        currChild = currChild->next;
    }
    // if there are no children or none of them is the searched one, return false
    return 0;
}


int countTotalBags(ParentBag *bag, ParentBag* allBags, int allBagsCount) {
    ChildBagListItem *currChild = bag->childListHead;
    int bagCounter = 1;
    while(currChild != NULL) {
        for(int i = 0; i < allBagsCount; i++) {
            // search current child in parent list and check recurively for that one
            if(strcmp(currChild->name, allBags[i].name) == 0) {
                // multiply count of bags in each child with the count of children of this kind
                bagCounter += (currChild->count * countTotalBags(&allBags[i], allBags, allBagsCount));
                break;
            }
        }
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

    // for better performance, the parent-child relation of the bags should be implemented as a map/dict for faster lookup
    ParentBag parentBags[file.lineCount];

    for(int i = 0; i < file.lineCount; i++) {
        parseBagRule(file.lines[i], &parentBags[i]);
    }

    int bagsWithShinyGoldSubBag = 0;
    ParentBag *myBag;

    for(int i = 0; i < file.lineCount; i++) {

        // count bags recursively containing "shiny gold" bag
        bagsWithShinyGoldSubBag += checkBagsContaining(&parentBags[i], parentBags, file.lineCount, "shiny gold");

        // remember the "shiny gold" bag for puzzle 2
        if(strcmp(parentBags[i].name, "shiny gold") == 0) {
            myBag = &parentBags[i];
        }
    }
    // count total bags of bag "shiny gold" and substact 1, as the question does not ask for the "root" bag itself
    int subBagsOfShinyGold = (countTotalBags(myBag, parentBags, file.lineCount) - 1);

    printf("Puzzle 1 Answer: %i\n", bagsWithShinyGoldSubBag);
    printf("Puzzle 2 Answer: %i\n", subBagsOfShinyGold);

    clearBagList(parentBags, file.lineCount);
    closeFile(&file);
}