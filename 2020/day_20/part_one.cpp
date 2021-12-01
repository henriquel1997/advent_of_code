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
};

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
	for(int i = 0; i < rotation; i++){
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

		tiles[tileCont++] = tile;

		while(*position == '\r' || *position == '\n') position++;
	}

	//Search for the corners
	int cornersCount = 0;
	Tile corners[4];
	for (uint64 n = 0; n < numTiles; n++) {
		Tile tile = tiles[n];
		int topFound = 0;
		int bottomFound = 0;
		int leftFound = 0;
		int rightFound = 0;
		
		bool found = false;
		for (uint64 m = 0; m < numTiles; m++) {
			if (n != m) {
				Tile other = tiles[m];
				for (int i = 0; i < 16; i++) {
					Tile otherFlipped = getTileVariation(&other, i);
					if(!leftFound && tile.left == otherFlipped.right){
						leftFound = 1;
					}
					if(!rightFound && tile.right == otherFlipped.left){
						rightFound = 1;
					}
					if(!topFound && tile.top == otherFlipped.bottom){
						topFound = 1;
					}
					if(!bottomFound && tile.bottom == otherFlipped.top){
						bottomFound = 1;
					}
					
					if((topFound + bottomFound + leftFound + rightFound) > 2){
						found = true;
						goto end;
					}
				}
			}
		}
		end:
		if(!found){
			corners[cornersCount++] = tile;
			if(cornersCount >= 4){
				break;
			}
		}
	}

	if(cornersCount >= 4){
		uint64 result = corners[0].id * corners[1].id * corners[2].id * corners[3].id;
		printf("Corners multiplied: %llu\n", result);
	} else if(cornersCount > 0){
		printf("Only %i corners found\n", cornersCount);
	} else {
		printf("No corners found\n");
	}
	return 0;
}