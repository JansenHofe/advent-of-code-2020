#include "readFile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_LINE_BUFFER_SIZE 50

int validPasswordsOldPolicy = 0;
int validPasswordsNewPolicy = 0;

// checks if atleast minCount and at most maxCount letters of neededLetter are in password
void checkPasswordValidOldPolicy(char* password, char neededLetter, int minCount, int maxCount) {
    int matches = 0;
    char* match = strchr(password, neededLetter);
    while(match != NULL) {
        matches++;
        match = strchr(match + 1, neededLetter);
    }
    if(matches >= minCount && matches <= maxCount) {
        validPasswordsOldPolicy++;
    }
}

// checks if exactly one of the letters of password at postions lowerPosition and upperPosition equals neededLetter
// positions starting at 1 (not zero-index!)
void checkPasswordValidNewPolicy(char* password, char neededLetter, int lowerPosition, int upperPosition) {
    if((password[lowerPosition - 1] == neededLetter || password[upperPosition - 1] == neededLetter)
        && !(password[lowerPosition - 1] == neededLetter && password[upperPosition - 1] == neededLetter)) {
        validPasswordsNewPolicy++;
    }
}

void checkValidPasswords(char* line) {
    int lower, upper;
    char neededLetter;
    char* password;

    char* delimiters = "- :\n";
    char* ptr = strtok(line, delimiters);
    lower = atoi(ptr);

    ptr = strtok(NULL, delimiters);
    upper = atoi(ptr);

    ptr = strtok(NULL, delimiters);
    neededLetter = ptr[0];

    ptr = strtok(NULL, delimiters);
    password = ptr;

    checkPasswordValidOldPolicy(password, neededLetter, lower, upper);
    checkPasswordValidNewPolicy(password, neededLetter, lower, upper);
}

int main(int argc, char *argv[]) {

    if(argc != 2) {
        fprintf(stderr, "Error: exactly one argument needed, which is the path to the input file \n");
        return -1;
    }
    int result = executeOnEachLine(argv[1], INPUT_LINE_BUFFER_SIZE, checkValidPasswords);
    if(result != 0) {
        perror("Error processing file: ");
        return -1;
    }

    printf("Puzzle 1 answer: %i\n", validPasswordsOldPolicy);
    printf("Puzzle 2 answer: %i\n", validPasswordsNewPolicy);
}