#include "..\common.h"

struct Range{
	uint64 min;
	uint64 max;
};

struct Rule {
	String name;
	Range range1;
	Range range2;
};

Range parseRange(char** text){
	while (!isDigit(**text)) (*text)++;

	uint64 min = strtoull(*text, text, 10);
	(*text)++;
	uint64 max = strtoull(*text, text, 10);
	(*text)++;

	return {min, max};
}

#define MAX_NUM_VALUES 10000
#define MAX_NUM_RULES 20

int main(){
	String text = readFile("input.txt");
	
	char* position = text.data;

	//Parsing the rules
	uint64 numRules = 0;
	Rule rules[MAX_NUM_RULES];
	while (*position != '\n' && (position - text.data) < text.size) {
		char* nameStart = position;
		while (*position != ':') position++;
		String name = {(long)(position - nameStart), nameStart};

		position++;

		Range range1 = parseRange(&position);
		Range range2 = parseRange(&position);

		if(numRules > MAX_NUM_RULES){
			printf("Trying to set rule at index %llu", numRules);
			return -1;
		}
		rules[numRules++] = {name, range1, range2};
	}

	position++;

	//Parsing your ticket
	uint64 numValues = 0;
	uint64* values = (uint64*) malloc(sizeof(uint64) * MAX_NUM_VALUES);

	while (*position != '\n' && (position - text.data) < text.size) {
		while (!isDigit(*position)) position++;
		if(numValues > MAX_NUM_VALUES){
			printf("Trying to set value at index %llu", numValues);
			return -2;
		}
		values[numValues++] = strtoull(position, &position, 10);
	}

	position++;

	uint64 ticketSize = numValues;

	//Parsing nearby tickets
	while ((position - text.data) < text.size) {
		while (!isDigit(*position)) position++;
		if(numValues > MAX_NUM_VALUES){
			printf("Trying to set value at index %llu", numValues);
			return -3;
		}
		values[numValues++] = strtoull(position, &position, 10);

		position++;
	}

	uint64 invalidSum = 0;
	for (int i = 0; i < numValues; i++){
		uint64 value = values[i];

		bool valid = false;
		for(int k = 0; k < numRules; k++){
			Rule rule = rules[k];
			if(
				(value >= rule.range1.min && value <= rule.range1.max) ||
				(value >= rule.range2.min && value <= rule.range2.max)
			){
				valid = true;
				break;
			}
		}

		if(!valid){
			printf("Invalid value %llu\n", value);
			invalidSum += value;
		}
	}

	printf("Invalid sum: %llu\n", invalidSum);
	return 0;
}