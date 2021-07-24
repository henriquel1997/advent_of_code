#include "..\common.h"

void printBits(size_t const size, void const * const ptr){
	unsigned char *b = (unsigned char*) ptr;
	unsigned char byte;
	int i, j;
	
	for (i = size-1; i >= 0; i--) {
		for (j = 7; j >= 0; j--) {
			byte = (b[i] >> j) & 1;
			printf("%u", byte);
		}
	}
	puts("");
}

int main(){
	String text = readFile("input.txt");
	
	char* position = text.data;

	uint64 usedSlots = 0;
	uint64 memSlots[10000];
	uint64 memValues[10000];

	const char* maskStart = "mask = ";
	const char* memStart = "mem[";

	uint64 xMask = 0;
	uint64 onesMask = 0;
	while((position - text.data) < text.size) {
		if(strings_equal(position, maskStart)){
			position += strlen(maskStart);

			xMask = 0;
			onesMask = 0;

			for(int i = 35; i >= 0; i--){
				char c = *(position++);
				if(c == 'X'){
					xMask |= 1ULL << i;
				}else if(c == '1'){
					onesMask |= 1ULL << i;
					xMask |= 1ULL << i;
				}
			}

			position++;
		}else if(strings_equal(position, memStart)){
			position += strlen(memStart);
			uint64 pos = strtoull(position, &position, 10);
			position += strlen("] = ");
			uint64 value = strtoull(position, &position, 10);
			position++;

			uint64 xMaskValue = (value & xMask);
			uint64 maskedValue = xMaskValue | onesMask;

			uint64 slotIndex;
			bool foundSlot = false;
			for(uint64 i = 0; i < usedSlots; i++){
				if(memSlots[i] == pos){
					foundSlot = true;
					slotIndex = i;
					break;
				}
			}

			if(!foundSlot){
				slotIndex = usedSlots++;
				memSlots[slotIndex] = pos;
			}

			memValues[slotIndex] = maskedValue;
		}
	}

	uint64 sum = 0;
	for(uint64 i = 0; i < usedSlots; i++){
		sum += memValues[i];
	}
	printf("Sum of all values: %llu\n", sum);
	return 0;
}