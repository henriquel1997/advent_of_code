#include "common.h"

int main(){
	String text = readFile("input.txt");
    TextFileInfo info = getTextFileInfo(text);
    uint64 num_windows = info.numberOfLines - 2;
    int* windows = (int*)malloc(num_windows * sizeof(int));
	
	char* position = text.data;

    uint64 index = 0;
	while((position - text.data) < text.size){
        long int number = strtol (position, &position, 10);
        position++;
        printf("Number: %ld\n", number);

        windows[index] += number;
        if(index + 1 < num_windows) windows[index + 1] += number;
        if(index + 2 < num_windows) windows[index + 2] += number;
        index++;
    }

    uint64 increase_count = 0;
    printf("Window 0: %ld\n", windows[0]);
    for(uint64 i = 1; i < num_windows; i++){
        printf("Window %i: %ld\n", i, windows[i]);
        if(windows[i] > windows[i - 1]) increase_count++;
    }

    printf("Increase count: %i\n", increase_count);
}