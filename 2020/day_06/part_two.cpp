#include "..\common.h"

int countGroupAnswers(char** position){
    const int checklistSize = 'z' - 'a' + 1;
    int answerChecklist [checklistSize] = {0};

    int personCount = 0;
    while(true){
        char c = *((*position)++);
        printf("%c", c);
        if(c != '\n'){
            answerChecklist['z' - c]++;
        }else{
            personCount++;
            if(**position == '\n' || **position == '\0'){
                break;
            }
        }
        
    }

    (*position)++;

    int answerCount = 0;
    for(char i = 0; i < checklistSize; i++){
        if(answerChecklist[i] == personCount){
            answerCount++;
        }
    }

    printf("Answer count: %i\n\n", answerCount);
    return answerCount;
}

int main(){
    String text = readFile("input.txt");

    char* position = text.data;

    int totalSum = 0;
    while(true){
        totalSum += countGroupAnswers(&position);
        if(*position == '\0'){
            break;
        }
    }

    printf("Total sum: %i\n", totalSum);

    return 0;
}