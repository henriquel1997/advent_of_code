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

void removeTile(Tile* tile){
	if (tile->next) tile->next->prev = tile->prev;
	if (tile->prev)tile->prev->next = tile->next;
	tile->next = 0;
	tile->prev = 0;
}

void removeTile(List* list, Tile* tile){
	if (tile->next) tile->next->prev = tile->prev;
	if (tile->prev)tile->prev->next = tile->next;
	if (list->start == tile) list->start = tile->next;
	if (list->end == tile) list->end = tile->prev;
	tile->next = 0;
	tile->prev = 0;
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
	return flipped;
}

Tile getTileRotated(Tile* tile, int rotation){
	Tile rotated = *tile;
	rotation = rotation % 4;
	for(int i = 1; i < rotation; i++){
		Side aux = rotated.right;
		rotated.right = rotated.top;
		Side aux2 = rotated.bottom;
		rotated.bottom = reverseSide(aux);
		rotated.top = reverseSide(rotated.left);
		rotated.left = aux2;
	}
	return rotated;
}

Tile getTileVariation(Tile* tile, int variation){
	Tile rotated = getTileRotated(tile, variation % 4);
	return getTileFlipped(&rotated, variation / 4);
}

bool searchTiles(List* list, uint64 x, uint64 y, Tile** square, uint64 squareSize){
	uint64 listSize = list->size;
	Tile firstTile = *list->start;
	for(uint64 count = 0; count < listSize; count++){
		Tile* tile = popTile(list);
		if(count > 0 && tile->id == firstTile.id){
			printf("Loop\n");
			return false;
		}
		for (int i = 0; i < 16; i++) {
			Tile flipped = getTileVariation(tile, i);

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

bool assembleBorder(List* corners, List* borders, Tile* borderArray, uint64 squareSize){
	borderArray[0] = *popTile(corners);
	uint64 numTiles = 1;
	for(uint64 n = 1; n < squareSize - 1; n++){
		bool found = false;
		for (Tile* tile = borders->start; tile; tile = tile->next){
			for (int i = 0; i < 2; i++) {
				Tile flipped = getTileFlipped(tile, i);
				if (borderArray[n - 1].right == flipped.left){
					borderArray[n] = flipped;
					removeTile(borders, tile);
					found = true;
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
		for (int i = 0; i < 16; i++) {
			Tile flipped = getTileVariation(corner, i);
			if (borderArray[squareSize - 2].right == flipped.left){
				borderArray[squareSize - 1] = flipped;
				removeTile(corners, corner);
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
			removeTile(tile);
			pushTile(&cornerList, tile);
		} else if (borderList.size < borderTilesMax && sum == 3){ //Borders
			if(!bottomFound){
				*tile = getTileFlipped(tile, 2);
			}else if(!leftFound){
				*tile = getTileRotated(tile, 1);
			}else if(!rightFound){
				*tile = getTileRotated(tile, 3);
			}
			removeTile(tile);
			pushTile(&borderList, tile);
		}

		if((cornerList.size >= 4) && (borderList.size >= borderTilesMax)){
			break;
		}
	}
	
	if (cornerList.size == 0){
		printf("No corners found\n");
		return -1;
	} else if (cornerList.size < 4) {
		printf("Only %llu corners found\n", cornerList.size);
		return -2;
	} else if (borderList.size == 0){
		printf("No border tiles found\n");
		return -3;
	} else if (borderList.size < borderTilesMax){
		printf("Only %llu border tiles found\n", borderList.size);
		return -4;
	}

	Tile* squareMemory = (Tile*) malloc(tileBytes);
	Tile** square = (Tile**) malloc(squareSize * sizeof(Tile*));
	for(uint64 i = 0; i < squareSize; i++){
		square[i] = &squareMemory[i * squareSize];
	}

	//Assemble the border
	if(!assembleBorder(&cornerList, &borderList, square[0], squareSize)){
		printf("Couldn't assemble top border\n");
		return -5;
	}

	if(!assembleBorder(&cornerList, &borderList, square[squareSize - 1], squareSize)){
		printf("Couldn't assemble bottom border\n");
		return -6;
	}

	/*//Search for a solution
	List list;
	list.size = numTiles;
	list.start = tiles;
	list.end = &tiles[numTiles - 1];

	if(searchTiles(&list, 0, 0, square, squareSize)){
		uint64 end = squareSize - 1;
		uint64 result = square[0][0].id * square[0][end].id * square[end][0].id * square[end][end].id;
		printf("Corners multiplied: %llu\n", result);
	} else {
		printf("No combinations found\n");
	}*/
	return 0;
}