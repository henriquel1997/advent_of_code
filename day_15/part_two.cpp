#include "..\common.h"

#define HASHMAP_SIZE 20000
#define MAX_ARRAY_SIZE 1000

#define numberType unsigned int

struct HashMapItem {
	uint64 index;
	numberType number;
};

struct HashMapStart {
	numberType arraySize;
	HashMapItem* array;
};

int main(){
	
	numberType startSequence [] = {0,5,4,1,10,14,7};
	uint64 numAppearences = 0;
	HashMapStart* indexHashmap = (HashMapStart*) calloc(HASHMAP_SIZE, sizeof(HashMapStart));
	numberType* lastAppearences = (numberType*) malloc(sizeof(numberType) * MAX_ARRAY_SIZE * HASHMAP_SIZE);
	
	numberType number;
	for(int age = 0; age < 30000000 - 1; age++){
		if (age < array_size(startSequence)){
			number = startSequence[age];
		}
		
		numberType distance = 0;
		HashMapStart* start = &indexHashmap[number % HASHMAP_SIZE];
		if(start->arraySize == 0){
			start->array = (HashMapItem*) malloc(sizeof(HashMapItem) *  HASHMAP_SIZE);
		} else {
			for(int i = 0; i < start->arraySize; i++){
				if(start->array[i].number == number){
					uint64 index = start->array[i].index;
					distance = age - lastAppearences[index];
					lastAppearences[index] = age;
					break;
				}
			}
		}
		
		if(distance == 0){
			if(numAppearences >= MAX_ARRAY_SIZE * HASHMAP_SIZE){
				printf("lastAppearences array overflow at index %llu\n", numAppearences);
				return -1;
			}
			if(start->arraySize >= HASHMAP_SIZE){
				printf("hashmap array overflow at index %u\n", start->arraySize);
				return -2;
			}
			start->array[start->arraySize].index = numAppearences;
			start->array[start->arraySize++].number = number;
			lastAppearences[numAppearences++] = age;
		}
		
		number = distance;
	}
	
	printf("2020th number: %u\n", number);
	return 0;
}