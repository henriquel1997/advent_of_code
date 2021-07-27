#include "..\common.h"

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

//Maybe use a lookup table if this is too slow
Side reverseSide(Side side){
	Side result = 0;
	for(uint64 i = 0; i < TILE_SIZE; i++){
		result |= ((side >> i) & ((Side)1)) << (TILE_SIZE - 1);
	}
	return result;
}

void removeTile(Tile tile){
	if(tile.next) tile.next->prev = tile.prev;
	if(tile.prev) tile.prev->next = tile.next;
}

void printTiles(Tile* tile){
	for (; tile; tile = tile->next){
		printf("Tile %llu:\n", tile->id);
		for (uint64 i = 0; i < TILE_SIZE; i++){
			bool filled = (tile->top >> i) & ((Side)1);
			if(filled) printf("#");
			else printf(".");
		}
		printf("\n");

		for (uint64 i = 1; i < TILE_SIZE - 1; i++){
			bool filled = (tile->left >> i) & ((Side)1);
			if(filled) printf("#");
			else printf(".");
			
			for(uint64 j = 0; j < (TILE_SIZE - 2); j++) printf(".");

			filled = (tile->right >> i) & ((Side)1);
			if(filled) printf("#");
			else printf(".");

			printf("\n");
		}

		for (uint64 i = 0; i < TILE_SIZE; i++){
			bool filled = (tile->bottom >> i) & ((Side)1);
			if(filled) printf("#");
			else printf(".");
		}
		printf("\n\n");
	}
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
	Tile** square = (Tile**) malloc(tileBytes);
	Tile* scratchTiles = (Tile*) malloc(tileBytes);
	memcpy(scratchTiles, tiles, tileBytes);

	
	return 0;
}