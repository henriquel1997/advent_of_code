#include <stdio.h>
#include <stdlib.h>

struct String {
    long size;
    char* data;
};

String readFile(const char* fileName){
    FILE *f = fopen(fileName, "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* string = (char*) malloc(fsize + 1);
    fread(string, 1, fsize, f);
    fclose(f);

    string[fsize] = 0;
    return (String){fsize, string};
}

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