#include "common.h"

#define GRID_SIZE 10

int main(){
	String text = readFile("input.txt");
	
	char* position = text.data;

	int grid[GRID_SIZE][GRID_SIZE];

	int x = 0;
	int y = 0;
	while((position - text.data) < text.size){
		int number = *(position++) - 48;
		grid[x++][y] = number;
		printf("|%i|", number);
		if (*position == '\n'){
			printf("\n");
			position++;
			x = 0;
			y++;
		}
	}

	for (uint64 step = 0; true; step++){
		for (int64 y = 0; y < GRID_SIZE; y++) {
			for (int64 x = 0; x < GRID_SIZE; x++) {			
				grid[x][y]++;
			}
		}

		bool flashed;
		do {
			flashed = false;
			for (int64 x = 0; x < GRID_SIZE; x++) {
				for (int64 y = 0; y < GRID_SIZE; y++) {
					if (grid[x][y] > 9) {
						grid[x][y] = 0;
						flashed = true;
						for (int64 dx = -1; dx < 2; dx++) {
							for (int64 dy = -1; dy < 2; dy++) {
								int64 other_x = x + dx;
								int64 other_y = y + dy;
								if (
									other_x >= 0 && other_y >= 0 &&
									other_x < GRID_SIZE && other_y < GRID_SIZE &&
									grid[other_x][other_y] > 0
								)
									grid[other_x][other_y]++;
							}
						}
					}
				}
			}
		} while(flashed);

		bool all_zeroes = true;
		printf("\nAfter step %lli", step + 1);
		for (int64 y = 0; y < GRID_SIZE; y++) {
			for (int64 x = 0; x < GRID_SIZE; x++) {
				if (x % GRID_SIZE == 0) printf("\n");
				printf("|%i|", grid[x][y]);
				if(grid[x][y] > 0) all_zeroes = false;
			}
		}
		
		if(all_zeroes) return 0;
	}
}