#include "..\common.h"

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