#include "..\common.h"
#include <xmmintrin.h>

#define TILE_SIZE 10

#if (TILE_SIZE <= 16)
#define Side unsigned short int
#else
//The other cases could be treated here if necessary
#define Side uint64
#endif

//Every side is and array of bits
struct Tile{
	uint64 id;
	Side top;
	Side left;
	Side right;
	Side bottom;
	Tile* next;
	Tile* prev;
};

struct List {
	uint64 size;
	Tile* start;
	Tile* end;
};

void pushTile(List* list, Tile* tile){
	list->end->next = tile;
	tile->prev = list->end;
	list->end = tile;

	list->size++;
}

Tile* popTile(List* list){
	Tile* tile = list->start;
	list->start = tile->next;
	list->start->prev = 0;
	tile->next = 0;

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
	return flipped;
}

bool searchTiles(List* list, uint64 x, uint64 y, Tile** square, uint64 squareSize){
	uint64 listSize = list->size;
	for(uint64 count = 0; count < listSize; count++){
		Tile* tile = popTile(list);
		for (int i = 0; i < 4; i++) {
			Tile flipped = getTileFlipped(tile, i);

			if (
				(x == 0 || square[x - 1][y].right == flipped.left) &&
				(y == 0 || square[x][y - 1].bottom == flipped.top)
			) {
				square[x][y] = flipped;

				if (x == (squareSize - 1) && y == (squareSize - 1)) {
					return true;
				}

				uint64 nextX = (x < (squareSize - 1)) ? (x + 1) : 0;
				uint64 nextY = (nextX > 0) ? y : (y + 1);
				if (searchTiles(list, nextX, nextY, square, squareSize)) {
					return true;
				}
			}
		}
		pushTile(list, tile);
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
	uint64 tileBytes = sizeof(Tile) * numTiles;
	Tile* tiles = (Tile*) malloc(tileBytes);

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
				position += TILE_SIZE;
			}

			if(*(position - 1) == '#') tile.right |= ((Side)1) << height;	
			while(*position == '\r' || *position == '\n') position++;
		}

		if(tileCont > 0)              tile.prev = &tiles[tileCont - 1];
		if((tileCont + 1) < numTiles) tile.next = &tiles[tileCont + 1];
		tiles[tileCont++] = tile;

		while(*position == '\r' || *position == '\n') position++;
	}

	//Search for a solution
	List list;
	list.size = numTiles;
	list.start = tiles;
	list.end = &tiles[numTiles - 1];

	uint64 squareSize = squareRoot(numTiles);
	Tile* squareMemory = (Tile*) malloc(tileBytes);
	Tile** square = (Tile**) malloc(squareSize * sizeof(Tile*));
	for(uint64 i = 0; i < squareSize; i++){
		square[i] = &squareMemory[i * squareSize];
	}

	if(searchTiles(&list, 0, 0, square, squareSize)){
		uint64 end = squareSize - 1;
		uint64 result = square[0][0].id * square[0][end].id * square[end][0].id * square[end][end].id;
		printf("Corners multiplied: %llu\n", result);
	} else {
		printf("No combinations found\n");
	}	
	return 0;
}