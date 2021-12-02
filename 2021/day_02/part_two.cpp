#include "common.h"

int main(){
	String text = readFile("input.txt");
	
	char* position = text.data;

    int aim = 0;
    int h_pos = 0;
    int depth = 0;

    while((position - text.data) < text.size){
        char* command = position;
        while(!isDigit(*position)) position++;
        long int value = strtol(position, &position, 10);
        position++;

        if(strings_equal(command, "forward")){
            h_pos += value;
            depth += aim * value;
        }else if(strings_equal(command, "down")){
            aim += value;
        }else if(strings_equal(command, "up")){
            aim -= value;
        }
    }

    printf("Aim: %i\n", aim);
    printf("Horizontal position: %i\n", h_pos);
    printf("Depth: %i\n", depth);
    printf("Result: %i\n", h_pos * depth);
}