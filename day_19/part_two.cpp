#include "..\common.h"

struct Sequence {
	int numIds;
	uint64* ids;
};

struct Rule {
	char letter;
	int numSequences;
	Sequence firstSequence;
	Sequence secondSequence;
};

struct Memory {
	Rule* rules;
	uint64 totalIDs = 0;
	uint64* ids;
};

bool isRuleValid(char** text, Memory memory, uint64 ruleId);

bool isSequenceValid(char** text, Sequence sequence, Memory memory){
	char* start = *text;

	for(uint64 i = 0; i < sequence.numIds; i++){
		uint64 id = sequence.ids[i];
		if(!isRuleValid(text, memory, id)){
			*text = start;
			return false;
		}
	}
	return true;
}

bool isRuleValid(char** text, Memory memory, uint64 ruleId){

	Rule rule = memory.rules[ruleId];

	if(rule.letter){
		return *((*text)++) == rule.letter;
	}

	if(rule.numSequences == 1){
		return isSequenceValid(text, rule.firstSequence, memory);
	}

	return isSequenceValid(text, rule.firstSequence, memory) || 
		   isSequenceValid(text, rule.secondSequence, memory);
}

bool isRule8Valid(char** text, Memory memory, char* end){
	if (!isRuleValid(text, memory, 42)) return false;

	do{
		if(*text == end){
			return true;
		}
	}while (isRuleValid(text, memory, 42));
	return false;
}

bool isRule11Valid(char** text, Memory memory){
	char* start = *text;
	
	uint64 firstCount = 0;
	while (isRuleValid(text, memory, 42)) firstCount++;
	
	bool valid = false;
	if(firstCount > 0){
		uint64 secondCount = 0;
		while (isRuleValid(text, memory, 31)) secondCount++;
		if(firstCount == secondCount){
			valid = true;
		}
	}
	
	if(!valid) *text = start;
	return valid;
}

bool isRule0Valid(char** text, Memory memory){
	char* rule11Start = *text;
	while (*(rule11Start) != '\n') rule11Start++;
	char* endText = rule11Start;

	char* rule11Current;

	while (true) {
		do {
			rule11Start--;
			rule11Current = rule11Start;
			if (rule11Start < *text) {
				return false;
			}
		} while (!isRule11Valid(&rule11Current, memory) || (*rule11Current != '\n'));

		char* rule8End = *text;
		if (isRule8Valid(&rule8End, memory, rule11Start)) {
			*text = endText;
			return true;
		}
	}
	return false;
}

#define MAX_RULES 1000
#define MAX_TOTAL_IDS 10000

Sequence parseSequence(char** text, Memory* memory){
	Sequence sequence;
	sequence.numIds = 0;
	sequence.ids = &memory->ids[memory->totalIDs];
	
	do{
		if(memory->totalIDs > MAX_TOTAL_IDS){
			printf("Ids array is too small\n");
		}
		memory->ids[memory->totalIDs++] = strtoull(*text, text, 10);
		sequence.numIds++;
		eatWhiteSpace(text);
	}while(isDigit(**text));

	return sequence;
}

int main(){
	String text = readFile("input.txt");
	
	char* position = text.data;

	uint64 ids[MAX_TOTAL_IDS];
	Rule rules[MAX_RULES];

	Memory memory;
	memory.rules = &rules[0];
	memory.totalIDs = 0;
	memory.ids = &ids[0];

	if(!memory.rules){
		return -1;
	}

	position = text.data;

	//Parse the rules
	while(*position != '\n'){
		uint64 id = strtoull(position, &position, 10);
		if(id > MAX_RULES){
			printf("Rules array is too small\n");
			return -2;
		}
		position++;

		eatWhiteSpace(&position);
		if(isDigit(*position)){
			Rule* rule = &memory.rules[id];
			rule->firstSequence = parseSequence(&position, &memory);

			if(*position != '\n'){
				position++;
				eatWhiteSpace(&position);

				rule->secondSequence = parseSequence(&position, &memory);
				rule->numSequences = 2;
			} else {
				rule->numSequences = 1;
			}
		} else {
			position++;
			memory.rules[id] = {*(position++)};
			position++;
		}

		while (*position != '\n') position++;
		position++;
	}

	position++;

	//Validate the strings
	uint64 validStringCount = 0;
	while((position - text.data) < text.size){
		if(isRule0Valid(&position, memory)){
			if(*position == '\n'){
				validStringCount++;
			}
		}
		while (*position != '\n') position++;
		position++;
	}

	printf("Number of valid strings: %llu\n", validStringCount);
	return 0;
}