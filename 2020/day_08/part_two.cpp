#include "..\common.h"

bool isSign(char c){
    return c == '+' || c == '-';
}

struct ProgramResult{
    bool finished;
    int acc;
};

ProgramResult runProgram(String text){
    char* position = text.data;
    
    int numVisitedLines = 0;
    int visitedLines[10000] = {0};

    int acc = 0;
    int currentLine = 1;
    bool finished = false;
    while((position - text.data) < text.size){
        for(int i = 0; i < numVisitedLines; i++){
            if(visitedLines[i] == currentLine){
                printf("Infinite loop at line %i, stopping execution.\n", currentLine);
                goto end;
            }
        }
        visitedLines[numVisitedLines++] = currentLine;

        printf("Line %i: ", currentLine);

        if(strings_equal(position, "acc")){
            while(!isSign(*position)) position++;
            char sign = *position;
            int amount = strtol(position, &position, 10);
            acc += amount;

            while((*position) != '\n') position++;
            currentLine++;
            position++;

            if(amount >= 0){
                printf("acc %c%i\n", sign, amount);
            }else{
                printf("acc %i\n", amount);
            }
        }else if(strings_equal(position, "jmp")){
            while(!isSign(*position)) position++;
            char csign = *position;
            int jumpAmount = strtol(position, &position, 10);

            currentLine += jumpAmount;

            int sign = (jumpAmount >= 0) ? 1 : -1;
            jumpAmount = abs(jumpAmount);
            if(sign < 0){
                jumpAmount += 2;
            }
            for(int i = 0; i < jumpAmount; i++){
                while((*position) != '\n'){ 
                    position += (char)sign;
                }
                position += sign;
            }
            if(sign < 0){
                position += 2;
            }

            if(sign >= 0){
                printf("jmp %c%i\n", csign, jumpAmount);
            }else{
                printf("jmp -%i\n", jumpAmount - 2);
            }
        }else if(strings_equal(position, "nop")){
            while(!isSign(*position)) position++;
            char sign = *position;
            int amount = strtol(position, &position, 10);

            while((*position) != '\n') position++;
            currentLine++;
            position++;
            
            if(amount >= 0){
                printf("nop %c%i\n", sign, amount);
            }else{
                printf("nop %i\n", amount);
            }
        }
    }

    finished = true;
    printf("Program finished!\n");

    end: printf("Result acc: %i\n", acc);
    return {finished, acc};
}

int main(){
    String text = readFile("input.txt");
    char* copy = (char*) malloc(text.size + 1);

    char* position = text.data;

    ProgramResult result;
    do{
        while(!strings_equal(position, "jmp")) position++;
        strncpy(copy, text.data, text.size);
        strcpy(copy + (position - text.data), "nop");

        result = runProgram({text.size, copy});

        while((*position) != '\n') position++;
        position++;
    }while(!result.finished && (*position) != '\0');
    
    return 0;
}