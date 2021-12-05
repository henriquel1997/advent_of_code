#include "common.h"

struct Line {
	long int x1;
	long int y1;
	long int x2;
	long int y2;
	bool horizontal;
	bool vertical;
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

		line.horizontal = line.y1 == line.y2;
		line.vertical = line.x1 == line.x2;
		if(line.horizontal || line.vertical){
			if((line.horizontal && line.x1 > line.x2) || (line.y1 > line.y2)){
				auto aux_x = line.x1;
				auto aux_y = line.y1;
				line.x1 = line.x2;
				line.y1 = line.y2;
				line.x2 = aux_x;
				line.y2 = aux_y;
			}

			lines[line_count++] = line;

			if (line.x1 < min_x) min_x = line.x1;
			if (line.y1 < min_y) min_y = line.y1;
			if (line.x2 > max_x) max_x = line.x2;
			if (line.y2 > max_y) max_y = line.y2;
		}
	}

	uint64 width = max_x - min_x + 1;
	uint64 height = max_y - min_y + 1;
	auto space = (uint64*) calloc(width * height, sizeof(uint64));

	uint64 intersection_count = 0;
	for (uint64 i = 0; i < line_count; i++){
		Line line = lines[i];
		if (line.horizontal){
			long int y = line.y1;
			for (uint64 x = line.x1; x <= line.x2; x++){
				uint64 pos = ((y - min_y) * width) + (x - min_x);
				uint64 count = ++space[pos];
				if (count == 2) intersection_count++;
			}
		} else {
			long int x = line.x1;
			for (uint64 y = line.y1; y <= line.y2; y++){
				uint64 pos = ((y - min_y) * width) + (x - min_x);
				uint64 count = ++space[pos];
				if (count == 2) intersection_count++;
			}
		}
	}

	printf("Intersection count: %llu\n", intersection_count);
}