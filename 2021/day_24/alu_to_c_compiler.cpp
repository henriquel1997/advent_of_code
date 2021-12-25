#include "common.h"

#define CODE_FILE_MAX_SIZE 100000

int main(){
	String text = readFile("input.txt");

	char* position = text.data;

	uint64 code_pos = 0;
	char* code = (char*) malloc(CODE_FILE_MAX_SIZE * sizeof(char));

	const char* code_start =
		"uint64 run_program(uint64 input){\n"
		"\tuint64 reverse = 0;\n"
		"\twhile (input){\n"
		"\t\treverse *= 10;\n"
		"\t\treverse += input % 10;\n"
		"\t\tinput /= 10;\n"
		"\t}\n\n"
		"\tint64 w = 0;\n"
		"\tint64 x = 0;\n"
		"\tint64 y = 0;\n"
		"\tint64 z = 0;\n\n";

	uint64 code_start_length = strlen(code_start);
	memcpy(code, code_start, code_start_length);
	code_pos += code_start_length;

	
	while ((position - text.data) < text.size) {
		char* instruction_name = position;
		while (!isWhiteSpace(*position)) position++;
		position++;

		char buffer [1000];
		buffer[0] = '\0';

		if (startsWith(instruction_name, "inp")){
			char variable = *position;
			sprintf(buffer, "\t%c = reverse %% 10;\n\treverse /= 10;\n", variable);
		} else if (startsWith(instruction_name, "eql")){
			char variable = *(position++);
			while (isWhiteSpace(*position)) position++;

			if (*position == '-' || isDigit(*position)) {
				sprintf(buffer, "\t%c = %c == %lli;\n", variable, variable, strtoll(position, &position, 10));
			} else {
				sprintf(buffer, "\t%c = %c == %c;\n", variable, variable, *position);
			}
		} else if (startsWith(instruction_name, "add")){
			char variable = *(position++);
			while (isWhiteSpace(*position)) position++;

			if (*position == '-' || isDigit(*position)) {
				sprintf(buffer, "\t%c += %lli;\n", variable, strtoll(position, &position, 10));
			} else {
				sprintf(buffer, "\t%c += %c;\n", variable, *position);
			}
		} else if (startsWith(instruction_name, "mul")){
			char variable = *(position++);
			while (isWhiteSpace(*position)) position++;

			if (*position == '-' || isDigit(*position)) {
				sprintf(buffer, "\t%c *= %lli;\n", variable, strtoll(position, &position, 10));
			} else {
				sprintf(buffer, "\t%c *= %c;\n", variable, *position);
			}
		} else if (startsWith(instruction_name, "div")){
			char variable = *(position++);
			while (isWhiteSpace(*position)) position++;

			if (*position == '-' || isDigit(*position)) {
				sprintf(buffer, "\t%c /= %lli;\n", variable, strtoll(position, &position, 10));
			} else {
				sprintf(buffer, "\t%c /= %c;\n", variable, *position);
			}
		} else if (startsWith(instruction_name, "mod")){
			char variable = *(position++);
			while (isWhiteSpace(*position)) position++;

			if (*position == '-' || isDigit(*position)) {
				sprintf(buffer, "\t%c %%= %lli;\n", variable, strtoll(position, &position, 10));
			} else {
				sprintf(buffer, "\t%c %%= %c;\n", variable, *position);
			}
		}

		if (buffer[0] != '\0') {
			uint64 buffer_length = strlen(buffer);
			memcpy(&code[code_pos], buffer, buffer_length);
			code_pos += buffer_length;
		}

		while (*position != '\n') position++;
		position++;
	}

	const char* code_end =
		"\treturn z;\n"
		"}\n";
	uint64 code_end_length = strlen(code_end);
	memcpy(&code[code_pos], code_end, code_end_length);
	code_pos += code_end_length;

	code[code_pos] = '\0';
	printf("%s", code);

	FILE* file_pointer = fopen("alu_program.h", "w");
	if (file_pointer){
		fputs(code, file_pointer);
		fclose(file_pointer);
	} else {
		printf("Couldn't write to file\n");
	}
	return 0;
}