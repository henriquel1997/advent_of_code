#include "common.h"

int main(){
	String text = readFile("input.txt");
	
	char* position = text.data;

    uint64 crab_count = 0;
    while((position - text.data) < text.size){
        if(*(position++) == ',') crab_count++;
    }
    crab_count++;

    int64* positions = (int64*) malloc(crab_count * sizeof(int64));

    position = text.data;
    crab_count = 0;
    int64 max_position = 0;
    while((position - text.data) < text.size){
        long int number = strtol(position, &position, 10);
        position++;

        positions[crab_count++] = number;
        if(number > max_position) max_position = number;
    }

    int64 best_position = -1;
    uint64 least_fuel = 0xffffffffffffffff;
    for(int64 i = 0; i < max_position; i++){
        uint64 total_fuel = 0;
        for(uint64 j = 0; j < crab_count; j++){
            int64 steps_to_pos = abs(positions[j] - i);
            uint64 fuel_cost = 0;
            for(uint64 k = 1; k <= steps_to_pos; k++){
                fuel_cost += k;
            }
            total_fuel += fuel_cost;
            if(total_fuel > least_fuel) break;
        }
        if(total_fuel < least_fuel){
            least_fuel = total_fuel;
            best_position = i;
        }
    }

    printf("Best position: %lli (%llu fuel)\n", best_position, least_fuel);
    return 0;
}