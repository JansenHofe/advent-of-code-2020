#include "readFile.h"
#include <stdio.h>

#define ROW_SIZE 98
// +2 for newline and null-terminator
#define INPUT_LINE_BUFFER_SIZE (ROW_SIZE + 2)
#define CHAR_SEAT_OCCUPIED '#'
#define CHAR_SEAT_EMPTY 'L'
#define CHAR_FLOOR '.'

typedef int (*getOccupiedSeatsFunction)(char** seatLayout, int width, int height, int posY, int posX);

int getAdjacentOccupiedSeats(char** seatLayout, int width, int height, int posY, int posX) {
    int occSeats = 0;
    // check each adjacent seat (exclude the seat itself and seats out of bounds)
    for(int y = -1; y <= 1; y++) {
        for(int x = -1; x <= 1; x++) {
            int checkX = posX + x;
            int checkY = posY + y;

            if(checkX >= 0 && checkX < width 
            && checkY >= 0 && checkY < height 
            && !(x == 0 && y == 0)
            && seatLayout[checkY][checkX] == CHAR_SEAT_OCCUPIED) {
                occSeats++;
            }
        }
    }
    return occSeats;
}

int getOccupiedSeatsInSight(char** seatLayout, int width, int height, int posY, int posX){
    int occSeats = 0;
    //iterate each direction
    for(int y = -1; y <= 1; y++) {
        for(int x = -1; x <= 1; x++) {
            if(x == 0 && y == 0) continue;

            // go forward each direction until position is out of bounds, an empty seat or an occupied seat
            int range = 1;
            while(1) {
                int checkX = posX + (x * range);
                int checkY = posY + (y * range);

                if(checkX < 0 || checkX >= width || checkY < 0 || checkY >= height || seatLayout[checkY][checkX] == CHAR_SEAT_EMPTY) {
                break;
                } else if(seatLayout[checkY][checkX] == CHAR_SEAT_OCCUPIED) {
                occSeats++;
                break;
                }
                range++;
            }
        }
    }
    return occSeats;
}

int applyRules(char** seatLayout, int width, int height, getOccupiedSeatsFunction occSeatsFn, int occSeatTolerance) {
    char newSeatLayout[height][width];
    int layoutChanges = 0;

    // apply rules on each position in seat layout and save next state of the position in 'newSeatLayout'
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            int occAdjacentSeats = occSeatsFn(seatLayout, width, height, y, x);
            if(seatLayout[y][x] == CHAR_SEAT_EMPTY && occAdjacentSeats == 0) {
                newSeatLayout[y][x] = CHAR_SEAT_OCCUPIED;
                layoutChanges++;
            } else if(seatLayout[y][x] == CHAR_SEAT_OCCUPIED && occAdjacentSeats >= occSeatTolerance) {
                newSeatLayout[y][x] = CHAR_SEAT_EMPTY;
                layoutChanges++;
            } else {
                newSeatLayout[y][x] = seatLayout[y][x];
            }
        }
    }
    // set seatLayout to newSeatLayout
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            seatLayout[y][x] = newSeatLayout[y][x];
        }
    }
    // return the number of changed positions in layout
    return layoutChanges++;
}

int getOccupiedSeatsOnStableLayout(AocFile *file, getOccupiedSeatsFunction getOccSeatsFn, int occSeatTolerance) {
    int layoutChanges = 1;
    // apply rules until the layout does not change anymore
    while (layoutChanges) {
        layoutChanges = applyRules(file->lines, ROW_SIZE, file->lineCount, getOccSeatsFn, occSeatTolerance);
    }

    // count the occupied seats
    int occupiedSeats = 0;
    for(int y = 0; y < file->lineCount; y++) {
        for(int x = 0; x < ROW_SIZE; x++) {
            if(file->lines[y][x] == CHAR_SEAT_OCCUPIED) {
                occupiedSeats++;
            }
        }
    }
    return occupiedSeats;
}

int main(int argc, char *argv[]) {

    if(argc != 2) {
        fprintf(stderr, "Error: exactly one argument needed, which is the path to the input file \n");
        return -1;
    }

    AocFile file;
    AocFile fileClone;
    if(readFile(argv[1], INPUT_LINE_BUFFER_SIZE, &file) != 0 || cloneFile(&file, &fileClone) != 0) {
        perror("Error reading file: ");
        return -1;
    }

    printf("Puzzle 1 Answer: %i\n", getOccupiedSeatsOnStableLayout(&file, getAdjacentOccupiedSeats, 4));
    printf("Puzzle 2 Answer: %i\n", getOccupiedSeatsOnStableLayout(&fileClone, getOccupiedSeatsInSight, 5));

    closeFile(&file);
    closeFile(&fileClone);
}