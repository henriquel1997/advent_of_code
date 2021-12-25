#include "common.h"

enum Direction {
	NONE = 0,
	EAST = 1,
	SOUTH = 2
};

#define get_pos(x, y) ((width * (y)) + (x))

bool equals(Direction* map1, Direction* map2, uint64 size){
	for (uint64 i = 0; i < size; i++){
		if (map1[i] != map2[i]){
			return false;
		}
	}
	return true;
}

void print_map(Direction* map, int64 width, int64 height){
	for (int64 y = 0; y < height; y++) {
		for (int64 x = 0; x < width; x++) {
			int64 pos = get_pos(x, y);
			if (map[pos] == EAST){
				printf(">");
			}else if (map[pos] == SOUTH){
				printf("v");
			} else {
				printf(".");
			}
		}
		printf("\n");
	}
	printf("\n");
}

int main(){
	String text = readFile("input.txt");
	TextFileInfo info = getTextFileInfo(text);

	int64 width = info.maxLineSize;
	int64 height = info.numberOfLines;

	uint64 map_size = width * height;
	Direction* map = (Direction*) calloc(map_size, sizeof(Direction));

	char* position = text.data;

	int64 x = 0;
	int64 y = 0;
	while ((position - text.data) < text.size) {
		int64 pos = get_pos(x, y);
		if (*position == '>'){
			map[pos] = EAST;
		}else if (*position == 'v'){
			map[pos] = SOUTH;
		}
		position++;
		if (isEndOfLine(*position)){
			x = 0;
			y++;
			while (isEndOfLine(*position)) position++;
		} else {
			x++;
		}
	}

	Direction* previous_map = (Direction*) malloc(map_size * sizeof(Direction));

	uint64 step = 0;

	//printf("Step %llu\n", step);
	//print_map(map, width, height);

	Direction* aux = (Direction*) malloc(map_size * sizeof(Direction));

	while (!equals(map, previous_map, map_size)){
		memcpy(previous_map, map, map_size * sizeof(Direction));

		step++;

		memcpy(aux, map, map_size * sizeof(Direction));
		for (int64 y = 0; y < height; y++){
			for (int64 x = 0; x < width; x++){
				int64 pos = get_pos(x, y);
				int64 target = get_pos((x + 1) % width, y);
				if (aux[pos] == EAST && aux[target] == NONE){
					map[pos] = NONE;
					map[target] = EAST;
				}
			}
		}

		/*printf("Step %llu moving east\n", step);
		print_map(map, width, height);*/

		memcpy(aux, map, map_size * sizeof(Direction));
		for (int64 x = 0; x < width; x++){
			for (int64 y = 0; y < height; y++){
				int64 pos = get_pos(x, y);
				int64 target = get_pos(x, (y + 1) % height);
				if (aux[pos] == SOUTH && aux[target] == NONE){
					map[pos] = NONE;
					map[target] = SOUTH;
				}
			}
		}

		//printf("Step %llu\n", step);
		//print_map(map, width, height);
	}

	printf("Number of steps: %llu\n", step);
	return 0;
}