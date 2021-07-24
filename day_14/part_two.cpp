#include "..\common.h"

#define NUM_MEM_SLOTS 80000ULL

int main(){
	String text = readFile("input.txt");
	
	char* position = text.data;

	uint64 usedSlots = 0;
	uint64* memSlots = (uint64*) malloc(sizeof(uint64) * NUM_MEM_SLOTS);
	uint64* memValues  = (uint64*) malloc(sizeof(uint64) * NUM_MEM_SLOTS);

	const char* maskStart = "mask = ";
	const char* memStart = "mem[";

	uint64 onesMask = 0;
	uint64 numXPos = 0;
	uint64 xPos[36];
	while((position - text.data) < text.size) {
		if(strings_equal(position, maskStart)){
			position += strlen(maskStart);

			onesMask = 0;
			numXPos = 0;

			for(int i = 35; i >= 0; i--){
				char c = *(position++);
				if(c == 'X'){
					xPos[numXPos++] = i;
					onesMask |= 1ULL << i;
				}else if(c == '1'){
					onesMask |= 1ULL << i;
				}
			}

			position++;
		}else if(strings_equal(position, memStart)){
			position += strlen(memStart);
			uint64 address = strtoull(position, &position, 10);
			position += strlen("] = ");
			uint64 value = strtoull(position, &position, 10);
			position++;

			//Setting the value at all addresses (minus the ones above)
			uint64 xMask = 68719476735ULL; //All 36 bits set
			uint64 numBits = 1ULL << numXPos; // 2 ^ numXPos
			for (uint64 bits = 0; bits < numBits; bits++) {
				for (uint64 index = 0; index < numXPos; index++) {
					//Getting the bit at index
					uint64 bit = (bits >> (index)) & 1ULL;
					//Setting that bit at the mask
					xMask = (xMask & ~(1ULL << xPos[index])) | (bit << xPos[index]);
				}

				//Using the masks in the address
				uint64 addressMaskedWithOnes = (address | onesMask);
				uint64 maskedAddress = addressMaskedWithOnes & xMask;
				
				uint64 slotIndex;
				bool foundSlot = false;
				for (uint64 x = 0; x < usedSlots; x++) {
					if(x > NUM_MEM_SLOTS){
						printf("Trying to access slot number %llu which is higher than the maximum of %llu\n", x, NUM_MEM_SLOTS);
						return -1;
					}
					if (memSlots[x] == maskedAddress) {
						foundSlot = true;
						slotIndex = x;
						break;
					}
				}
				
				if (!foundSlot) {
					slotIndex = usedSlots++;
					memSlots[slotIndex] = maskedAddress;
				}
				
				memValues[slotIndex] = value;
			}
		}
	}

	uint64 sum = 0;
	for(uint64 i = 0; i < usedSlots; i++){
		uint64 prev = sum;
		sum += memValues[i];
	}
	printf("Num slots: %llu\n", usedSlots);
	printf("Sum of all values: %llu\n", sum);
	return 0;
}