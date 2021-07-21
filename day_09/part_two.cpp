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
    return {fsize, string};
}

#define uint64 unsigned long long
#define BUFFER_SIZE 25
#define NUMBERS_IN_LIST 1000

uint64 findInvalidNumber(uint64* numbers){
    uint64 buffer[BUFFER_SIZE];
    int startIndex = 0;
    int numElementsInBuffer = 0;
    
    int index = 0;
    while(index < NUMBERS_IN_LIST){
        uint64 value = numbers[index++];
        
        if(numElementsInBuffer < BUFFER_SIZE){
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
                return value;
            }
        }
    }

    return 0;
}

int main(){
    String text = readFile("input.txt");

    char* position = text.data;
    uint64 numbers[NUMBERS_IN_LIST];
    int index = 0;
    
    while(*(position + 1) != '\0' && index < NUMBERS_IN_LIST && ((position - text.data) < text.size)){
        numbers[index++] = strtoull(position, &position, 10);
    }

    uint64 invalidNumber = findInvalidNumber(&numbers[0]);
    if(invalidNumber > 0){
        for(int start = 0; start < NUMBERS_IN_LIST; start++){
            uint64 sum = 0;
            uint64 lowest = ULLONG_MAX;
            uint64 highest = 0;
            for(int i = start; i < NUMBERS_IN_LIST; i++){
                uint64 value = numbers[i];
                sum += value;
                if(sum > invalidNumber) break;
                if(value > highest) highest = value;
                if(value < lowest) lowest = value;
                if(sum == invalidNumber){
                    printf("Found the sum!\n");
                    printf("(%i - %i)\n", start + 1, i + 1);
                    printf("Lowest value: %lld\n", lowest);
                    printf("Highest value: %lld\n", highest);
                    printf("%lld + %lld = %lld\n", lowest, highest, lowest + highest);
                    return 0;
                }
            }
        }
    }
    return -1;
}