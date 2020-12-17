#include "readFile.h"
#include <stdio.h>
#include <stdlib.h>

#define INPUT_LINE_BUFFER_SIZE 10

#define INPUT_SIZE_X 8
#define INPUT_SIZE_Y 8
#define INPUT_SIZE_Z 1
#define INPUT_SIZE_W 1

#define CHAR_INACTIVE '.'
#define CHAR_ACTIVE '#'

typedef struct {
    char**** data;
    int sizeW;
    int sizeZ;
    int sizeY;
    int sizeX;

} Grid4d;

Grid4d* initGrid(int gridSizeW, int gridSizeZ, int gridSizeY, int gridSizeX ) {
    Grid4d* g = malloc(sizeof(Grid4d));
    g->sizeX = gridSizeX;
    g->sizeY = gridSizeY;
    g->sizeZ = gridSizeZ;
    g->sizeW = gridSizeW;
    g->data = malloc(sizeof(char***) * gridSizeW);

    for(int w = 0; w < gridSizeW; w++) {
        g->data[w] = malloc(sizeof(char**) * gridSizeZ);
        for(int z = 0; z < gridSizeZ; z++) {
            g->data[w][z] = malloc(sizeof(char*) * gridSizeY);
            for(int y = 0; y < gridSizeY; y++) {
                g->data[w][z][y] = malloc(sizeof(char) * gridSizeX);
                for(int x = 0; x < gridSizeX; x++) {
                    g->data[w][z][y][x] = CHAR_INACTIVE;
                }
            }
        }
    }
    return g;
}

void freeGrid(Grid4d* grid) {
    for(int w = 0; w < grid->sizeW; w++) {
        for(int z = 0; z < grid->sizeZ; z++) {
            for(int y = 0; y < grid->sizeY; y++) {
                free(grid->data[w][z][y]);
            }
            free(grid->data[w][z]);
        }
        free(grid->data[w]);
    }
    free(grid->data);
    free(grid);
}

int getActiveNeighbors(Grid4d* grid, int targetW, int targetZ, int targetY, int targetX) {

    int neighborCounter = 0;
    for(int w = -1; w <= 1; w++) {
        for(int z = -1; z <= 1; z++) {
            for(int y = -1; y <= 1; y++) {
                for(int x = -1; x <= 1; x++) {
                    if(z == 0 && y == 0 && x == 0 && w == 0) {
                        continue;
                    }
                    int checkX = targetX + x;
                    int checkY = targetY + y;
                    int checkZ = targetZ + z;
                    int checkW = targetW + w;

                    if(checkZ >= 0 && checkZ < grid->sizeZ 
                    && checkY >= 0 && checkY < grid->sizeY 
                    && checkX >= 0 && checkX < grid->sizeX 
                    && checkW >= 0 && checkW < grid->sizeW
                    && grid->data[checkW][checkZ][checkY][checkX] == CHAR_ACTIVE) {
                        neighborCounter++;
                    }
                }
            }
        }
    }
    return neighborCounter;
}

int countActiveCubes(Grid4d* grid) {
    int counter = 0;
    for(int w = 0; w < grid->sizeW; w++) {
      for(int z = 0; z < grid->sizeZ; z++) {
            for(int y = 0; y < grid->sizeY; y++) {
                for(int x = 0; x < grid->sizeX; x++) {
                    if(grid->data[w][z][y][x] == CHAR_ACTIVE) {
                        counter++;
                    }
                }
            }
        }
    }
  
    return counter;
}

void applyRules(Grid4d** gridPointer) {
    Grid4d* grid = *gridPointer;
    Grid4d* newGrid = initGrid(grid->sizeW, grid->sizeZ, grid->sizeY, grid->sizeX);

    for(int w = 0; w < grid->sizeW; w++) {
        for(int z = 0; z < grid->sizeZ; z++) {
            for(int y = 0; y < grid->sizeY; y++) {
                for(int x = 0; x < grid->sizeX; x++) {
                    int activeNeighbors = getActiveNeighbors(grid, w, z, y, x);
                    if(grid->data[w][z][y][x] == CHAR_ACTIVE && (activeNeighbors == 2 || activeNeighbors == 3)) {
                        newGrid->data[w][z][y][x] = CHAR_ACTIVE;
                    } else if(grid->data[w][z][y][x] == CHAR_INACTIVE && activeNeighbors == 3) {
                        newGrid->data[w][z][y][x] = CHAR_ACTIVE;
                    }
                }
            }
        }
    }
    freeGrid(grid);
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
    // each cycle, the grid can only grow two cubes (one for each direction) in each dimension
    int gridSizeX = INPUT_SIZE_X + (2 * cycles);
    int gridSizeY = INPUT_SIZE_Y + (2 * cycles);
    int gridSizeZ = INPUT_SIZE_Z + (2 * cycles);
    int gridSizeW = INPUT_SIZE_W + (2 * cycles);

    // the 3d grid has gridSizeW = 1
    Grid4d* grid3d = initGrid(1, gridSizeZ, gridSizeY, gridSizeX);
    Grid4d* grid4d = initGrid(gridSizeW, gridSizeZ, gridSizeY, gridSizeX);

    // put the input data in the middle of the grids
    for (int y = 0; y < INPUT_SIZE_Y; y++) {
        for( int x = 0; x < INPUT_SIZE_X; x++ ) {
            grid3d->data[0][cycles][cycles + y][cycles + x] = file.lines[y][x];
            grid4d->data[cycles][cycles][cycles + y][cycles + x] = file.lines[y][x];
        }
    }

    // apply the rules each cycle
    for(int i = 0; i < cycles; i++) {
        applyRules(&grid3d);
        applyRules(&grid4d);
    }
    
    // count the active cubes afterwards
    int activeCubes3d = countActiveCubes(grid3d);
    int activeCubes4d = countActiveCubes(grid4d);

    printf("Puzzle 1 Answer: %i\n", activeCubes3d);
    printf("Puzzle 2 Answer: %i\n", activeCubes4d);

    freeGrid(grid3d);
    freeGrid(grid4d);
    closeFile(&file);
}
