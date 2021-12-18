#include "common.h"

struct Pair {
	Pair* first_pair;
	Pair* second_pair;
	uint64 first_number_index;
	uint64 second_number_index;
};

#define MAX_PAIRS 1000
uint64 num_pairs = 0;
Pair* pairs; //Array
Pair* free_pairs = 0; //Linked list, the first_pair is the next pair in the list

#define MAX_NUMBERS (MAX_PAIRS * 4)
uint64 numbers_size = 0;
int64* numbers;

Pair* get_new_pair(){
	Pair* result;
	if (free_pairs){
		result = free_pairs;
		free_pairs = free_pairs->first_pair;
	} else {
		result = &pairs[num_pairs++];
	}
	*result = { 0 };
	return result;
}

Pair* parse_pair(char** position){
	Pair* result = 0;
	if (**position != '[') return 0;
	(*position)++;

	result = get_new_pair();
	
	if(isDigit(**position)){
		result->first_number_index = numbers_size++;
		numbers[result->first_number_index] = strtoll(*position, position, 10);
	} else if(**position == '['){
		result->first_pair = parse_pair(position);
	}
	
	while(**position != '[' && !isDigit(**position)) (*position)++;
	
	if(isDigit(**position)){
		result->second_number_index = numbers_size++;
		numbers[result->second_number_index] = strtoll(*position, position, 10);
	} else if(**position == '['){
		result->second_pair = parse_pair(position);
	}
	
	if (**position != ']') return 0;
	(*position)++;
	
	while(isEndOfLine(**position)) (*position)++;
	return result;
}

bool check_for_explosion(Pair* pair, int level = 1){
	if (!pair) return false;
	
	if(level == 4){
		//Explode
		Pair* exploded_pair = 0;
		if (pair->first_pair) {
			exploded_pair = pair->first_pair;
			pair->first_number_index = exploded_pair->first_number_index;
			pair->first_pair = 0;
		} else if (pair->second_pair) {
			exploded_pair = pair->second_pair;
			pair->second_number_index = exploded_pair->first_number_index;
			pair->second_pair = 0;
		}

		if(exploded_pair){
			uint64 left_index = exploded_pair->first_number_index;
			uint64 right_index = exploded_pair->second_number_index;
			int64 left_number = numbers[left_index];
			int64 right_number = numbers[right_index];

			for (uint64 i = 0; i < num_pairs; i++){
				Pair* pair = &pairs[i];
				if (!pair->first_pair && pair->first_number_index > (left_index + 1)){
					pair->first_number_index--;
				}
				
				if (!pair->second_pair && pair->second_number_index > (left_index + 1)){
					pair->second_number_index--;
				}
			}

			for (uint64 i = right_index; i < (numbers_size - 1); i++){
				numbers[i] = numbers[i + 1];
			}
			numbers_size--;

			if (left_index > 0){
				numbers[left_index - 1] += left_number;
			}

			if (left_index < (numbers_size - 1)){
				numbers[left_index + 1] += right_number;
			}

			numbers[left_index] = 0;

			exploded_pair->first_pair = 0;
			exploded_pair->second_pair = 0;
			
			if (free_pairs){
				Pair* prev_pair = free_pairs;
				while(prev_pair->first_pair) prev_pair = prev_pair->first_pair;
				prev_pair->first_pair = exploded_pair;
			} else {
				free_pairs = exploded_pair;
			}

			return true;
		}

		return false;
	}

	if (check_for_explosion(pair->first_pair, level + 1)) return true;
	if (check_for_explosion(pair->second_pair, level + 1)) return true;
	return false;
}

Pair* get_pair_from_splitted_number(uint64 splitted_number_index){
	int64 left = numbers[splitted_number_index] / 2;
	int64 right = left + (numbers[splitted_number_index] % 2);

	for (uint64 i = 0; i < num_pairs; i++){
		Pair* pair = &pairs[i];
		if (!pair->first_pair && pair->first_number_index > splitted_number_index){
			pair->first_number_index++;
		}
		
		if (!pair->second_pair && pair->second_number_index > splitted_number_index){
			pair->second_number_index++;
		}
	}

	numbers_size++;
	for(uint64 i = numbers_size - 1; i > (splitted_number_index + 1); i--){
		numbers[i] = numbers[i - 1];
	}
	
	Pair* new_pair = get_new_pair();
	new_pair->first_number_index = splitted_number_index;
	numbers[new_pair->first_number_index] = left;
	new_pair->second_number_index = splitted_number_index + 1;
	numbers[new_pair->second_number_index] = right;
	return new_pair;
}

bool check_for_split(Pair* pair){
	if (!pair) return false;

	if (pair->first_pair){
		if(check_for_split(pair->first_pair)) return true;
	}else if (numbers[pair->first_number_index] >= 10){
		pair->first_pair = get_pair_from_splitted_number(pair->first_number_index);
		return true;
	}
	
	if (pair->second_pair){
		if(check_for_split(pair->second_pair)) return true;
	} else if (numbers[pair->second_number_index] >= 10) {
		pair->second_pair = get_pair_from_splitted_number(pair->second_number_index);
		return true;
	}

	return false;
}

void print_numbers_array(){
	printf("Numbers: [");
	printf((numbers[0] >= 0 && numbers[0] < 10) ? " %lli" : "%lli", numbers[0]);
	for(uint64 i = 1; i < numbers_size; i++){
		printf((numbers[i] >= 0 && numbers[i] < 10) ? ",  %lli" : ", %lli", numbers[i]);
	}
	printf("]\n");
}

void print_pair(Pair* pair){
	printf("[");
	if (pair->first_pair) print_pair(pair->first_pair);
	else printf("%lli", numbers[pair->first_number_index]);
	printf(",");
	if (pair->second_pair) print_pair(pair->second_pair);
	else printf("%lli", numbers[pair->second_number_index]);
	printf("]");
}

Pair* add_pairs(Pair* pair1, Pair* pair2){
	Pair* result = get_new_pair();
	result->first_pair = pair1;
	result->second_pair = pair2;

	//Reduce the pair
	while (check_for_explosion(result) || check_for_split(result));

	return result;
}

uint64 get_pair_magnitude(Pair* pair){
	int64 left_magnitude;
	if(pair->first_pair){
		left_magnitude = get_pair_magnitude(pair->first_pair);
	} else {
		left_magnitude = numbers[pair->first_number_index];
	}

	int64 right_magnitude;
	if(pair->second_pair){
		right_magnitude = get_pair_magnitude(pair->second_pair);
	} else {
		right_magnitude = numbers[pair->second_number_index];
	}

	return (left_magnitude * 3) + (right_magnitude * 2);
}

int main(){
	String text = readFile("input.txt");
	
	char* position = text.data;
	
	pairs = (Pair*) malloc(MAX_PAIRS * sizeof(Pair));
	numbers = (int64*) malloc(MAX_NUMBERS * sizeof(int64));

	Pair* result = parse_pair(&position);
	while((position - text.data) < text.size){
		Pair* other = parse_pair(&position);
		result = add_pairs(result, other);
	}

	print_pair(result);
	printf("\n");
	print_numbers_array();
	printf("Result maginitude=%llu\n", get_pair_magnitude(result));
	return 0;
}