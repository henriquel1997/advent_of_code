#include "common.h"

struct Digit {
    int segmentCount;
    char* segments;
};

Digit readDigit(char** textPointer){
    char* digitStart = *textPointer;
    while(!isWhiteSpace(**textPointer) && !isEndOfLine(**textPointer)){
        (*textPointer)++;
    }
    int segmentCount = (*textPointer) - digitStart;
    return {segmentCount, digitStart};
}

int getNumberOfSegmentsInCommon(Digit digit1, Digit digit2){
    int segCount = 0;
    for(int j = 0; j < digit1.segmentCount; j++){
        for(int k = 0; k < digit2.segmentCount; k++){
            if(digit1.segments[j] == digit2.segments[k]){
                segCount++;
            }
        }
    }
    return segCount;
}

int main(){
	String text = readFile("input.txt");
	
	char* position = text.data;

    uint64 totalSum = 0;
    while((position - text.data) < text.size){
        Digit digits[10];

        int fiveSegmentDigitsCount = 0;
        Digit fiveSegmentDigits[3];
        int sixSegmentDigitsCount = 0;
        Digit sixSegmentDigits[3];

        //Find the unique digits, store the rest
        int uniqueDigitsCount = 0;
        while(*position != '|'){
            Digit digit = readDigit(&position);
            
            if(digit.segmentCount == 2){
                digits[1] = digit;
                uniqueDigitsCount++;
            }else if(digit.segmentCount == 4){
                digits[4] = digit;
                uniqueDigitsCount++;
            }else if(digit.segmentCount == 3){
                digits[7] = digit;
                uniqueDigitsCount++;
            }else if(digit.segmentCount == 7){
                digits[8] = digit;
                uniqueDigitsCount++;
            }else if(digit.segmentCount == 5){
                if(fiveSegmentDigitsCount == 3){
                    printf("Too many 5 segment digits\n");
                    return -1001;
                }
                fiveSegmentDigits[fiveSegmentDigitsCount++] = digit;
            }else if(digit.segmentCount == 6){
                if(sixSegmentDigitsCount == 3){
                    printf("Too many 6 segment digits\n");
                    return -1002;
                }
                sixSegmentDigits[sixSegmentDigitsCount++] = digit;
            }else{
                printf("Something went wrong\n");
                return -1000;
            }

            eatWhiteSpace(&position);
        }
        position++;

        //Look for 9 the only six segment digit that have all of 4's segments
        Digit four = digits[4];
        for(int i = 0; i < sixSegmentDigitsCount; i++){
            Digit digit = sixSegmentDigits[i];

            if(getNumberOfSegmentsInCommon(four, digit) == 4){
                //Found 9
                digits[9] = digit;
                //Remove it from the six segment array
                for(int j = i; j < sixSegmentDigitsCount - 1; j++){
                    sixSegmentDigits[j] = sixSegmentDigits[j + 1];
                }
                sixSegmentDigitsCount--;
                break;
            }
        }

        //Look for 0 and 6
        Digit one = digits[1];
        for(int i = 0; i < sixSegmentDigitsCount; i++){
            Digit digit = sixSegmentDigits[i];

            int segCount = getNumberOfSegmentsInCommon(one, digit);
            if(segCount == 1){
                //Found 6
                digits[6] = digit;
            }else if(segCount == 2){
                //Found 0
                digits[0] = digit;
            }
        }

        //Look for 3, the only six segment digit that have all of 1's segments
        for(int i = 0; i < fiveSegmentDigitsCount; i++){
            Digit digit = fiveSegmentDigits[i];

            if(getNumberOfSegmentsInCommon(one, digit) == 2){
                //Found 3
                digits[3] = digit;
                //Remove it from the five segment array
                for(int j = i; j < fiveSegmentDigitsCount - 1; j++){
                    fiveSegmentDigits[j] = fiveSegmentDigits[j + 1];
                }
                fiveSegmentDigitsCount--;
                break;
            }
        }

        //Look for 2 and 5
        for(int i = 0; i < fiveSegmentDigitsCount; i++){
            Digit digit = fiveSegmentDigits[i];

            int segCount = getNumberOfSegmentsInCommon(four, digit);
            if(segCount == 2){
                //Found 2
                digits[2] = digit;
            }else if(segCount == 3){
                //Found 5
                digits[5] = digit;
            }
        }

        //Found all digits, now decode the number
        uint64 number = 0;
        while(!isEndOfLine(*position)){
            eatWhiteSpace(&position);
            
            Digit currentDigit = readDigit(&position);
            for(int i = 0; i < 10; i++){
                Digit digit = digits[i];
                if(currentDigit.segmentCount == digit.segmentCount){
                    if(getNumberOfSegmentsInCommon(currentDigit, digit) == digit.segmentCount){
                        number = (number * 10) + i;
                        break;
                    }
                }
            }
        }
        position++;

        if(number == 0){
            printf("Error decoding number\n");
            return -1003;
        }

        printf("Number %llu\n", number);

        totalSum += number;
    }

    printf("Sum of all numbers %llu\n", totalSum);
}