#include "..\common.h"

int countGroupAnswers(char** position){
    const int checklistSize = 'z' - 'a' + 1;
    int answerChecklist [checklistSize] = {0};

    while(true){
        char c = *((*position)++);
        printf("%c", c);
        if(c != '\n'){
            answerChecklist['z' - c] = 1;
        }else if(**position == '\n' || **position == '\0'){
            break;
        }
        
    }

    (*position)++;

    int sum = 0;
    for(char i = 0; i < checklistSize; i++){
        sum += answerChecklist[i];
    }

    printf("Sum: %i\n\n", sum);
    return sum;
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