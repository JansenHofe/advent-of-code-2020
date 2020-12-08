#include "readFile.h"
#include "passportDefinitions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define INPUT_LINE_BUFFER_SIZE 100


void putKeyValueIntoPassport(char** passport, char* keyValue) {

    // split key-value pair by delimiter by replacing it with null terminator
    char* delimiterChar = strchr(keyValue, KEY_VALUE_DELIMITER);
    delimiterChar[0] = '\0';
    char* key = keyValue;
    char* value = delimiterChar + 1;

    //put values into passport in correct order 
    for(int i = 0; i < NUM_PASSPORT_FIELDS; i++) {
        if(strcmp(passportFields[i], key) == 0) {
            passport[i] = value;
        }
    }
}

int checkPassportFilled(char** passport) {
    int filledKeyCounter = 0;
    // check if each field of passport is filled (except for Country ID)
    for(int i = 0; i < NUM_PASSPORT_FIELDS; i++) {
        if(strcmp(passport[i], "") != 0 || i == COUNTRY_ID_INDEX) {
            filledKeyCounter++;
        }
    }
    if(filledKeyCounter == NUM_PASSPORT_FIELDS) {
        return 1;
    }
    return 0;
}

int checkHeightValid(char* inputString) {
    if(strlen(inputString) <= HEIGHT_UNIT_STRING_LENGHT) {
        return 0;
    }
    // get unit string
    char unit[(HEIGHT_UNIT_STRING_LENGHT + 1)];
    strcpy(unit, (inputString + (strlen(inputString) - HEIGHT_UNIT_STRING_LENGHT)));
    
    // check height value based on unit string
    char value[HEIGHT_VALUE_BUFFER] = "";
    if(strcmp(unit, HEIGHT_UNIT_CM) == 0) {
        strncpy(value, inputString, strlen(inputString) - HEIGHT_UNIT_STRING_LENGHT);
        return (atoi(value) >= HEIGHT_CM_MIN && atoi(value) <= HEIGHT_CM_MAX ? 1 : 0);
    } else if(strcmp(unit, HEIGHT_UNIT_IN) == 0) {
        strncpy(value, inputString, strlen(inputString) - HEIGHT_UNIT_STRING_LENGHT);
        return (atoi(value) >= HEIGHT_IN_MIN && atoi(value) <= HEIGHT_IN_MAX ? 1 : 0);
    } else {
        return 0;
    }
}

int checkHairColor(char* inputString) {
    if(strlen(inputString) != HAIR_COLOR_STRING_LENGTH || inputString[0] != HAIR_COLOR_FIRST_CHAR) {
        return 0;
    }

    for(int i = 1; i < strlen(inputString); i++) {
        if(strchr(HAIR_COLOR_VALID_CHARS, inputString[i]) == NULL) {
            return 0;
        }
    }
    return 1;

}

int checkEyeColor(char* inputString) {
    for(int i = 0; i < NUM_VALID_EYE_COLORS; i++) {
        if(strcmp(inputString, validEyeColors[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int checkPassportIdValid(char* inputString) {
    if(strlen(inputString) != PASSPORT_ID_LENGTH) {
        return 0;
    }
    for(int i = 0; i < PASSPORT_ID_LENGTH; i++) {
        if(strchr(PASSPORT_ID_VALID_CHARS, inputString[i]) == NULL) {
            return 0;
        }
    }
    return 1;
}

int checkPassportValid(char** passport) {
    int byr = atoi(passport[BIRTH_YEAR_INDEX]);
    int iyr = atoi(passport[ISSUE_YEAR_INDEX]);
    int eyr = atoi(passport[EXPIRATION_YEAR_INDEX]);

    int byrValid = (byr >= BIRTH_YEAR_MIN && byr <= BIRTH_YEAR_MAX ? 1 : 0);
    int iyrValid = (iyr >= ISSUE_YEAR_MIN && iyr <= ISSUE_YEAR_MAX ? 1 : 0);
    int eyrValid = (eyr >= EXPIRATION_YEAR_MIN && eyr <= EXPIRATION_YEAR_MAX ? 1 : 0);
    int hgtValid = checkHeightValid(passport[HEIGHT_INDEX]);
    int hclValid = checkHairColor(passport[HAIR_COLOR_INDEX]);
    int eclValid = checkEyeColor(passport[EYE_COLOR_INDEX]);
    int pidValid = checkPassportIdValid(passport[PASSPORT_ID_INDEX]);

    return (byrValid && iyrValid && eyrValid && hgtValid && hclValid && eclValid && pidValid ? 1 : 0);
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

    char* currentPassport[NUM_PASSPORT_FIELDS];
    for(int i = 0; i < NUM_PASSPORT_FIELDS; i++) {
        currentPassport[i] = "";
    }
    int filledPassportCounter = 0;
    int validPassportCounter = 0;

    for(int i = 0; i < file.lineCount; i++) {
        char* line = file.lines[i];

        // if an empty line is found the current password is completed and can be checked
        if(strcmp(line, "\n") == 0) {
            if(checkPassportFilled(currentPassport)) {
                filledPassportCounter++;
                validPassportCounter += checkPassportValid(currentPassport);
            }
            // clear currentPassport
            for(int j = 0; j < NUM_PASSPORT_FIELDS; j++) {
                currentPassport[j] = "";
            }
        }

        // split each line into key-value pairs
        char* keyValue = strtok(line, KEY_VALUE_PAIR_DELIMITERS);
        while (keyValue != NULL)
        {
            // parse each key-value pair and sort it into current passport
            putKeyValueIntoPassport(currentPassport, keyValue);
            keyValue = strtok(NULL, KEY_VALUE_PAIR_DELIMITERS);
        }
    }
    // check for last passport not separated by empty line
    if(checkPassportFilled(currentPassport)) {
        filledPassportCounter++;
        validPassportCounter += checkPassportValid(currentPassport);
    }

    printf("Puzzle 1 Answer: %i\n", filledPassportCounter);
    printf("Puzzle 2 Answer: %i\n", validPassportCounter);

    closeFile(&file);
}