#include "common.h"

int main(){
	String text = readFile("input.txt");
    TextFileInfo info = getTextFileInfo(text);

    uint64 numHeights = 0;
    int* heights = (int*) malloc(info.numberOfLines * info.maxLineSize * sizeof(int));
	
	char* position = text.data;

    while((position - text.data) < text.size){
        int number = *(position++) - 48;
        if(*position == '\n') position++;

        heights[numHeights++] = number;
    }

    #define getPos(x, y) (((y) * info.maxLineSize) + (x))

    int lowestHeightsSum = 0;
    for(uint64 y = 0; y < info.numberOfLines; y++){
        for(uint64 x = 0; x < info.maxLineSize; x++){
            int height = heights[getPos(x, y)];
            bool isLowest = true;
            if(y > 0){
                isLowest &= height < heights[getPos(x, y - 1)];
            }
            if(y < info.numberOfLines - 1){
                isLowest &= height < heights[getPos(x, y + 1)];
            }
            if(x > 0){
                isLowest &= height < heights[getPos(x - 1, y)];
            }
            if(x < info.maxLineSize - 1){
                isLowest &= height < heights[getPos(x + 1, y)];
            }

            if(isLowest){
                lowestHeightsSum += height + 1;
            }
        }
    }

    printf("Sum: %i\n", lowestHeightsSum);
    return 0;
}