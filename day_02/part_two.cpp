#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

struct Password {
    long first_position;
    long second_position;
    char policy_char;
    String text;
};

Password parsePassword(char** text){
    char* position = *text;

    long first_position = strtol(position, &position, 10);
    if(first_position <= 0) return (Password){};

    position++;
    
    long second_position = strtol(position, &position, 10);
    if(second_position <= 0) return (Password){};

    position++;

    Password password;
    password.first_position = first_position;
    password.second_position = second_position;
    
    password.policy_char = position[0];
    char* passwordStart = &position[3];
    long passwordLength = 0;
    while(passwordStart[passwordLength] != '\n'){
        passwordLength++;
    }
    
    password.text = (String){passwordLength, passwordStart};

    *text = &passwordStart[passwordLength];

    return password;
}

bool validatePassword(Password password){
    bool isFirst = password.text.data[password.first_position - 1] == password.policy_char;
    bool isSecond = password.text.data[password.second_position - 1] == password.policy_char;
    return isFirst ^ isSecond;
}

int main(){
    String text = readFile("input.txt");

    char* position = text.data;

    int validPasswordsCount = 0;
    while(true){
        Password p = parsePassword(&position);

        if(p.first_position <= 0){
            break;
        }

        printf("%ld-%ld %c: %.*s", p.first_position, p.second_position, p.policy_char, (int)p.text.size, p.text.data);
        if(validatePassword(p)){
            validPasswordsCount++;
            printf(" - Valid\n");
        }else{
            printf(" - Invalid\n");
        }
    }
    printf("\nNumber of valid passwords: %i\n", validPasswordsCount);

    return 0;
}