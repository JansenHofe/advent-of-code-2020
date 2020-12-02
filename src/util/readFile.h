#ifndef READ_FILE_H
#define READ_FILE_H

typedef struct {
    char** lines;
    int lineCount;
} AocFile;

int readFile(char* path, int bufferLength, AocFile* fileOut);
void closeFile(AocFile* file);

#endif