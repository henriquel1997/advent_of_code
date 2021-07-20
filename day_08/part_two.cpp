#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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

bool strings_equal(char* str1, char* str2, int length){
    for(int i = 0; i < length; i++){
        if(str1[i] != str2[i]){
            return false;
        }
    }
    return true;
}

bool strings_equal(char* str1, const char* str2){
    return strings_equal(str1, (char*) str2, strlen(str2));
}

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
    return (ProgramResult){finished, acc};
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

        result = runProgram((String){text.size, copy});

        while((*position) != '\n') position++;
        position++;
    }while(!result.finished && (*position) != '\0');
    
    return 0;
}