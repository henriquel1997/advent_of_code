#include "common.h"

int main(){
	String text = readFile("input.txt");
	
	char* position = text.data;

    int h_pos = 0;
    int depth = 0;

    while((position - text.data) < text.size){
        char* command = position;
        while(!isDigit(*position)) position++;
        long int value = strtol(position, &position, 10);
        position++;

        if(strings_equal(command, "forward")){
            h_pos += value;
        }else if(strings_equal(command, "down")){
            depth += value;
        }else if(strings_equal(command, "up")){
            depth -= value;
        }
    }

    printf("Horizontal position: %i\n", h_pos);
    printf("Depth: %i\n", depth);
    printf("Result: %i\n", h_pos * depth);
}