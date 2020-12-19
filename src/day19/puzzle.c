#include "readFile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_LINE_BUFFER_SIZE 100
#define NUM_RULES 131
#define DATA_START_LINE (NUM_RULES + 1)

typedef struct {
    int id;
    char* rule;
} RuleString;

int compare(const void *p1, const void *p2)
{
    const RuleString *elem1 = p1;    
    const RuleString *elem2 = p2;

   if (elem1->id < elem2->id)
      return -1;
   else if (elem1->id > elem2->id)
      return 1;
   else
      return 0;
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

    RuleString rules[NUM_RULES];
    for(int i = 0; i < NUM_RULES; i++) {
        rules[i].id = strtol(file.lines[i], &rules[i].rule, 10);
        rules[i].rule++;
    }
    qsort(rules, NUM_RULES, sizeof(RuleString), compare);

    
    return 1;
    closeFile(&file);
}