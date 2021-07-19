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
    long minimum_amount;
    long maximum_amount;
    char policy_char;
    String text;
};

Password parsePassword(char** text){
    char* position = *text;

    long minimum = strtol(position, &position, 10);
    if(minimum <= 0) return (Password){};

    position++;
    
    long maximum = strtol(position, &position, 10);
    if(maximum <= 0) return (Password){};

    position++;

    Password password;
    password.minimum_amount = minimum;
    password.maximum_amount = maximum;
    
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
    int char_count = 0;
    for(int i = 0; i < password.text.size; i++){
        if(password.text.data[i] == password.policy_char){
            char_count++;
        }
    }
    return char_count >= password.minimum_amount && char_count <= password.maximum_amount;
}

int main(){
    String text = readFile("input.txt");

    char* position = text.data;

    int validPasswordsCount = 0;
    while(true){
        Password p = parsePassword(&position);

        if(p.minimum_amount <= 0){
            break;
        }

        printf("%ld-%ld %c: %.*s", p.minimum_amount, p.maximum_amount, p.policy_char, (int)p.text.size, p.text.data);
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