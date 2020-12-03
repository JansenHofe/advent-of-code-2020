#include "readFile.h"
#include <stdio.h>

#define TREE_PATTERN_LENGTH 31
// has to be two char longer, as newline and nullterminator for string is added
#define INPUT_LINE_BUFFER_SIZE (TREE_PATTERN_LENGTH + 2)

#define POSITION_START_X 0
#define POSITION_START_Y 0

#define TREE_CHARACTER '#'

typedef struct {
    int x;
    int y;
} Position, MovementPattern;

int getTreesInSlope(AocFile* input, int treePatternLength, Position start, MovementPattern move) {
    int treeCounter = 0;

    while(start.y < input->lineCount) {
        if(input->lines[start.y][start.x] == TREE_CHARACTER) {
            treeCounter++;
        }

        start.x = (start.x + move.x) % treePatternLength;
        start.y += move.y;
    }
    return treeCounter;
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

    // taken from puzzle description
    MovementPattern patterns[] = {
        {.x = 1, .y = 1},
        {.x = 3, .y = 1},   // input for puzzle 1
        {.x = 5, .y = 1},
        {.x = 7, .y = 1},
        {.x = 1, .y = 2}
    };

    Position start = {.x = POSITION_START_X, .y = POSITION_START_Y};
    long treeProduct = 1;
    for(int i = 0; i < 5; i++) {
       int trees = getTreesInSlope(&file, TREE_PATTERN_LENGTH, start, patterns[i]);

       treeProduct *= trees;

        if(i == 1) {
           printf("Puzzle 1 Answer: %i\n", trees);
        }
    }
    printf("Puzzle 2 Answer: %li\n", treeProduct);

    closeFile(&file);
}