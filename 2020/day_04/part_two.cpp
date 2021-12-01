#include "..\common.h"

void goToNextField(char** position){
    while(**position != ' ' && **position != '\n'){
        (*position)++;
    }
    (*position)++;
}

void goToValue(char** position){
    while(**position != ':'){
        (*position)++;
    }
    (*position)++;
}

bool isHexLetter(char c){
    return c >= 'a' && c <= 'f';
}

bool parseAndValidate(char** position){
    const char* requiredFields [] = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"}; //ignoring cid
    bool flags[array_size(requiredFields)] = {0};

    while(true){
        int fieldIndex = -1;
        for(int i = 0; i < array_size(requiredFields); i++){
            if(!flags[i] && strings_equal(*position, (char*)requiredFields[i], strlen(requiredFields[i]))){
                fieldIndex = i;
                break;
            }
        }
        if(fieldIndex != -1){
            goToValue(position);

            bool isFieldValid = false;
            switch(fieldIndex){
                case 0:{
                    //Validate birth year
                    long value = strtol(*position, position, 10);
                    isFieldValid = value >= 1920 && value <= 2002;
                    break;
                }
                case 1:{
                    //Validate issue year
                    long value = strtol(*position, position, 10);
                    isFieldValid = value >= 2010 && value <= 2020;
                    break;
                }
                case 2:{
                    //Expiration year
                    long value = strtol(*position, position, 10);
                    isFieldValid = value >= 2020 && value <= 2030;
                    break;
                }
                case 3:{
                    //Height
                    long value = strtol(*position, position, 10);
                    if(strings_equal(*position, "cm")){
                        isFieldValid = value >= 150 && value <= 193;
                    }else if(strings_equal(*position, "in")){
                        isFieldValid = value >= 59 && value <= 76;
                    }
                    break;
                }
                case 4:{
                    //Hair Color
                    if(*((*position)++) == '#'){
                        isFieldValid = true;
                        for(int i = 0; i < 6; i++){
                            char c = *((*position)++);
                            if(!isDigit(c) && !isHexLetter(c)){
                                isFieldValid = false;
                                break;
                            }
                        }
                    }
                    break;
                }
                case 5:{
                    //Eye Color
                    const char* eyeColors [] = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
                    for(int i = 0; i < array_size(eyeColors); i++){
                        if(strings_equal(*position, (char*)eyeColors[i], strlen(eyeColors[i]))){
                            isFieldValid = true;
                            break;
                        }
                    }
                    break;
                }
                case 6:{
                    //Passport ID
                    isFieldValid = true;
                    for(int i = 0; i < 9; i++){
                        char c = *((*position)++);
                        if(!isDigit(c)){
                            isFieldValid = false;
                            break;
                        }
                    }
                    if(isDigit(**position)){
                        isFieldValid = false;
                    }
                    break;
                }
            }

            flags[fieldIndex] = isFieldValid;
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