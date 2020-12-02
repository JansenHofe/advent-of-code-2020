#ifndef READ_FILE_H
#define READ_FILE_H

typedef struct {
    char** lines;
    int lineCount;
} AocFile;

typedef void (*readLineCallback)(char* currentLine);

int readFile(char* path, int bufferLength, AocFile* fileOut);
int executeOnEachLine(char* path, int bufferLength, readLineCallback onReadLine);
void closeFile(AocFile* file);

#endif