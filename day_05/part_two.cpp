#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

int getSeatID(char** position){
    int numRows = 128;
    int rowSpaceSize = numRows;
    int row = 0;
    for(int i = 0; i < 7; i++){
        char c = *((*position)++);
        rowSpaceSize /= 2;
        if(c == 'B'){
            row += rowSpaceSize;
        }
    }

    int numCols = 8;
    int colSpaceSize = numCols;
    int col = 0;
    for(int i = 0; i < 3; i++){
        char c = *((*position)++);
        colSpaceSize /= 2;
        if(c == 'R'){
            col += colSpaceSize;
        }
    }

    (*position)++; //Skips '\n'

    int id = (row * 8) + col;

    printf("Row: %i / Col: %i / ID: %i\n", row, col, id);

    return id;
}

int main(){
    String text = readFile("input.txt");

    char* position = text.data;

    int numIDs = 0;
    int ids [128 * 8];
    
    while((position - text.data) < text.size){
        ids[numIDs++] = getSeatID(&position);
    }

    for(int i = 0; i < numIDs; i++){
        int id = ids[i];
        bool foundIDFront = false;
        bool foundIDBack = false;

        for(int j = 0; j < numIDs; j++){
            if(i != j){
                int other = ids[j];

                if(!foundIDFront && other == id + 1){
                    foundIDFront = true;
                }else if(!foundIDBack && other == id - 1){
                    foundIDBack = true;
                }

                if(foundIDFront && foundIDBack){
                    break;
                }
            }
        }

        if(!foundIDFront){
            printf("Your seat id is: %i\n", id + 1);
            break;
        }else if(!foundIDBack){
            printf("Your seat id is: %i\n", id - 1);
            break;
        }
    }
    return 0;
}