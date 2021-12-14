#include "common.h"

#define POLYMER_MAX_SIZE 100000
#define NUM_ITERATIONS 10

int main(){
    String text = readFile("input.txt");
	
    char* position = text.data;

    uint64 polymer_size = 0;
    char* polymer = (char*) malloc(POLYMER_MAX_SIZE * sizeof(char));
    
    while(!isEndOfLine(*position))
        polymer[polymer_size++] = *(position++);
    polymer[polymer_size] = '\0';

    printf("Polymer Template: %s (%llu)\n", polymer, polymer_size);

    while(!isLetterUpperCase(*position)) position++;

    char* rulesStart = position;
    //Generate polymer
    for(uint64 i = 0; i < NUM_ITERATIONS; i++){
        for(uint64 polymer_pos = 0; polymer_pos < polymer_size - 1; polymer_pos++){
            while((position - text.data) < text.size){
                if(strings_equal(&polymer[polymer_pos], position, 2)){
                    while(*position != '>') position++;
                    position++;
                    eatWhiteSpace(&position);

                    for(uint64 j = polymer_size; j > (polymer_pos + 1); j--){
                        polymer[j] = polymer[j - 1];
                    }
                    polymer[polymer_pos + 1] = *position;

                    polymer_size++;
                    if(polymer_size >= POLYMER_MAX_SIZE){
                        printf("Exceeded max polymer size\n");
                        return -1;
                    }

                    polymer_pos++;
                    break;
                }

                while((position - text.data) < text.size && !isEndOfLine(*position)) position++;
                position++;
            }
            position = rulesStart;
        }
    }

    //Find the most and least common element
    const int count_size = 'Z' - 'A';
    uint64 count[count_size] = { 0 };
    for(uint64 polymer_pos = 0; polymer_pos < polymer_size; polymer_pos++){
        count[polymer[polymer_pos] - 'A']++;
    }

    int most_common_pos = polymer[0] - 'A';
    int least_common_pos = most_common_pos;
    for(int i = 1; i < count_size; i++){
        if(count[i] == 0) continue;

        if(count[i] > count[most_common_pos]){
            most_common_pos = i;
        }
        if(count[i] < count[least_common_pos]){
            least_common_pos = i;
        }
    }

    polymer[polymer_size] = '\0';
    printf("Polymer Result: %s (%llu)\n", polymer, polymer_size);
    printf("Most common element: %c (%llu)\n", 'A' + most_common_pos, count[most_common_pos]);
    printf("Least common element: %c (%llu)\n", 'A' + least_common_pos, count[least_common_pos]);
    printf("Most common quantity - Least common quantity = %llu\n", count[most_common_pos] - count[least_common_pos]);
    return 0;
}