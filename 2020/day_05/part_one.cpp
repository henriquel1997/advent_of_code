#include "..\common.h"

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

        printf("%c - Row:%i - %i\n", c, row, row + rowSpaceSize - 1);
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

        printf("%c - Col:%i - %i\n", c, col, col + colSpaceSize - 1);
    }

    (*position)++; //Skips '\n'

    int id = (row * 8) + col;

    printf("Row: %i / Col: %i / ID: %i\n", row, col, id);

    return id;
}

int main(){
    String text = readFile("input.txt");

    char* position = text.data;

    int highestID = -1;
    
    while((position - text.data) < text.size){
        int id = getSeatID(&position);
        if(id > highestID){
            highestID = id;
        }
    }

    printf("Highest ID: %i\n", highestID);
    return 0;
}