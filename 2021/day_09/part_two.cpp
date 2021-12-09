#include "common.h"

bool* visitedMap; 

#define getPos(x, y) (((y) * info.maxLineSize) + (x))

uint64 getBasinSize(uint64 x, uint64 y, int* heightArray, TextFileInfo info){
    uint64 pos = getPos(x, y);
    if(visitedMap[pos]) return 0;
    visitedMap[pos] = true;

    int height = heightArray[pos];
    uint64 size = 1;
    if(y > 0){
        int above = heightArray[getPos(x, y - 1)];
        if(above < 9 && height < above){
            size += getBasinSize(x, y - 1, heightArray, info);
        }
    }
    if(y < info.numberOfLines - 1){
        int below = heightArray[getPos(x, y + 1)];
        if(below < 9 && height < below){
            size += getBasinSize(x, y + 1, heightArray, info);
        }
    }
    if(x > 0){
        int left = heightArray[getPos(x - 1, y)];
        if(left < 9 && height < left){
            size += getBasinSize(x - 1, y, heightArray, info);
        }
    }
    if(x < info.maxLineSize - 1){
        int right = heightArray[getPos(x + 1, y)];
        if(right < 9 && height < right){
            size += getBasinSize(x + 1, y, heightArray, info);
        }
    }
    return size;
}

int main(){
	String text = readFile("input.txt");
    TextFileInfo info = getTextFileInfo(text);

    uint64 numHeights = 0;
    int* heights = (int*) malloc(info.numberOfLines * info.maxLineSize * sizeof(int));
    visitedMap = (bool*) calloc(info.numberOfLines * info.maxLineSize, sizeof(bool));
	
	char* position = text.data;

    while((position - text.data) < text.size){
        int number = *(position++) - 48;
        if(*position == '\n') position++;

        heights[numHeights++] = number;
    }

    #define basinArraySize 3
    uint64 biggestBasins[basinArraySize] = { 0 };
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
                uint64 basinSize = getBasinSize(x, y, heights, info);
                printf("Basin size: %llu\n", basinSize);
                for(int i = 0; i < basinArraySize; i++){
                    if(basinSize > biggestBasins[i]){
                        for(int j = basinArraySize - 1; j > i; j--){
                            biggestBasins[j] = biggestBasins[j - 1];
                        }
                        biggestBasins[i] = basinSize;
                        break;
                    }
                }
            }
        }
    }

    printf("\nFinal basins\n");
    uint64 result = 1;
    for(int i = 0; i < basinArraySize; i++){
        printf("Basin %i: %llu\n", i, biggestBasins[i]);
        result *= biggestBasins[i];
    }
    printf("Result: %llu\n", result);
    return 0;
}