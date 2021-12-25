#include "common.h"
#include "alu_program.h"

/*uint64 interpret_alu(String instructions, uint64 input){
	uint64 reverse = 0;
	while (input){
		reverse *= 10;
		reverse += input % 10;
		input /= 10;
	}

	char* position = instructions.data;

	int64 w = 0;
	int64 x = 0;
	int64 y = 0;
	int64 z = 0;

	while ((position - instructions.data) < instructions.size) {
		char* instruction_name = position;
		while (!isWhiteSpace(*position)) position++;
		position++;

		if (startsWith(instruction_name, "inp")){
			uint64 digit = reverse % 10;
			reverse /= 10;

			if (*position == 'w'){
				w = digit;
			} else if (*position == 'x'){
				x = digit;
			} else if (*position == 'y'){
				y = digit;
			} else if (*position == 'z'){
				z = digit;
			} else {
				printf("Unknown inp value\n");
				exit(1);
			}
		} else {
			char* variable = position;
			while (!isWhiteSpace(*position)) position++;
			position++;

			int64 value;
			if (*position == '-' || isDigit(*position)){
				value = strtoll(position, &position, 10);
			} else if (*position == 'w'){
				value = w;
			}else if (*position == 'x'){
				value = x;
			}else if (*position == 'y'){
				value = y;
			}else if (*position == 'z'){
				value = z;
			} else {
				printf("Unknown value\n");
				exit(1);
			}

			if (startsWith(instruction_name, "add")) {
				if (*variable == 'w') {
					w += value;
				} else if (*variable == 'x') {
					x += value;
				} else if (*variable == 'y') {
					y += value;
				} else if (*variable == 'z') {
					z += value;
				} else {
					printf("Unknown add variable\n");
					exit(1);
				}
			} else if (startsWith(instruction_name, "mul")) {
				if (*variable == 'w') {
					w *= value;
				} else if (*variable == 'x') {
					x *= value;
				} else if (*variable == 'y') {
					y *= value;
				} else if (*variable == 'z') {
					z *= value;
				} else {
					printf("Unknown mul variable\n");
					exit(1);
				}
			} else if (startsWith(instruction_name, "div")) {
				if (*variable == 'w') {
					w /= value;
				} else if (*variable == 'x') {
					x /= value;
				} else if (*variable == 'y') {
					y /= value;
				} else if (*variable == 'z') {
					z /= value;
				} else {
					printf("Unknown div variable\n");
					exit(1);
				}
			} else if (startsWith(instruction_name, "mod")) {
				if (*variable == 'w') {
					w %= value;
				} else if (*variable == 'x') {
					x %= value;
				} else if (*variable == 'y') {
					y %= value;
				} else if (*variable == 'z') {
					z %= value;
				} else {
					printf("Unknown mod variable\n");
					exit(1);
				}
			} else if (startsWith(instruction_name, "eql")) {
				if (*variable == 'w'){
					w = w == value;
				}else if (*variable == 'x'){
					x = x == value;
				}else if (*variable == 'y'){
					y = y == value;
				}else if (*variable == 'z'){
					z = z == value;
				} else {
					printf("Unknown eql variable\n");
					exit(1);
				}
			} else {
				printf("Unknown instruction\n");
				exit(1);
			}
		}

		while (*position != '\n') position++;
		position++;
	}

	return z;
}*/

uint64 run_program_refactored(uint64 input) {
	uint64 reverse = 0;
	while (input) {
		reverse *= 10;
		reverse += input % 10;
		input /= 10;
	}

	int64 array0[14] = { 12, 12, 12, -9, -9, 14, 14, -10, 15, -2, 11, -15, -9, -3 };
	int64 array1[14] = {  1,  1,  1, 26, 26,  1,  1,  26,  1, 26,  1,  26, 26, 26 };
	int64 array2[14] = {  9,  4,  2,  5,  1,  6, 11,  15,  7, 12, 15,   9, 12, 12 };

	int64 w = 0;
	int64 z = 0;

	for (int i = 0; i < 14; i++) {

		w = reverse % 10;
		reverse /= 10;

		int64 x = ((z % 26) + array0[i]) != w;
		z /= array1[i];
		if (x){
			z *= 26;
			z += w + array2[i];
		}

		printf("w: %lli, z: %lli\n", w, z);
	}

	return z;
}

int main(){
	//String text = readFile("input.txt");

	//char digits [14] = { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9 };
	char digits [14] = { 1, 1, 8, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	while(true){
		bool print = digits[13] == 9 && digits[12] == 9 && digits[11] == 9 && digits[10] == 9 && digits[9] == 9 && digits[8] == 9  && digits[7] == 9;
		//bool print = false;

		uint64 input = 0;
		for (int i = 0; i < 14; i++){
			input *= 10;
			input += digits[i];
			if(print) printf("%i", digits[i]);
		}

		//uint64 result = interpret_alu(text, input);
		printf("Compiled program\n");
		uint64 result = run_program(input);
		printf("Refactored program\n");
		uint64 result_ref = run_program_refactored(input);
		if(print) printf(" - z register: %llu\n", result);
		if (result == 0){
			printf("Largest valid input: %llu\n", input);
			return 0;
		} else if (result != result_ref){
			printf("Result was different for input %llu\n", input);
			printf("Result %llu\n", result);
			printf("Result ref %llu\n", result_ref);
			return -1;
		}

		return 0;

		bool next;
		//int i = 13;
		int i = 0;
		do{
			next = false;
			/*digits[i]--;
			if (digits[i] == 0){
				digits[i] = 9;
				i--;
				next = true;
			}*/

			digits[i]++;
			if (digits[i] == 10){
				digits[i] = 1;
				i++;
				next = true;
			}
		//}while (next && i >= 0);
		}while (next && i < 14);

		//if (i < 0){
		if (i >= 14){
			printf("No input was valid\n");
			return -1;
		}
	}
	return 0;
}