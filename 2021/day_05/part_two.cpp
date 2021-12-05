#include "common.h"

struct Line {
	long int x1;
	long int y1;
	long int x2;
	long int y2;
};

int main(){
	String text = readFile("input.txt");
	TextFileInfo info = getTextFileInfo(text);

	uint64 line_count = 0;
	auto lines = (Line*) malloc(info.numberOfLines * sizeof(Line));
	
	char* position = text.data;

	long int min_x = 1000000;
	long int min_y = 1000000;
	long int max_x = 0;
	long int max_y = 0;
	while ((position - text.data) < text.size){
		Line line;
		line.x1 = strtol(position, &position, 10);
		position++;
		line.y1 = strtol(position, &position, 10);
		while (!isDigit(*position)) position++;
		
		line.x2 = strtol(position, &position, 10);
		position++;
		line.y2 = strtol(position, &position, 10);
		while (!isDigit(*position)) position++;

		//if ((line.x1 != line.x2) && (line.y1 != line.y2)) continue;
		
		lines[line_count++] = line;
		
		min_x = min(min_x, min(line.x1, line.x2));
		min_y = min(min_y, min(line.y1, line.y2));
		max_x = max(max_x, max(line.x1, line.x2));
		max_y = max(max_y, max(line.y1, line.y2));
	}

	uint64 width = max_x - min_x + 1;
	uint64 height = max_y - min_y + 1;
	auto space = (uint64*) calloc(width * height, sizeof(uint64));

	uint64 intersection_count = 0;
	for (uint64 i = 0; i < line_count; i++){
		Line line = lines[i];

		int64 dir_x = line.x2 - line.x1;
		if (dir_x != 0) dir_x = sign(dir_x);
		int64 dir_y = line.y2 - line.y1;
		if (dir_y != 0) dir_y = sign(dir_y);
		uint64 x = line.x1;
		uint64 y = line.y1;

		do{
			uint64 pos = ((y - min_y) * width) + (x - min_x);
			uint64 count = ++space[pos];
			if (count == 2) intersection_count++;
			x += dir_x;
			y += dir_y;
		} while ((x != (line.x2 + dir_x)) || (y != (line.y2 + dir_y)));
	}

	/*for (uint64 i = 0; i < width * height; i++){
		if (i % width == 0) printf("\n");
		if (space[i] == 0){
			printf(".");
		} else {
			printf("%llu", space[i]);
		}
	}*/

	printf("\nIntersection count: %llu\n", intersection_count);
}