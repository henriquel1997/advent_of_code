#include "..\common.h"
#include <xmmintrin.h>

#define TILE_SIZE 10
#define CONTENT_LINE_SIZE (TILE_SIZE - 2)

#if (TILE_SIZE <= 16)
#define Side unsigned short int
#else
//The other cases could be treated here if necessary
#endif

#if (CONTENT_LINE_SIZE <= 8)
#define ContentLine unsigned char
#else
//The other cases could be treated here if necessary
#endif

//Every side is and array of bits
struct Tile{
	uint64 id;
	Side top;
	Side left;
	Side right;
	Side bottom;
	
	ContentLine content[CONTENT_LINE_SIZE];
	
	Tile* next;
	Tile* prev;
};

struct List {
	uint64 size;
	Tile* start;
	Tile* end;
};

void removeTileFromList(List* list, Tile* tile){
	if (tile->next) tile->next->prev = tile->prev;
	if (tile->prev)tile->prev->next = tile->next;
	if (list->start == tile) list->start = tile->next;
	if (list->end == tile) list->end = tile->prev;
	tile->next = 0;
	tile->prev = 0;
	list->size--;
}

void pushTile(List* list, Tile* tile){
	if (list->end){
		list->end->next = tile;
		tile->prev = list->end;
	} else {
		list->start = tile;
	}
	list->end = tile;
	
	tile->next = 0;
	
	list->size++;
}

Tile* popTile(List* list){
	Tile* tile = list->start;
	list->start = tile->next;
	if(list->start) list->start->prev = 0;
	
	tile->next = 0;
	tile->prev = 0;
	
	list->size--;
	return tile;
}

Side reverseSide(Side side){
	Side result = 0;
	for(uint64 i = 0; i < TILE_SIZE; i++){
		result |= ((side >> i) & ((Side)1)) << (TILE_SIZE - 1 - i);
	}
	return result;
}

Tile getTileFlipped(Tile* tile, int flip){
	Tile flipped = *tile;
	if(flip == 1){
		flipped.left = reverseSide(flipped.left);
		flipped.right = reverseSide(flipped.right);
		Side aux = flipped.bottom;
		flipped.bottom = flipped.top;
		flipped.top = aux;

	}else if(flip == 2){
		flipped.top = reverseSide(flipped.top);
		flipped.bottom = reverseSide(flipped.bottom);
		Side aux = flipped.left;
		flipped.left = flipped.right;
		flipped.right = aux;
	}else if(flip == 3){
		Side aux = reverseSide(flipped.top);
		flipped.top = reverseSide(flipped.bottom);
		flipped.bottom = aux;
		aux = reverseSide(flipped.left);
		flipped.left = reverseSide(flipped.right);
		flipped.right = aux;
	}

	if (flip == 1 || flip == 3){
		uint64 halfIndex = (CONTENT_LINE_SIZE / 2);
		for(uint64 i = 0; i < halfIndex; i++){
			ContentLine aux = flipped.content[i];
			flipped.content[i] = flipped.content[CONTENT_LINE_SIZE - 1 - i];
			flipped.content[CONTENT_LINE_SIZE - 1 - i] = aux;
		}
		//TODO: Treat case where CONTENT_LINE_SIZE is odd here if necessary
	}
	if (flip == 2 || flip == 3){
		for(uint64 n = 0; n < CONTENT_LINE_SIZE; n++){
			ContentLine line = 0;
			for(uint64 i = 0; i < CONTENT_LINE_SIZE; i++){
				line |= ((flipped.content[n] >> i) & ((ContentLine)1)) << (CONTENT_LINE_SIZE - 1 - i);
			}
			flipped.content[n] = line;
		}
	}

	return flipped;
}

Tile getTileFlippedDiagonally(Tile* tile){
	Tile flipped = *tile;

	Side aux = flipped.left;
	flipped.left = flipped.top;
	flipped.top = aux;
	aux = flipped.right;
	flipped.right = flipped.bottom;
	flipped.bottom = aux;

	ContentLine content[CONTENT_LINE_SIZE] = { 0 };
	for(uint64 n = 0; n < CONTENT_LINE_SIZE; n++){
		ContentLine line = 0;
		for(uint64 i = 0; i < CONTENT_LINE_SIZE; i++){
			line |= ((tile->content[i] >> (n)) & ((ContentLine)1)) << i;
		}
		content[n] = line;
	}

	for (uint64 n = 0; n < CONTENT_LINE_SIZE; n++) {
		flipped.content[n] = content[n];
	}

	return flipped;
}

