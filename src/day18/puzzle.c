#include "readFile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_LINE_BUFFER_SIZE 200


// reduced and modified shunting-yard algorithm (https://en.wikipedia.org/wiki/Shunting-yard_algorithm)
// in this version only single digit numbers, parenthesis and the operators + and * are allowed
// furthermore + and * have the same precedence if the argument 'part2' is set to false
// if it is set to true, + has the higher precedence
char* shuntingYard(char* line, int part2) {
    char* current = line;
    // output in reverse polish notation
    char* output = malloc(sizeof(char) * strlen(line) + sizeof(char));
    char operatorStack[strlen(line) + 1];
    int outputIdx = 0;
    int opStackTopIdx = -1;

    while(*current != '\n') {
        switch(*current) {
            case '(':
                operatorStack[++opStackTopIdx] = *current;
            break;
            case ')': {
                char opStackTop = operatorStack[opStackTopIdx];
                while(opStackTop != '(') {
                    output[outputIdx++] = opStackTop;
                    opStackTop = operatorStack[--opStackTopIdx];
                }
                opStackTopIdx--;
            }   
            break;
            case '+': {
                char opStackTop = operatorStack[opStackTopIdx];
                // special case: if part2 is true, + has the higher precedence
                while((opStackTopIdx != -1) && (opStackTop != '(') && (opStackTop != '*' || !part2)) {
                    output[outputIdx++] = opStackTop;
                    opStackTop = operatorStack[--opStackTopIdx];
                }
                operatorStack[++opStackTopIdx] = '+';
            }
            break;
            case '*': {
                char opStackTop = operatorStack[opStackTopIdx];
                while((opStackTopIdx != -1) && (opStackTop != '(')) {
                    output[outputIdx++] = opStackTop;
                    opStackTop = operatorStack[--opStackTopIdx];
                }
                operatorStack[++opStackTopIdx] = '*';
            }
            break;
            case ' ':
            break;
            default:
                output[outputIdx++] = *current;
            break;

        }
        current++;
    }
    char opStackTop = operatorStack[opStackTopIdx];
    while((opStackTopIdx != -1)) {
        output[outputIdx++] = opStackTop;
        opStackTop = operatorStack[--opStackTopIdx];
    }
    output[outputIdx] = '\0';
    return output;
}

long performOperation(long a, long b, char op) {
    if(op == '+') return (a + b);
    else return (a * b);
}

// execute an expression in reverse polish notation
long executeRpnExpression(char* expression) {

    int expressionLength = strlen(expression);
    long stack[expressionLength];
    int stackTopIdx = -1;

    for(int i = 0; i < expressionLength; i++) {
        char token = expression[i];
        if(token == '+' || token == '*') {
            long number1 = stack[stackTopIdx--];
            long number2 = stack[stackTopIdx--];
            stack[++stackTopIdx] = performOperation(number1, number2, token);
        } else {
            stack[++stackTopIdx] = (token - '0');
        }
    }
    return stack[stackTopIdx];
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

    long sumPart1 = 0;
    long sumPart2 = 0;
    for(int i = 0; i < file.lineCount; i++) {

        char* rpnExpressionPart1 = shuntingYard(file.lines[i], 0);
        char* rpnExpressionPart2 = shuntingYard(file.lines[i], 1);

        sumPart1 += executeRpnExpression(rpnExpressionPart1);
        sumPart2 += executeRpnExpression(rpnExpressionPart2);

        free(rpnExpressionPart1);
        free(rpnExpressionPart2);
    }

    printf("Puzzle 1 Answer: %li\n", sumPart1);
    printf("Puzzle 2 Answer: %li\n", sumPart2);
    closeFile(&file);
}