#include "common.h"

#define NUM_ROWS 5
#define NUM_COLS 5

struct Board {
	long int cells[NUM_ROWS][NUM_COLS];
};

int main(){
	String text = readFile("input.txt");
	TextFileInfo info = getTextFileInfo(text);
	
	char* position = text.data;
	
	uint64 number_of_boards = (info.numberOfLines - 1) / (NUM_ROWS + 1);
	auto boards = (Board*) malloc(number_of_boards * sizeof(Board));

	//Skips the first line and goes to the first board
	while(*position != '\n') position++;
	while(!isDigit(*position)) position++;

	printf("Reading boards\n");

	int row = 0;
	int column = 0;
	uint64 boardIndex = 0;
	while(true){
		long int number = strtol(position, &position, 10);
		printf("%ld ", number);
		boards[boardIndex].cells[row++][column] = number;
		
		if(row >= NUM_ROWS){
			row = 0;
			column++;
			printf("\n");
			while(*position != '\n') position++;
			if(column >= NUM_COLS){
				column = 0;
				boardIndex++;
				printf("\n");
				if(boardIndex < number_of_boards) 
					while(!isDigit(*position)) position++;
				else break;
			}
		}
	}

	printf("Checking numbers\n");

	position = text.data;
	while (isDigit(*position)){
		long int number = strtol(position, &position, 10);
		position++;
		printf("Number: %ld\n", number);
		for (int i = 0; i < number_of_boards; i++){
			Board* board = &boards[i];
			bool won = false;
			for (int row = 0; row < NUM_ROWS; row++){
				for (int column = 0; column < NUM_COLS; column++){
					if(board->cells[row][column] == number){
						board->cells[row][column] = -1;
						int row_count = 0;
						int col_count = 0;
						for(int j = 0; j < NUM_ROWS; j++){
							if(board->cells[row][j] < 0) row_count++;
							if(board->cells[j][column] < 0) col_count++;
						}

						if(row_count == NUM_ROWS || col_count == NUM_COLS){
							won = true;
							goto check_win;
						}
					}
				}
			}
			check_win: if(won){
				long int unmarked_sum = 0;
				for (int row = 0; row < NUM_ROWS; row++) {
					for (int column = 0; column < NUM_COLS; column++) {
						long int cell = board->cells[row][column];
						if (cell >= 0) unmarked_sum += cell;
					}
				}

				long int final_score = unmarked_sum * number;
				printf("Final score: %ld\n", final_score);
				return 0;
			}
		}
	}

	printf("No winners\n");
	return -1;
}