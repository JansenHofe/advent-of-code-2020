#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readFile.h"

int readFile(char* path, int bufferLength, AocFile* fileOut) {
    FILE* filePointer;
    int ch;
    fileOut->lineCount = 0;
    fileOut->lineBufferSize = bufferLength;

    filePointer = fopen(path, "r");
    if(filePointer == NULL) {
        return -1;
    }

    // count file lines
    do {                                                                                                  
        ch = fgetc(filePointer);                                                                            
        if (ch == '\n')                                                                                
            fileOut->lineCount++;                                                                                   
    } while (ch != EOF);                                                                               

    rewind(filePointer); 
    
    // allocate memory for line list
    fileOut->lines = malloc(sizeof(char*) * fileOut->lineCount);
    if(fileOut->lines == NULL) {
        fclose(filePointer);
        return -1;
    }

    for(int i = 0; i < fileOut->lineCount; i++) {
        // allocate memory for single line
        fileOut->lines[i] = malloc(sizeof(char) * bufferLength);
        if(fileOut->lines[i] == NULL) {
            // if malloc fails, free all previously allocated memory
            for(int j = (i - 1); j >= 0; j--) {
                free(fileOut->lines[j]);
            }
            free(fileOut->lines);
            fclose(filePointer);
            return -1;
        }
        // read single line
        fgets(fileOut->lines[i], bufferLength, filePointer);
    }
    fclose(filePointer);
    return 0;
}

int executeOnEachLine(char* path, int bufferLength, readLineCallback onReadLine) {
    FILE* filePointer;
    char buffer[bufferLength]; 
    filePointer = fopen(path, "r");

    if(filePointer == NULL) {
        return -1;
    }

    while(fgets(buffer, bufferLength, filePointer)) {
        onReadLine(buffer);
    }

    fclose(filePointer);
    return 0;
}

int cloneFile(AocFile* inputFile, AocFile* outputFile) {
    outputFile->lines = malloc(sizeof(char*) * inputFile->lineCount);
        if(outputFile->lines == NULL) {
        return -1;
    }

    for(int i = 0; i < inputFile->lineCount; i++) {
        // allocate memory for single line
        outputFile->lines[i] = malloc(sizeof(char) * inputFile->lineBufferSize);
        if(outputFile->lines[i] == NULL) {
            // if malloc fails, free all previously allocated memory
            for(int j = (i - 1); j >= 0; j--) {
                free(outputFile->lines[j]);
            }
            free(outputFile->lines);
            return -1;
        }
        // read single line
        strcpy(outputFile->lines[i], inputFile->lines[i]);
    }
    outputFile->lineCount = inputFile->lineCount;
    outputFile->lineBufferSize = inputFile->lineBufferSize;
    return 0;
}

void closeFile(AocFile* file) {
    for(int i = 0; i < file->lineCount; i++) {
        free(file->lines[i]);
    }
    free(file->lines);
}