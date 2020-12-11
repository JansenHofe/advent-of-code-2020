#ifndef READ_FILE_H
#define READ_FILE_H

typedef struct {
    char** lines;
    int lineCount;
    int lineBufferSize;
} AocFile;

typedef void (*readLineCallback)(char* currentLine);

int readFile(char* path, int bufferLength, AocFile* fileOut);
int cloneFile(AocFile* inputFile, AocFile* outputFile);
int executeOnEachLine(char* path, int bufferLength, readLineCallback onReadLine);
void closeFile(AocFile* file);

#endif