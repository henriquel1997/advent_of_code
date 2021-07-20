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

#define array_size(array) (sizeof(array)/sizeof(array[0]))

void goToNextField(char** position){
    while(**position != ' ' && **position != '\n'){
        (*position)++;
    }
    (*position)++;
}

bool parseAndValidate(char** position){
    const char* requiredFields [] = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"}; //ignoring cid
    bool flags[array_size(requiredFields)] = {0};

    while(true){
        for(int i = 0; i < array_size(requiredFields); i++){
            if(!flags[i] && strings_equal(*position, (char*)requiredFields[i], strlen(requiredFields[i]))){
                flags[i] = true;
                break;
            }
        }
        goToNextField(position);
        if(**position == '\n' || **position == '\0'){
            break;
        }
    }

    for(int i = 0; i < array_size(requiredFields); i++){
        if(!flags[i]){
            return false;
        }
    }

    return true;
}

int main(){
    String text = readFile("input.txt");

    long validPassportsCount = 0;
    while(true){
        if(parseAndValidate(&text.data)){
            validPassportsCount++;
        }
        if(text.data[0] == '\0'){
            break;
        }
    }

    printf("Number of valid passports: %ld\n", validPassportsCount);
    return -1;
}