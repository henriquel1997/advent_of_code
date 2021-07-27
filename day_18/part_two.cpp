#include "..\common.h"

int64 parseExpression(char** text);

int64 parseValue(char** text){
	char c = **text;
	if(c == '('){
		(*text)++;
		eatWhiteSpace(text);
		int64 result =  parseExpression(text);
		if(result == LLONG_MIN) return LLONG_MIN;
		eatWhiteSpace(text);
		c = *((*text)++);
		if (c != ')'){
			printf("Missing closing parenthesis ')'\n");
			return LLONG_MIN;
		}
		return result;
	} else if (isDigit(c)){
		int64 value = strtoll(*text, text, 10);
		return value;
	} else {
		printf("Unknown character: %c\n", c);
		return LLONG_MIN;
	}
}

int64 executeOperation(int64 value1, char operation, int64 value2){
	if(operation == '+'){
		return value1 + value2;
	} else if (operation == '*'){
		return value1 * value2;
	}

	printf("Unknown operator %c\n", operation);
	return LLONG_MIN;
}

int64 parseExpression(char** text){
	char operation = 0;
	int64 total;

	do {
		eatWhiteSpace(text);
		int64 value;
		if (operation == '*'){
			value = parseExpression(text);
		} else {
			value = parseValue(text);
		}
		if (value == LLONG_MIN) return LLONG_MIN;

		if (operation){
			total = executeOperation(total, operation, value);
			if (total == LLONG_MIN) return LLONG_MIN;
		} else {
			total = value;
		}

		eatWhiteSpace(text);
		operation = *((*text)++);
	} while (operation != '\n' && operation != ')');

	(*text)--;

	return total;
}

int main(){
	String text = readFile("input.txt");
	
	char* position = text.data;

	int64 resultSum = 0;
	uint64 expressionCount = 0;
	while((position - text.data) < text.size){
		int64 result = parseExpression(&position);
		if (result == LLONG_MIN){
			printf("Error at expression %llu\n", expressionCount);
			return -1;
		}
		resultSum += result;
		expressionCount++;
		position++;
	}

	printf("Sum of all expression results: %lld\n", resultSum);
	return 0;
}