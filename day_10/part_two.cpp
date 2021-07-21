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
#define NUMBERS_IN_LIST 101

uint64 numberOfPaths[NUMBERS_IN_LIST] = {0};

uint64 getNumberOfPathsForIndex(long* array, int size, int index){
    if(index >= size - 1){
        return 1;
    }

    uint64 totalNumberOfPaths = 0;
    long value = array[index];

    for(int i = 1; i <= 3; i++){
        int id = index + i;
        if(id >= size) break;

        long difference = array[id] - value;
        if(difference <= 3){
            if(numberOfPaths[id]){
                totalNumberOfPaths += numberOfPaths[id];
            }else{
                uint64 paths = getNumberOfPathsForIndex(array, size, id);
                numberOfPaths[id] = paths;
                totalNumberOfPaths += paths;
            }
        }else{
            break;
        }
    }
    return totalNumberOfPaths;
}

int main(){
    String text = readFile("input.txt");

    char* position = text.data;
    long numbers[NUMBERS_IN_LIST + 2];
    int size = 0;

    numbers[size++] = 0;
    
    //Insertion sort
    while(*(position + 1) != '\0' && size < (NUMBERS_IN_LIST + 2) && ((position - text.data) < text.size)){
        long value = strtol(position, &position, 10);

        int pos = size;
        for(int i = 0; i < size; i++){
            if(value < numbers[i]){
                pos = i;
                break;
            }
        }

        for(int i = size; i > pos; i--){
            numbers[i] = numbers[i - 1];
        }

        numbers[pos] = value;
        size++;
    }

    numbers[size] = numbers[size - 1] + 3;
    size++;

    uint64 answer = getNumberOfPathsForIndex(&numbers[0], size, 0);
    numberOfPaths[0] = answer;

    printf("Total number of combinations: %llu\n", answer);
    return 0;
}