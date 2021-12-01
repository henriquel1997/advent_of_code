#include "common.h"

int main(){
	String text = readFile("input.txt");
	
	char* position = text.data;

    int increase_count = 0;
    long int prev_number = strtol (position, &position, 10);
    position++;
    printf("Number: %ld\n", prev_number);

	while((position - text.data) < text.size){
        long int number = strtol (position, &position, 10);
        position++;
        printf("Number: %ld / pos: %ld / text.size: %ld\n", number, position - text.data, text.size);
        if(number > prev_number) increase_count++;
        prev_number = number;
    }

    printf("Increase count: %i\n", increase_count);
}