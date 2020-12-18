#include "readFile.h"
#include <stdio.h>
#include <stdlib.h>

#define INPUT_LINE_BUFFER_SIZE 200

typedef struct {
    char* lastChar;
    long value;

} ExpressionReturn;

long execOperation(long operand1, long operand2, char operator) {
    if(operator == '*') {
        return (operand1 * operand2);
    } else if(operator == '+') {
        return (operand1 + operand2);
    }
    fprintf(stderr, "Error: unknown operator '%c'\n", operator);
    return -1;
}

ExpressionReturn evalExpression(char* line) {
    char* current = line;
    long expressionValue = 0;
    char currentOperator = '+';

    while(*current != '\0') {
        switch(*current) {
            case '(': {
                    ExpressionReturn r = evalExpression(++current);
                    current = r.lastChar;
                    expressionValue = execOperation(expressionValue, r.value, currentOperator);
                }   
            break;
            case ')':
            case '\n': {
                    ExpressionReturn r = {.lastChar = current, .value = expressionValue};
                    return r;
                }
            break;
            case '+': currentOperator = '+'; break;
            case '*': currentOperator = '*'; break;
            case ' ': break;
            default: 
                expressionValue = execOperation(expressionValue, atoi(current), currentOperator);
                break;

        }

        current++;
    }
    ExpressionReturn ret = {.lastChar = current, .value = expressionValue};
    return ret;
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



    long sum = 0;
    for(int i = 0; i < file.lineCount; i++) {
        sum += evalExpression(file.lines[i]).value; 
    }

    printf("Puzzle 1 Answer: %li\n", sum);
    closeFile(&file);
}