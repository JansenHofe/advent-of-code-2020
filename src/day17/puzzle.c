#include "readFile.h"
#include <stdio.h>
#include <stdlib.h>

#define INPUT_LINE_BUFFER_SIZE 10

#define INPUT_SIZE_X 8
#define INPUT_SIZE_Y 8
#define INPUT_SIZE_Z 1

#define CHAR_INACTIVE '.'
#define CHAR_ACTIVE '#'

typedef struct {
    char*** data;
    int sizeZ;
    int sizeY;
    int sizeX;

} Grid3d;

Grid3d* initGrid(int gridSizeZ, int gridSizeY, int gridSizeX) {
    char*** grid = malloc(sizeof(char**) * gridSizeZ);
    for(int z = 0; z < gridSizeZ; z++) {
        grid[z] = malloc(sizeof(char*) * gridSizeY);
        for(int y = 0; y < gridSizeY; y++) {
            grid[z][y] = malloc(sizeof(char) * gridSizeX);

            for(int x = 0; x < gridSizeX; x++) {
                grid[z][y][x] = CHAR_INACTIVE;
            }
        }
    }
    Grid3d* g = malloc(sizeof(Grid3d));
    g->data = grid;
    g->sizeX = gridSizeX;
    g->sizeY = gridSizeY;
    g->sizeZ = gridSizeZ;
    return g;
}

void printGrid(Grid3d* grid) {
  for(int z = 0; z < grid->sizeZ; z++) {
      printf("z = %i\n", z);
        for(int y = 0; y < grid->sizeY; y++) {
            for(int x = 0; x < grid->sizeX; x++) {
                printf("%c", grid->data[z][y][x]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

int getActiveNeighbors(Grid3d* grid, int targetZ, int targetY, int targetX) {

    int neighborCounter = 0;
    for(int z = -1; z <= 1; z++) {
        for(int y = -1; y <= 1; y++) {
            for(int x = -1; x <= 1; x++) {
                if(z == 0 && y == 0 && x == 0) {
                    continue;
                }
                int checkX = targetX + x;
                int checkY = targetY + y;
                int checkZ = targetZ + z;

                if(checkZ >= 0 && checkZ < grid->sizeZ 
                && checkY >= 0 && checkY < grid->sizeY 
                && checkX >= 0 && checkX < grid->sizeX 
                && grid->data[checkZ][checkY][checkX] == CHAR_ACTIVE) {
                    neighborCounter++;
                }
            }
        }
    }
    return neighborCounter;
}

int countActiveCubes(Grid3d* grid) {
    int counter = 0;
    for(int z = 0; z < grid->sizeZ; z++) {
        for(int y = 0; y < grid->sizeY; y++) {
            for(int x = 0; x < grid->sizeX; x++) {
                if(grid->data[z][y][x] == CHAR_ACTIVE) {
                    counter++;
                }
            }
        }
    }
    return counter;
}

void applyRules(Grid3d** gridPointer) {
    Grid3d* grid = *gridPointer;
    Grid3d* newGrid = initGrid(grid->sizeZ, grid->sizeY, grid->sizeX);

    for(int z = 0; z < grid->sizeZ; z++) {
        for(int y = 0; y < grid->sizeY; y++) {
            for(int x = 0; x < grid->sizeX; x++) {
                int activeNeighbors = getActiveNeighbors(grid, z, y, x);
                if(grid->data[z][y][x] == CHAR_ACTIVE && (activeNeighbors == 2 || activeNeighbors == 3)) {
                    newGrid->data[z][y][x] = CHAR_ACTIVE;
                } else if(grid->data[z][y][x] == CHAR_INACTIVE && activeNeighbors == 3) {
                    newGrid->data[z][y][x] = CHAR_ACTIVE;
                }
            }
        }
    }
    *gridPointer = newGrid;
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
    int cycles = 6;
    int gridSizeX = INPUT_SIZE_X + (2 * cycles);
    int gridSizeY = INPUT_SIZE_Y + (2 * cycles);
    int gridSizeZ = INPUT_SIZE_Z + (2 * cycles);
    Grid3d* grid = initGrid(gridSizeZ, gridSizeY, gridSizeX);

    for (int y = 0; y < INPUT_SIZE_Y; y++) {
        for( int x = 0; x < INPUT_SIZE_X; x++ ) {
            grid->data[cycles][cycles + y][cycles + x] = file.lines[y][x];
        }
    }

    for(int i = 0; i < cycles; i++) {
        applyRules(&grid);
    }
   
    int activeCubes = countActiveCubes(grid);

    printf("Puzzle 1 Answer: %i\n", activeCubes);

    closeFile(&file);
}
