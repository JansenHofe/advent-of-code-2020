#include "readFile.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INPUT_LINE_BUFFER_SIZE 6
#define NUM_COMPASS_DIRECTIONS 4

typedef struct {
    int x;
    int y;
} Position, Direction;

Direction compassDirections[NUM_COMPASS_DIRECTIONS] = {{.x=1, .y=0}, {.x=0, .y=1}, {.x=-1, .y=0}, {.x=0, .y=-1}}; 

// modulo function working with negative numbers, so -1 mod 4 = 3
int mod(int a,int b){
    return (a % b + b) %b;
}

int getDistanceFollowingDirectInstructions(AocFile* instructions, Position startPos, int startDir) {
    for(int i = 0; i < instructions->lineCount; i++) {
        int value = atoi((instructions->lines[i] + 1));

        switch (instructions->lines[i][0]) {
        case 'N': startPos.y -= value; break;
        case 'E': startPos.x += value; break;
        case 'S': startPos.y += value; break;
        case 'W': startPos.x -= value; break;
        case 'L': startDir =  mod(startDir - (value / 90),NUM_COMPASS_DIRECTIONS); break;
        case 'R': startDir =  mod(startDir + (value / 90),NUM_COMPASS_DIRECTIONS); break;
        case 'F': 
            startPos.x += (compassDirections[startDir].x * value);
            startPos.y += (compassDirections[startDir].y * value);
            break;
        }
    }
    return (abs(startPos.x) + abs(startPos.y));
}

int getDistanceFollowingWaypointInstructions(AocFile* instructions, Position startPos, int startDir, Position waypointStartPos) {
    for(int i = 0; i < instructions->lineCount; i++) {
        int value = atoi((instructions->lines[i] + 1));

        switch (instructions->lines[i][0]) {
        case 'N': waypointStartPos.y -= value; break;
        case 'E': waypointStartPos.x += value; break;
        case 'S': waypointStartPos.y += value; break;
        case 'W': waypointStartPos.x -= value; break;
        case 'L': value *= 3; 
            // intentionally no break
        case 'R': 
            // rotate waypoint
            for(int i = 0; i < (value / 90); i++) {
                int temp = waypointStartPos.y;
                waypointStartPos.y = waypointStartPos.x;
                waypointStartPos.x = (temp * -1);
            }
            break;
        case 'F': 
            startPos.x += (waypointStartPos.x * value);
            startPos.y += (waypointStartPos.y * value);
            break;
        }
    }
    return (abs(startPos.x) + abs(startPos.y));
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

    Position startPos = {.x= 0, .y=0};
    int startDirectionIdx = 0;
    Position wayPointStartPos = {.x=10, .y=-1};

    int distanceDirectInstructions = getDistanceFollowingDirectInstructions(&file, startPos, startDirectionIdx);
    int distanceWaypointInstructions = getDistanceFollowingWaypointInstructions(&file, startPos, startDirectionIdx, wayPointStartPos);
    printf("Puzzle 1 Answer: %i\n", distanceDirectInstructions);
    printf("Puzzle 2 Answer: %i\n", distanceWaypointInstructions);

    closeFile(&file);
}
