#include "common.h"

uint64 num_starts = 0;
uint64 packet_starts[10000] = {0};
uint64 num_ends = 0;
uint64 packet_ends[10000] = {0};

char hex_digit_to_bits(char hex){
	if(isDigit(hex)){
		return hex - '0';
	}else{
		return hex - 'A' + 10;
	}
}

//From https://stackoverflow.com/a/30590727
bool get_bit(char A[], uint64 k){
	return A[k/8] & 1 << (7 - (k%8));
}

uint64 get_bits(char A[], uint64 pos, uint64 num_bits){
	uint64 result = 0;
	for(uint64 bit = 0; bit < num_bits; bit++){
		result = result << 1;
		result |= get_bit(A, pos + bit);
	}
	return result;
}

uint64 parse_packet(uint64* current_bit, char bytes[]);

uint64 do_math_operation(char bytes[], uint64* current_bit,  uint64 packet_type_id, uint64 value){
	uint64 result = parse_packet(current_bit, bytes);
	switch (packet_type_id) {
		case 0:
			return value + result;
		case 1:
			return value * result;
		case 2:
			return min(value, result);
		default:
		case 3:
			return max(value, result);
	}
}

uint64 do_operation(char bytes[], uint64* current_bit,  uint64 packet_type_id){
	uint64 value;
	if (packet_type_id < 4) {
		switch (packet_type_id) {
			case 1:
				value = 1;
				break;
			case 2:
				value = ULLONG_MAX;
				break;
			default:
				value = 0;
				break;
		}

		if (get_bit(bytes, *current_bit) == 0) {
			*current_bit += 1;
			uint64 sub_packets_size = get_bits(bytes, *current_bit, 15);
			*current_bit += 15;

			uint64 packet_start_bit = *current_bit;
			while ((*current_bit) - packet_start_bit < sub_packets_size) {
				value = do_math_operation(bytes, current_bit, packet_type_id, value);
			}
		} else {
			*current_bit += 1;
			uint64 num_sub_packets = get_bits(bytes, *current_bit, 11);
			*current_bit += 11;
			
			for(uint64 i = 0; i < num_sub_packets; i++){
				value = do_math_operation(bytes, current_bit, packet_type_id, value);
			}
		}
	} else {
		if (get_bit(bytes, *current_bit) == 0) {
			*current_bit += 1 + 15;
		} else {
			*current_bit += 1 + 11;
		}
		uint64 first = parse_packet(current_bit, bytes);
		uint64 second = parse_packet(current_bit, bytes);
		switch (packet_type_id) {
			case 5:
				value = first > second;
				break;
			case 6:
				value = first < second;
				break;
			case 7:
				value = first == second;
				break;
		}
	}

	return value;
}

uint64 parse_packet(uint64* current_bit, char bytes[]){
	packet_starts[num_starts++] = *current_bit;
	
	*current_bit += 3;
	uint64 packet_type_id = get_bits(bytes, *current_bit, 3);
	*current_bit += 3;
	
	uint64 value;
	if(packet_type_id == 4){
		//Literal value
		value = 0;
		bool continue_parse;
		do{
			continue_parse = get_bit(bytes, *current_bit);
			*current_bit += 1;
			value = value << 4;
			value |= get_bits(bytes, *current_bit, 4);
			*current_bit += 4;
		} while (continue_parse);
	}else{
		//Operator
		value = do_operation(bytes, current_bit, packet_type_id);
	}
	
	packet_ends[num_ends++] = *current_bit;
	return value;
}

int main(){
	String text = readFile("input.txt");
	
	char* position = text.data;
	
	uint64 num_bytes = 0;
	char* bytes = (char*) malloc((text.size / 2) + (text.size % 2));
	
	while((position - text.data) < text.size){
		char result = hex_digit_to_bits(*(position++)) << 4;
		
		if((position - text.data) < text.size){
			char second_part = hex_digit_to_bits(*(position++));
			result |= 0xF;
			result &= second_part | 0xF0;
		}
		
		bytes[num_bytes++] = result;
	}
	
	uint64 current_bit = 0;
	uint64 result = parse_packet(&current_bit, bytes);
	
	uint64 current_start = 0;
	uint64 current_end = 0;
	for(uint64 current_bit = 0; current_bit < (num_bytes * 8); current_bit++){
		while(current_start < num_starts && packet_starts[current_start] == current_bit){
			current_start++;
			printf("[");
		}
		printf("%i", get_bit(bytes, current_bit));
		while(current_end < num_ends && packet_ends[current_end] == current_bit + 1){
			current_end++;
			printf("]");
		}
	}
	printf("\n");
	
	printf("Result: %llu\n", result);
	return 0;
}