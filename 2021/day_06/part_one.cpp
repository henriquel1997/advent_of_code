#include "common.h"

#define ARRAY_SIZE 1000000
#define SIMULATION_DAYS 80

int main(){
	String text = readFile("input.txt");
	
	char* position = text.data;

    uint64 fish_count = 0;
    char fishes[ARRAY_SIZE];

    int index = 0;
    while((position - text.data) < text.size){
        long int number = strtol(position, &position, 10);
        position++;

        fishes[fish_count++] = number;
        if(fish_count >= ARRAY_SIZE){
            printf("Array is too small\n");
            return -1;
        }
    }

    for(int day = 0; day < SIMULATION_DAYS; day++){
        uint64 fishes_to_add = 0;
        for(int i = 0; i < fish_count; i++){
            if(fishes[i] == 0){
                fishes[i] = 6;
                fishes_to_add++;
            }else{
                fishes[i]--;
            }
        }

        for(int i = 0; i < fishes_to_add; i++){
            fishes[fish_count++] = 8;
            if(fish_count >= ARRAY_SIZE){
                printf("Array is too small\n");
                return -1;
            }
        }
    }

    printf("Fish count: %llu\n", fish_count);
    return 0;
}