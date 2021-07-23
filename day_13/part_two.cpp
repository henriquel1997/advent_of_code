#include "..\common.h"

uint64 greatestCommonDivisor(uint64 a, uint64 b){
	while(b != 0){
		uint64 aux = b;
		b = a % b;
		a = aux;
	}
	return a;
}

uint64 leastCommonMultiple(uint64 a, uint64 b){
	return (a * b) / greatestCommonDivisor(a, b);
}

int main(){
	String text = readFile("input.txt");
	
	char* position = text.data;

	while(*position != '\n')position++;
	position++;

	uint64 numIDs = 0;
	uint64 ids [1000] = {0};
	uint64 offsets [1000] = {0};

	uint64 cont = 0;
	while((position - text.data) < text.size){
		if(*position != 'x'){
			uint64 busID = strtoul(position, &position, 10);
			ids[numIDs] = busID;
			offsets[numIDs++] = cont;
			printf("%llu - %llu\n", cont, busID);
		} else {
			printf("x\n");
			position++;
		}
		position++;
		cont++;
	}


	uint64 time = 0;
	uint64 stepTime = ids[0];
	for(int i = 1; i < numIDs; i++){
		uint64 id = ids[i];
		while(((time + offsets[i]) % id) != 0){
			time += stepTime;
		}
		stepTime = leastCommonMultiple(id, stepTime);
	}
	printf("Time: %llu\n", time);
	return 0;
}