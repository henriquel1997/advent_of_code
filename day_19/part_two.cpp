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

bool isRuleValid(char** text, Memory memory, uint64 ruleId, int64 nextRuleId = -1);

bool isSequenceValid(char** text, Sequence sequence, Memory memory, int64 nextRuleId){
	char* start = *text;

	for(uint64 i = 0; i < sequence.numIds; i++){
		uint64 id = sequence.ids[i];
		int64 nextId = (i + 1) < sequence.numIds ? sequence.ids[i + 1] : -1;
		if(!isRuleValid(text, memory, id, nextId)){
			*text = start;
			return false;
		}
	}
	return true;
}

bool isRule8Valid(char** text, Memory memory, int64 nextRuleId = -1){
	char* start = *text;
	if (!isRuleValid(text, memory, 42)) return false;

	if (nextRuleId >= 0){
		char* next = *text;
		char* nextStart = next;
		while(!isRuleValid(&next, memory, nextRuleId)){
			next++;
			nextStart = next;
			if(*next == '\n'){
				*text = start;
				return false;
			}
		}

		if(nextStart == *text){
			return true;
		}
		
		if(!isRule8Valid(text, memory) || *text < nextStart){
			*text = start;
			return false;
		}
	} else {
		while (isRuleValid(text, memory, 42));
	}
	return true;
}

bool isRule11Valid(char** text, Memory memory){
	char* start = *text;
	
	uint64 firstCount = 0;
	while (isRuleValid(text, memory, 42)) firstCount++;
	
	if(firstCount > 0){
		uint64 secondCount = 0;
		while (isRuleValid(text, memory, 31)) secondCount++;
		if(firstCount == secondCount){
			return true;
		}
	}

	*text = start;
	return false;
}

bool isRuleValid(char** text, Memory memory, uint64 ruleId, int64 nextRuleId){
	if(ruleId == 8){
		return isRule8Valid(text, memory, nextRuleId);
	} else if (ruleId == 11){
		return isRule11Valid(text, memory);
	}

	Rule rule = memory.rules[ruleId];

	if(rule.letter){
		return *((*text)++) == rule.letter;
	}

	if(rule.numSequences == 1){
		return isSequenceValid(text, rule.firstSequence, memory, nextRuleId);
	}

	return isSequenceValid(text, rule.firstSequence, memory, nextRuleId) || 
		   isSequenceValid(text, rule.secondSequence, memory, nextRuleId);
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
		if(isRuleValid(&position, memory, 0)){
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