Tile getTileRotated(Tile* tile, int rotation){
	Tile rotated = *tile;
	rotation = rotation % 4;
	for(int i = 0; i < rotation; i++){
		Side aux = rotated.right;
		rotated.right = rotated.top;
		Side aux2 = rotated.bottom;
		rotated.bottom = reverseSide(aux);
		rotated.top = reverseSide(rotated.left);
		rotated.left = aux2;

		ContentLine content[CONTENT_LINE_SIZE] = { 0 };
		for(uint64 n = 0; n < CONTENT_LINE_SIZE; n++){
			ContentLine line = 0;
			for(uint64 i = 0; i < CONTENT_LINE_SIZE; i++){
				ContentLine otherLine = rotated.content[CONTENT_LINE_SIZE - 1 - i];
				ContentLine bit = (otherLine >> (n)) & ((ContentLine)1);
				line |= bit << i;
			}
			content[n] = line;
		}

		for (uint64 n = 0; n < CONTENT_LINE_SIZE; n++) {
			rotated.content[n] = content[n];
		}
	}

	return rotated;
}

Tile getTileVariation(Tile* tile, int variation){
	Tile rotated = getTileRotated(tile, variation % 4);
	return getTileFlipped(&rotated, variation / 4);
}

void getImageFlipped(bool** image, bool** output, uint64 imageSize, int flip){
	if (flip == 1 || flip == 3){
		for (uint64 x = 0; x < imageSize; x++) {
			for (uint64 y = 0; y < imageSize; y++) {
				output[x][imageSize - 1 - y] = image[x][y];
			}
		}
	}
	if (flip == 2 || flip == 3){
		for (uint64 x = 0; x < imageSize; x++) {
			for (uint64 y = 0; y < imageSize; y++) {
				output[imageSize - 1 - x][y] = image[x][y];
			}
		}
	}
}

void getImageRotated(bool** image, bool** output, uint64 imageSize, int rotation){
	rotation = rotation % 4;
	for (int i = 0; i < rotation; i++) {
		for (uint64 x = 0; x < imageSize; x++) {
			for (uint64 y = 0; y < imageSize; y++) {
				output[y][imageSize - 1 - x] = image[x][y];
			}
		}
	}
}

void getImageVariation(bool** image, bool** aux, bool** output, uint64 imageSize, int variation){
	getImageRotated(image, aux, imageSize, variation % 4);
	getImageFlipped(aux, output, imageSize, variation / 4);
}

void reverseTileArray(Tile* array, uint64 size){
	uint64 halfIndex = size / 2;
	for(uint64 i = 0; i < halfIndex; i++){
		Tile aux = getTileFlipped(&array[i], 2);
		array[i] = getTileFlipped(&array[size - 1 - i], 2);
		array[size - 1 - i] = aux;
	}
	if(size % 2 == 1){
		array[halfIndex] = getTileFlipped(&array[halfIndex], 2);
	}
}

bool assembleBorder(List* corners, List* borders, Tile* borderArray, uint64 squareSize){
	borderArray[0] = *popTile(corners);
	uint64 numTiles = 1;
	for(uint64 n = 1; n < squareSize - 1; n++){
		bool found = false;
		for (Tile* tile = borders->start; tile; tile = tile->next){
			for (int i = 0; i <= 2; i += 2) {
				Tile flipped = getTileFlipped(tile, i);
				if (borderArray[n - 1].right == flipped.left){
					borderArray[n] = flipped;
					removeTileFromList(borders, tile);
					found = true;
					numTiles++;
					goto end;
				}
			}
		}
		end:
		if (!found){
			return false;
		}
	}
	if(numTiles < (squareSize - 1)){
		return false;
	}
	for (Tile* corner = corners->start; corner; corner = corner->next){
		Tile flipped = getTileFlipped(corner, 2);
		for (int i = 0; i < 2; i++) {
			if (i == 1) {
				flipped = getTileFlippedDiagonally(&flipped);
			}
			if (borderArray[squareSize - 2].right == flipped.left){
				borderArray[squareSize - 1] = flipped;
				removeTileFromList(corners, corner);
				return true;
			}
		}
	}
	return false;
}

