#include "readFile.h"
#include <stdio.h>
#include <string.h>

#define INPUT_LINE_BUFFER_SIZE 30

#define NUM_QUESTIONS 26
#define ASCII_CODE_A 97

// count how many questions were answered 'yes' by at least minNumPeople 
int countQuestionsAnsweredYes(int* declarationForm, int minNumPeople) {
    int questionsAnsweredYes = 0;
    for(int i = 0; i < NUM_QUESTIONS; i++) {
        if(declarationForm[i] >= minNumPeople) {
            questionsAnsweredYes++;
        }
    }
    return questionsAnsweredYes;
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

    // create empty group declaration form
    // the declaration form represents how many people of the group answered "yes" for each question (0-25 -> a-z)
    int currentDeclarationForm[NUM_QUESTIONS];
    memset(currentDeclarationForm, 0, sizeof currentDeclarationForm); 

    int currentNumPeople = 0;
    int questionsAnyoneAnsweredYes = 0;
    int questionsEveryoneAnsweredYes = 0;

    for(int i = 0; i < file.lineCount; i++) {
        char* line = file.lines[i];

        // if an empty line is encountered, the current group form is completed and the question answers can be evaluated
        if(strcmp(line, "\n") == 0) {
            questionsAnyoneAnsweredYes += countQuestionsAnsweredYes(currentDeclarationForm, 1);
            questionsEveryoneAnsweredYes += countQuestionsAnsweredYes(currentDeclarationForm, currentNumPeople);

            // reset group values after evaluation
            memset(currentDeclarationForm, 0, sizeof currentDeclarationForm);
            currentNumPeople = 0;
            continue;
        }
        currentNumPeople++;

        while(*line != '\n') {
            // convert question index (a-z) to numeric values (0-25)
            int currentQuestionIdx = (*line - ASCII_CODE_A );
            currentDeclarationForm[currentQuestionIdx]++;
            line++;
        }
    }
    // evaluate last group which was not separated by an empty line
    questionsAnyoneAnsweredYes += countQuestionsAnsweredYes(currentDeclarationForm, 1);
    questionsEveryoneAnsweredYes += countQuestionsAnsweredYes(currentDeclarationForm, currentNumPeople);

    printf("Puzzle 1 Answer: %i\n",questionsAnyoneAnsweredYes);
    printf("Puzzle 2 Answer: %i\n",questionsEveryoneAnsweredYes);
    closeFile(&file);
}