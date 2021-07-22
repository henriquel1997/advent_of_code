#include "..\common.h"

int main(){
    String text = readFile("input.txt");

    int lineLength = 0;
    for(char* c = text.data; *c != '\n'; c++){
        lineLength++;
    }

    printf("Line length: %i\n", lineLength);

    int right_array[] = {1, 3, 5, 7, 1};
    int down_array[]  = {1, 1, 1, 1, 2};

    long multipliedTotal = 1;
    for(int i = 0; i < 5; i++){
        int x = 0;
        int y = 0;
        long treeCount = 0;
        while(true){
            x = (x + right_array[i]) % lineLength;
            y += down_array[i];
            int textPos = (lineLength * y) + x + y; //The last y is the number of "\n"'s
            if(textPos >= text.size){
                break;
            }

            if(text.data[textPos] == '#'){
                treeCount++;
            }
        };
        printf("%i - %ld\n", i, treeCount);
        multipliedTotal *= treeCount;
    }

    printf("Multiplied total: %ld\n", multipliedTotal);
    return 0;
}