#include "common.h"

#define NUM_ITERATIONS 40

struct Pair {
    char letters[2];
    uint64 count;
    uint64 next_iteration_addition;
};

#define get_pair_pos(a, b) ((((a) - 'A') * num_letters) + ((b) - 'A'))

int main(){
    String text = readFile("input.txt");
	
    char* position = text.data;

    const int num_letters = 'Z' - 'A';
    uint64 num_pairs = num_letters * num_letters;
    Pair* pairs = (Pair*) calloc(num_pairs, sizeof(Pair));

    while(!isEndOfLine(*(position + 1))){
        char a = *position;
        char b = *(position + 1);
        
        Pair* pair = &pairs[get_pair_pos(a, b)];
        pair->letters[0] = a;
        pair->letters[1] = b;
        pair->count++;
        position++;
    }
    char last_letter = *position;

    printf("Pairs before iterations:\n");
    for(uint64 i = 0; i < num_pairs; i++){
        Pair p = pairs[i];
        if(p.count == 0) continue;
        printf("%c%c: %llu\n", p.letters[0], p.letters[1], p.count);
    }

    while(!isLetterUpperCase(*position)) position++;
    char* rulesStart = position;

    for(uint64 i = 0; i < NUM_ITERATIONS; i++){
        while((position - text.data) < text.size){
            char a = *position;
            char b = *(position + 1);
            Pair* pair = &pairs[get_pair_pos(a, b)];
            if(pair->count > 0){
                while(*position != '>') position++;
                position++;
                eatWhiteSpace(&position);

                char c = *(position++);
                Pair* result = &pairs[get_pair_pos(a, c)];
                result->letters[0] = a;
                result->letters[1] = c;
                result->next_iteration_addition += pair->count;

                result = &pairs[get_pair_pos(c, b)];
                result->letters[0] = c;
                result->letters[1] = b;
                result->next_iteration_addition += pair->count;

                pair->count = 0;
            }

            while((position - text.data) < text.size && !isEndOfLine(*position)) position++;
            position++;
        }

        position = rulesStart;

        for(uint64 j = 0; j < num_pairs; j++){
            pairs[j].count += pairs[j].next_iteration_addition;
            pairs[j].next_iteration_addition = 0;
        }
    }

    printf("\nPairs after iterations:\n");

    uint64 count[num_letters] = { 0 };
    for(uint64 i = 0; i < num_pairs; i++){
        Pair p = pairs[i];
        if(p.count == 0) continue;
        count[p.letters[0] - 'A'] += p.count;

        printf("%c%c: %llu\n", p.letters[0], p.letters[1], p.count);
    }
    count[last_letter - 'A']++;

    int most_common_pos = text.data[0] - 'A';
    int least_common_pos = most_common_pos;
    for(int i = 1; i < num_letters; i++){
        if(count[i] == 0) continue;

        if(count[i] > count[most_common_pos]){
            most_common_pos = i;
        }
        if(count[i] < count[least_common_pos]){
            least_common_pos = i;
        }
    }

    printf("Most common element: %c (%llu)\n", 'A' + most_common_pos, count[most_common_pos]);
    printf("Least common element: %c (%llu)\n", 'A' + least_common_pos, count[least_common_pos]);
    printf("Most common quantity - Least common quantity = %llu\n", count[most_common_pos] - count[least_common_pos]);
    return 0;
}