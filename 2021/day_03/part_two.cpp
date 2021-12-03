#include "common.h"

struct BitCount{
    uint64 zero;
    uint64 one;
};

#define NUMBER_OF_BITS 12

void removeNumberFromArray(int index, uint64* array, uint64 array_size){
    for(int i = index + 1; i < array_size; i++){
        array[i - 1] = array[i];
    }
}

BitCount getBitCountAtPosition(int pos, uint64* array, uint64 array_size){
    BitCount count = {0, 0};

    uint64 mask = (1 << pos);

    for(int i = 0; i < array_size; i++){
        if(array[i] & mask){
            count.one++;
        }else{
            count.zero++;
        }
    }
    return count;
}

int main(){
	String text = readFile("input.txt");
    TextFileInfo info = getTextFileInfo(text);
    uint64 numbersBytes = info.numberOfLines * sizeof(uint64);
    uint64* numbers = (uint64*) malloc(numbersBytes);
	
	char* position = text.data;

    int index = NUMBER_OF_BITS - 1;
    int numberCount = 0;
    uint64 number = 0;
    while((position - text.data) < text.size){
        if(*position == '0'){
            index--;
            position++;
        }else if(*position == '1'){
            number |= 1 << (index--);
            position++;
        }else{
            if(index > -1){
                printf("Number of bits is lower than %i\n", NUMBER_OF_BITS);
                return -100;
            }

            index = NUMBER_OF_BITS - 1;
            numbers[numberCount++] = number;
            number = 0;
            while(!isDigit(*position) && (position - text.data) < text.size) 
                position++;
        }

        if(index < -1){
            printf("Number of bits is greater than %i\n", NUMBER_OF_BITS);
            return -101;
        }
    }

    printf("Searching for the oxygen generator rating\n");
    uint64* buffer = (uint64*) malloc(numbersBytes);;
    memcpy(buffer, numbers, numbersBytes);
    uint64 bufferSize = info.numberOfLines;

    for(int i = NUMBER_OF_BITS - 1; i >= 0 && bufferSize > 1; i--){
        
        BitCount count = getBitCountAtPosition(i, buffer, bufferSize);

        if(count.zero > count.one){
            for(int j = 0; j < bufferSize; j++){
                if(buffer[j] & (1 << i)){
                    removeNumberFromArray(j, buffer, bufferSize);
                    bufferSize--;
                    j--;
                }
            }
        }else{
            for(int j = 0; j < bufferSize; j++){
                if((~buffer[j]) & (1 << i)){
                    removeNumberFromArray(j, buffer, bufferSize);
                    bufferSize--;
                    j--;
                }
            }
        }
    }

    uint64 oxygen_generator_rating = buffer[0];
    printf("oxygen_generator_rating: %llu\n\n", oxygen_generator_rating);

    printf("Searching for the CO2 scrubber rating\n");
    memcpy(buffer, numbers, numbersBytes);
    bufferSize = info.numberOfLines;

    for(int i = NUMBER_OF_BITS - 1; i >= 0 && bufferSize > 1; i--){

        BitCount count = getBitCountAtPosition(i, buffer, bufferSize);

        if(count.one < count.zero){
            for(int j = 0; j < bufferSize; j++){
                if((~buffer[j]) & (1 << i)){
                    removeNumberFromArray(j, buffer, bufferSize);
                    bufferSize--;
                    j--;
                }
            }
        }else{
            for(int j = 0; j < bufferSize; j++){
                if(buffer[j] & (1 << i)){
                    removeNumberFromArray(j, buffer, bufferSize);
                    bufferSize--;
                    j--;
                }
            }
        }
    }

    uint64 co2_scrubber_rating = buffer[0];
    printf("co2_scrubber_rating: %llu\n\n", co2_scrubber_rating);

    printf("life support rating: %llu\n", oxygen_generator_rating * co2_scrubber_rating);
}