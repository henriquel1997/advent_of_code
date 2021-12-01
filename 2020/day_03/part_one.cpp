#include "..\common.h"

int main(){
    String text = readFile("input.txt");

    int lineLength = 0;
    for(char* c = text.data; *c != '\n'; c++){
        lineLength++;
    }

    int x, y, treeCount = 0;
    while(true){
        x = (x + 3) % lineLength;
        y++;

        int textPos = (lineLength * y) + x + y; //The last y is the number of "\n"'s
        if(textPos >= text.size){
            break;
        }

        if(text.data[textPos] == '#'){
            treeCount++;
            text.data[textPos] = 'X';
        }else{
            text.data[textPos] = 'O';
        }
    };

    printf("%s\n", text.data);
    printf("\nNumber of trees encountered: %i\n", treeCount);
    return 0;
}