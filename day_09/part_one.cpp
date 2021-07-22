#include "..\common.h"

#define BUFFER_SIZE 25

int main(){
    String text = readFile("input.txt");

    char* position = text.data;
    uint64 buffer[BUFFER_SIZE];
    int startIndex = 0;
    int numElementsInBuffer = 0;
    
    int cont = 1;
    while(*(position + 1) != '\0' && ((position - text.data) < text.size)){
        uint64 value = strtoull(position, &position, 10);
        printf("%i: %lld - ", cont++, value);
        
        if(numElementsInBuffer < BUFFER_SIZE){
            printf("Preamble\n");
            buffer[numElementsInBuffer++] = value;
        }else{
            bool validValue = false;
            for(int i = 0; i < BUFFER_SIZE; i++){
                uint64 first = buffer[(i + startIndex) % BUFFER_SIZE];
                for(int j = 0; j < BUFFER_SIZE; j++){
                    if(i != j){
                        uint64 second = buffer[(j + startIndex) % BUFFER_SIZE];
                        if(first + second == value){
                            validValue = true;
                            printf("Valid (%lld + %lld)\n", first, second);
                            goto endSearch;
                        }
                    }
                }
            }
            endSearch:
            if(validValue){
                buffer[startIndex] = value;
                startIndex = (startIndex + 1) % BUFFER_SIZE;
            }else{
                printf("Invalid value\n");
                break;
            }
        }
    }
    return 0;
}