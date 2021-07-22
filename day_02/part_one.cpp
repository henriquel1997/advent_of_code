#include "..\common.h"

struct Password {
    long minimum_amount;
    long maximum_amount;
    char policy_char;
    String text;
};

Password parsePassword(char** text){
    char* position = *text;

    long minimum = strtol(position, &position, 10);
    if(minimum <= 0) return {};

    position++;
    
    long maximum = strtol(position, &position, 10);
    if(maximum <= 0) return {};

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
    
    password.text = {passwordLength, passwordStart};

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