inline uint64 squareRoot(uint64 value) {
	return (uint64)_mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss((float)value)));
}

int main(){
	String text = readFile("input.txt");
	
	char* position = text.data;
	
	//Get the number of tiles
	uint64 numTiles = 0;
	const char* tileStart = "Tile ";
	while ((position - text.data) < text.size){
		if(strings_equal(position, tileStart)) numTiles++;
		position++;
	}
	
	position = text.data;
	
	//Parse each tile
	uint64 tileCont = 0;
	Tile* tiles = (Tile*) malloc(sizeof(Tile) * numTiles);
	
	while((position - text.data) < text.size){
		while (!strings_equal(position, tileStart)) position++;
		position += strlen(tileStart);
		
		Tile tile = { 0 };
		tile.id = strtoull(position, &position, 10);
		while(*position != '#' && *position != '.') position++;
		
		for(uint64 height = 0; height < TILE_SIZE; height++){
			if(*position == '#') tile.left |= ((Side)1) << height;
			
			bool top =    height == 0;
			bool bottom = height == TILE_SIZE - 1;
			if(top || bottom){
				for(uint64 pos = 0; pos < TILE_SIZE; pos++){
					if (*(position++) == '#') {
						if (top) {
							tile.top |= ((Side)1) << pos;
						} else {
							tile.bottom |= ((Side)1) << pos;
						}
					}
				}
			} else {
				position++;
				ContentLine line = 0;
				for(uint64 pos = 0; pos < CONTENT_LINE_SIZE; pos++){
					if (*(position++) == '#') {
						line |= ((ContentLine)1) << pos;
					}
				}
				tile.content[height - 1] = line;
				position++;
			}
			
			if(*(position - 1) == '#') tile.right |= ((Side)1) << height;	
			while(*position == '\r' || *position == '\n') position++;
		}
		
		tiles[tileCont] = tile;
		if(tileCont > 0){
			tiles[tileCont - 1].next = &tiles[tileCont];
			tiles[tileCont].prev = &tiles[tileCont - 1];
		}
		tileCont++;
		
		while(*position == '\r' || *position == '\n') position++;
	}
	
	//Search for the corner and border tiles
	uint64 squareSize = squareRoot(numTiles);
	uint64 borderTilesMax = (squareSize - 2) * 4;

	List borderList = { 0 };
	List cornerList = { 0 };

	List tilesList;
	tilesList.size = numTiles;
	tilesList.start = tiles;
	tilesList.end = &tiles[numTiles - 1];

	for (uint64 n = 0; n < numTiles; n++) {
		Tile* tile = &tiles[n];
		int topFound = 0;
		int bottomFound = 0;
		int leftFound = 0;
		int rightFound = 0;
		
		for (uint64 m = 0; m < numTiles; m++) {
			if (n == m) continue;

			Tile other = tiles[m];
			for (int i = 0; i < 16; i++) {
				Tile otherFlipped = getTileVariation(&other, i);
				
				if(!leftFound && tile->left == otherFlipped.right){
					leftFound = 1;
				}
				if(!rightFound && tile->right == otherFlipped.left){
					rightFound = 1;
				}
				if(!topFound && tile->top == otherFlipped.bottom){
					topFound = 1;
				}
				if(!bottomFound && tile->bottom == otherFlipped.top){
					bottomFound = 1;
				}
				
				if((topFound + bottomFound + leftFound + rightFound) == 4){
					goto end;
				}
			}
		}
		
		end:
		int sum = (topFound + bottomFound + leftFound + rightFound);
		if(cornerList.size < 4 && sum == 2){ //Corners
			if(!bottomFound){
				*tile = getTileFlipped(tile, 1);
			}
			if(!rightFound){
				*tile = getTileFlipped(tile, 2);
			}
			removeTileFromList(&tilesList, tile);
			pushTile(&cornerList, tile);
		} else if (borderList.size < borderTilesMax && sum == 3){ //Borders
			if(!bottomFound){
				*tile = getTileFlipped(tile, 1);
			}else if(!leftFound){
				*tile = getTileRotated(tile, 1);
			}else if(!rightFound){
				*tile = getTileRotated(tile, 3);
			}
			removeTileFromList(&tilesList, tile);
			pushTile(&borderList, tile);
		}

		if((cornerList.size >= 4) && (borderList.size >= borderTilesMax)){
			break;
		}
	}
	
	if (cornerList.size == 0){
		printf("No corners found\n");
		return -1;
	} else if (cornerList.size != 4) {
		printf("Only %llu corners found\n", cornerList.size);
		return -2;
	} else if (borderList.size == 0){
		printf("No border tiles found\n");
		return -3;
	} else if (borderList.size < borderTilesMax){
		printf("Only %llu border tiles found\n", borderList.size);
		return -4;
	}

	uint64 numCornersFlipped = 0;
	Tile cornersFlipped[4];
	for (Tile* corner = cornerList.start; corner; corner = corner->next){
		if (numCornersFlipped >= 4){
			printf("Here's the problem, too many corners!\n");
			return -5;
		}

		cornersFlipped[numCornersFlipped] = getTileFlippedDiagonally(corner);
		if(numCornersFlipped > 0){
			cornersFlipped[numCornersFlipped].prev = &cornersFlipped[numCornersFlipped - 1];
			cornersFlipped[numCornersFlipped - 1].next = &cornersFlipped[numCornersFlipped];
		} else {
			cornersFlipped[numCornersFlipped].prev = 0;
		}
		cornersFlipped[numCornersFlipped].next = 0;
		numCornersFlipped++;
	}
	
	List cornersFlippedList;
	cornersFlippedList.size = 4;
	cornersFlippedList.start = &cornersFlipped[0];
	cornersFlippedList.end = &cornersFlipped[3];
	

	Tile* squareMemory = (Tile*) calloc(numTiles, sizeof(Tile));
	Tile** square = (Tile**) malloc(squareSize * sizeof(Tile*));
	for(uint64 i = 0; i < squareSize; i++){
		square[i] = &squareMemory[i * squareSize];
	}

	uint64 lastPos = squareSize - 1;

	//Assemble the borders
	if(!assembleBorder(&cornerList, &borderList, square[0], squareSize)){
		printf("Couldn't assemble top border\n");
		return -6;
	}

	if(!assembleBorder(&cornerList, &borderList, square[lastPos], squareSize)){
		printf("Couldn't assemble bottom border\n");
		return -7;
	}

	for(uint64 i = 0; i < squareSize; i++){
		square[lastPos][i] = getTileFlipped(&(square[lastPos][i]), 1);
	}

	Tile* verticalBorder1 = (Tile*) malloc(sizeof(Tile) * squareSize);
	if(!assembleBorder(&cornersFlippedList, &borderList, verticalBorder1, squareSize)){
		printf("Couldn't assemble the first vertical border\n");
		return -8;
	}
	
	Tile* verticalBorder2 = (Tile*) malloc(sizeof(Tile) * squareSize);
	if(!assembleBorder(&cornersFlippedList, &borderList, verticalBorder2, squareSize)){
		printf("Couldn't assemble the second vertical border\n");
		return -9;
	}

	if(square[0][lastPos].id == verticalBorder1[0].id){
		reverseTileArray(square[0], squareSize);
	}
	if(square[0][0].id != verticalBorder1[0].id){
		reverseTileArray(verticalBorder1, squareSize);
	}
	if(square[lastPos][lastPos].id == verticalBorder1[lastPos].id){
		reverseTileArray(square[lastPos], squareSize);
	}
	if(square[0][lastPos].id != verticalBorder2[0].id){
		reverseTileArray(verticalBorder2, squareSize);
	}

	for(uint64 i = 1; i < lastPos; i++){
		square[i][0] = getTileFlippedDiagonally(&(verticalBorder1[i]));
		square[i][lastPos] = getTileFlippedDiagonally(&(verticalBorder2[i]));
		square[i][lastPos] = getTileFlipped(&(square[i][lastPos]), 2);
	}

	for(uint64 x = 1; x < lastPos; x++){
		for(uint64 y = 1; y < lastPos; y++){
			bool found = false;
			for (Tile* tile = tilesList.start; tile; tile = tile->next){
				for (int i = 0; i < 16; i++) {
					Tile flipped = getTileVariation(tile, i);
					Tile* pointer = &flipped;

					if ((flipped.left == (square[y][x - 1].right)) && (flipped.top == (square[y - 1][x].bottom))){
						if(x + 1 == lastPos && flipped.right != square[y][lastPos].left){
							continue;
						}
						if(y + 1 == lastPos && flipped.bottom != square[lastPos][x].top){
							continue;
						}

						square[y][x] = flipped;
						found = true;
						removeTileFromList(&tilesList, tile);
						goto endSearch;
					}
				}
			}
			endSearch:
			if(!found){
				printf("No tile found for X: %llu / Y: %llu\n", x, y);
				return -10;
			}
		}
	}

	const uint64 seaMonsterWidth = 20;
	const uint64 seaMonsterHeight = 3;
	bool seaMonster [seaMonsterHeight][seaMonsterWidth] = { 0 };
	seaMonster[0][18] = true;
	seaMonster[1][0] = true;
	seaMonster[1][5] = true;
	seaMonster[1][6] = true;
	seaMonster[1][11] = true;
	seaMonster[1][12] = true;
	seaMonster[1][17] = true;
	seaMonster[1][18] = true;
	seaMonster[1][19] = true;
	seaMonster[2][1] = true;
	seaMonster[2][4] = true;
	seaMonster[2][7] = true;
	seaMonster[2][10] = true;
	seaMonster[2][13] = true;
	seaMonster[2][16] = true;
	
	uint64 imageSize = squareSize * CONTENT_LINE_SIZE;
	bool* imageMemory = (bool*) malloc(imageSize * imageSize * sizeof(bool));
	bool** image = (bool**) malloc(imageSize * sizeof(bool*));
	for(uint64 i = 0; i < imageSize; i++){
		image[i] = &imageMemory[i * imageSize];
	}

	uint64 waterCount = 0;
	for (uint64 y = 0; y < squareSize; y++) {
		for (uint64 line = 0; line < CONTENT_LINE_SIZE; line++) {
			for (uint64 x = 0; x < squareSize; x++) {
				Tile tile = square[y][x];

				for (uint64 i = 0; i < CONTENT_LINE_SIZE; i++){
					bool filled = (tile.content[line] >> i) & ((ContentLine)1);
					uint64 imageX = (x * CONTENT_LINE_SIZE) + i;
					uint64 imageY = (y * CONTENT_LINE_SIZE) + line;
					image[imageX][imageY] = filled;
					if (filled) waterCount++;
				}
			}
		}
	}

	bool* auxMemory = (bool*) malloc(imageSize * imageSize * sizeof(bool));
	bool** aux = (bool**) malloc(imageSize * sizeof(bool*));
	for(uint64 i = 0; i < imageSize; i++){
		aux[i] = &auxMemory[i * imageSize];
	}

	bool* outputMemory = (bool*) malloc(imageSize * imageSize * sizeof(bool));
	bool** output = (bool**) malloc(imageSize * sizeof(bool*));
	for(uint64 i = 0; i < imageSize; i++){
		output[i] = &outputMemory[i * imageSize];
	}

	printf("Total water count: %llu\n", waterCount);
	for(int i = 0; i < 16; i++){
		getImageVariation(image, aux, output, imageSize, i);
		bool** temp = image;
		image = output;
		output = temp;

		uint64 seaMonsterCount = 0;
		for(uint64 y = 0; y < (imageSize - seaMonsterHeight); y++){
			for(uint64 x = 0; x < (imageSize - seaMonsterWidth); x++){
				bool found = true;
				for(uint64 seaMonsterY = 0; seaMonsterY < seaMonsterHeight; seaMonsterY++){
					for(uint64 seaMonsterX = 0; seaMonsterX < seaMonsterWidth; seaMonsterX++){
						if(seaMonster[seaMonsterY][seaMonsterX] && !image[y + seaMonsterY][x + seaMonsterX]){
							found = false;
							goto endSeaMonsterSearch;
						}
					}
				}
				endSeaMonsterSearch:
				if (found){
					seaMonsterCount++;
				}
			}
		}

		if(seaMonsterCount > 0){
			printf("%llu sea monsters found\n", seaMonsterCount);
			printf("%llu waters free\n", waterCount - (15 * seaMonsterCount));
			return 0;
		}
	}

	printf("No sea monsters found\n");
	return -1000;
}