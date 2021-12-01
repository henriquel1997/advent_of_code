#include "..\common.h"

int main(){

	uint64 startSequence [] = {0,5,4,1,10,14,7};
	uint64 numAppearences = 0;
	uint64 lastAppearenceIndex[20000] = { 0 };
	uint64 lastAppearences[20000] = { 0 };
	
	uint64 number;
	for(int age = 0; age < 2020 - 1; age++){
		if (age < array_size(startSequence)){
			number = startSequence[age];
		} 

		printf("%llu\n", number);
		

		uint64 distance = 0;
		for(int i = 0; i < numAppearences; i++){
			if(lastAppearenceIndex[i] == number){
				distance = age - lastAppearences[i];
				lastAppearences[i] = age;
				break;
			}
		}

		if(distance == 0){
			lastAppearenceIndex[numAppearences] = number;
			lastAppearences[numAppearences++] = age;
		}

		number = distance;
	}

	printf("2020th number: %llu\n", number);
	return 0;
}