#include "..\common.h"

#define abs(x) ((x) >= 0 ? (x) : -(x))

int main(){
	String text = readFile("input.txt");

	char* position = text.data;

	long rotation = 0; //Starts pointing at east
	long posX = 0;
	long posY = 0;

	while((position - text.data) < text.size){
		char c = *(position++);
		long value = strtol(position, &position, 10);
		position++;
		
		if(c == 'N'){
			posY += value; ;
		}else if(c == 'S'){
			posY -= value;
		}else if(c == 'E'){
			posX += value; ;
		}else if(c == 'W'){
			posX -= value;
		}else if(c == 'L'){
			rotation = (rotation + value) % 360;
		}else if(c == 'R'){
			rotation = (rotation - value) % 360;
			if (rotation < 0){
				rotation += 360;
			}
		}else if(c == 'F'){
			int dirX;
			int dirY;
			switch (rotation){
				case 0: {
					dirX = 1;
					dirY = 0;
					break;
				}
				case 90:{
					dirX = 0;
					dirY = 1;
					break;
				}
				case 180:{
					dirX = -1;
					dirY = 0;
					break;
				}
				default:
				case 270:{
					dirX = 0;
					dirY = -1;
					break;
				}
			}
			posX += dirX * value;
			posY += dirY * value;
		}

		printf("%c%ld - PosX: %ld / PosY: %ld / Rotation: %ld\n", c, value, posX, posY, rotation);
	}

	printf("Manhattan distance: %ld\n", abs(posX) + abs(posY));

	return 0;
}