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

bool isValueValid(uint64 value, Rule rule){
	return (value >= rule.range1.min && value <= rule.range1.max) ||
		   (value >= rule.range2.min && value <= rule.range2.max);
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

	//Parsing nearby tickets
	while ((position - text.data) < text.size) {
		while (!isDigit(*position)) position++;
		if(numValues > MAX_NUM_VALUES){
			printf("Trying to set value at index %llu", numValues);
			return -3;
		}
		uint64 value = strtoull(position, &position, 10);

		bool valid = false;
		for(int i = 0; i < numRules; i++){
			if(isValueValid(value, rules[i])){
				valid = true;
				break;
			}
		}
		
		if(!valid){
			numValues -= numValues % numRules;
			while (*position != '\n') position++;
		} else {
			values[numValues++] = value;
		}
		position++;
	}
	
	uint64 count [MAX_NUM_RULES][MAX_NUM_RULES] = { 0 };
	for(int ruleIndex = 0; ruleIndex < numRules; ruleIndex++){
		Rule rule = rules[ruleIndex];
		for (uint64 ticketStart = 0; ticketStart < numValues; ticketStart += numRules){
			for (uint64 col = 0; col < numRules; col++){
				uint64 value = values[ticketStart + col];
				if(isValueValid(value, rule)){
					count[ruleIndex][col]++;
				}
			}
		}
	}

	uint64 numTickets = numValues / numRules;

	int64 columnForRule [MAX_NUM_RULES];
	int64 ruleForColumn [MAX_NUM_RULES];
	for(int i = 0; i < numRules; i++){
		columnForRule[i] = -1;
		ruleForColumn[i] = -1;
	}

	for (uint64 n = 0; n < numRules; n++) {
		for (uint64 ruleIndex = 0; ruleIndex < numRules; ruleIndex++) {
			if (columnForRule[ruleIndex] >= 0) continue;
			
			int64 column = -1;
			for (int64 col = 0; col < numRules; col++) {
				if (ruleForColumn[col] >= 0) continue;
				
				if (count[ruleIndex][col] == numTickets) {
					if (column < 0) {
						column = col;
					} else {
						column = -1;
						break;
					}
				}
			}
			if (column >= 0) {
				columnForRule[ruleIndex] = column;
				ruleForColumn[column] = ruleIndex;
				break;
			}
		}
	}

	uint64 departureTotal = 1;
	for(uint64 i = 0; i < numRules; i++){
		Rule rule = rules[i];
		if(startsWith(rule.name, "departure")){
			uint64 value = values[columnForRule[i]];
			printf("%.*s: %llu\n", (int)rule.name.size, rule.name.data, value);
			departureTotal *= values[columnForRule[i]];
		}
	}

	printf("Departure field values multiplied together: %llu\n", departureTotal);
	return 0;
}