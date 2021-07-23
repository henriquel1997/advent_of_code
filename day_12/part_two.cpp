#include "..\common.h"

#define abs(x) ((x) >= 0 ? (x) : -(x))
#define sign(x) ((x) >= 0 ? (1) : -1)

struct Position {
	long x;
	long y;
};

Position rotateAroundOrigin(Position position, long degrees) {
	long sin;
	long cos;
	switch(abs(degrees)){
		case 0: {
			sin = 0;
			cos = 1;
			break;
		}
		case 90: {
			sin = 1 * sign(degrees);
			cos = 0;
			break;
		}
		case 180: {
			sin = 0;
			cos = -1;
			break;
		}
		case 270: {
			sin = -1 * sign(degrees);
			cos = 0;
			break;
		}
	}
	//p'x = cos(theta) * (px-ox) - sin(theta) * (py-oy) + ox
	//p'y = sin(theta) * (px-ox) + cos(theta) * (py-oy) + oy
	long x = (cos * position.x) - (sin * position.y);
	long y = (sin * position.x) + (cos * position.y);
	return {x, y};
}

int main(){
	String text = readFile("input.txt");

	char* textPos = text.data;

	Position waypoint = {10, 1};
	Position position = {0, 0};

	while((textPos - text.data) < text.size){
		char c = *(textPos++);
		long value = strtol(textPos, &textPos, 10);
		textPos++;
		
		if(c == 'N'){
			waypoint.y += value; ;
		}else if(c == 'S'){
			waypoint.y -= value;
		}else if(c == 'E'){
			waypoint.x += value; ;
		}else if(c == 'W'){
			waypoint.x -= value;
		}else if(c == 'L'){
			waypoint = rotateAroundOrigin(waypoint, value);
		}else if(c == 'R'){
			waypoint = rotateAroundOrigin(waypoint, -value);
		}else if(c == 'F'){
			position.x += waypoint.x * value;
			position.y += waypoint.y * value;
		}

		printf("%c%ld - PosX: %ld / PosY: %ld / WaypointX: %ld / WaypointY: %ld\n", c, value, position.x, position.y, waypoint.x, waypoint.y);
	}

	printf("Manhattan distance: %ld\n", abs(position.x) + abs(position.y));

	return 0;
}