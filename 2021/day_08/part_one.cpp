#include "common.h"

int main(){
	String text = readFile("input.txt");
	
	char* position = text.data;

    uint64 uniqueDigitsCount = 0;
    while((position - text.data) < text.size){
        while(*position != '|') position++;

        while(!isEndOfLine(*position)){
            eatWhiteSpace(&position);
            int segmentsCount = 0;
            for(;!isWhiteSpace(*position) && !isEndOfLine(*position); position++) 
                segmentsCount++;
            if(segmentsCount == 2 || segmentsCount == 4 || segmentsCount == 3 || segmentsCount == 7){
                uniqueDigitsCount++;
            }
        }
        position++;
    }

    printf("Unique digits %llu\n", uniqueDigitsCount);
}