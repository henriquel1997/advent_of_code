#include <stdio.h>
#include <stdlib.h>

struct String {
    long size;
    char* data;
};

String readFile(const char* fileName){
    FILE *f = fopen(fileName, "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* string = (char*) malloc(fsize + 1);
    fread(string, 1, fsize, f);
    fclose(f);

    string[fsize] = 0;
    return (String){fsize, string};
}

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