#include "common.h"

struct BitCount{
    uint64 zero;
    uint64 one;
};

#define NUMBER_OF_BITS 12

int main(){
	String text = readFile("input.txt");
    BitCount count[NUMBER_OF_BITS] = { 0, 0 };
	
	char* position = text.data;

    int index = 0;
    while((position - text.data) < text.size){
        if(*position == '0'){
            count[NUMBER_OF_BITS - 1 - index].zero++;
            index++;
            position++;
        }else if(*position == '1'){
            count[NUMBER_OF_BITS - 1 - index].one++;
            index++;
            position++;
        }else{
            index = 0;
            while(!isDigit(*position) && (position - text.data) < text.size) 
                position++;
        }

        if(index > NUMBER_OF_BITS){
            printf("Number of bits is greater than %i\n", NUMBER_OF_BITS);
            return -100;
        }
    }

    uint64 gamma_rate = 0;
    uint64 epsilon_rate = 0;
    for(int i = NUMBER_OF_BITS - 1; i >= 0; i--){
        if(count[i].zero > count[i].one){
            epsilon_rate |= 1 << i;
        }else{
            gamma_rate |= 1 << i;
        }
    }

    printf("gamma rate: %llu\n", gamma_rate);
    printf("epsilon rate: %llu\n", epsilon_rate);
    printf("power consumption: %llu\n", gamma_rate * epsilon_rate);
}