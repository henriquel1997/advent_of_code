#include "common.h"

#define SIMULATION_DAYS 256

int main(){
	String text = readFile("input.txt");
	
	char* position = text.data;

    uint64 fishes_per_day[9] = { 0 };

    int index = 0;
    while((position - text.data) < text.size){
        long int number = strtol(position, &position, 10);
        position++;

        fishes_per_day[number]++;
    }

    for(int day = 0; day < SIMULATION_DAYS; day++){
        uint64 fishes_to_add = fishes_per_day[0];
        for(int i = 0; i < 8; i++){
            fishes_per_day[i] = fishes_per_day[i + 1];
        }
        fishes_per_day[6] += fishes_to_add;
        fishes_per_day[8] = fishes_to_add;
    }

    uint64 fish_count = 0;
    for(int i = 0; i < 9; i++){
        fish_count += fishes_per_day[i];
    }

    printf("Fish count: %llu\n", fish_count);
    return 0;
}