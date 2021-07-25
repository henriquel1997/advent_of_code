#include "..\common.h"

struct Cube{
	int64 x;
	int64 y;
	int64 z;
	int64 w;
	uint64 activeNeighbors;
};

bool isSameCube(Cube cube1, Cube cube2){
	return cube1.x == cube2.x && cube1.y == cube2.y && cube1.z == cube2.z && cube1.w == cube2.w;
}

bool areNeighboors(Cube cube1, Cube cube2){
	return (abs(cube1.x - cube2.x) <= 1) && 
		   (abs(cube1.y - cube2.y) <= 1) && 
		   (abs(cube1.z - cube2.z) <= 1) &&
		   (abs(cube1.w - cube2.w) <= 1); 
}

struct ListNode{
	uint64 id;
	ListNode* next;
	ListNode* prev;
	Cube cube;
};

struct List{
	uint64 size;
	uint64 idCounter;
	ListNode* start;
	ListNode* end;

	ListNode* availableNodes;
	
	uint64 memorySize;
	ListNode* memory;
};

List createList(uint64 maxNodes){
	List list = { 0 };
	list.memorySize = maxNodes;
	list.memory = (ListNode*) malloc(sizeof(ListNode) * maxNodes);
	return list;
}

void copyList(List* to, List* from){
	to->size = 0;
	to->idCounter = from->idCounter;

	ListNode* prev = 0;
	for (ListNode* node = from->start; node; node = node->next){
		ListNode copy = { 0 };
		copy.id = node->id;
		copy.cube = node->cube;
		copy.prev = prev;
		to->memory[to->size] = copy;
		
		if(prev) prev->next = &to->memory[to->size];
		prev = &to->memory[to->size];
		to->size++;
	}

	if (to->size > 0){
		to->start = &to->memory[0];
		to->end = prev;
	}
}

bool addCube(List* list, Cube cube){
	ListNode* node;
	if(list->availableNodes){
		node = list->availableNodes;
		list->availableNodes = node->next;
	}else{
		if(list->size >= list->memorySize){
			printf("Not enough memory to add a new node (%llu)\n", list->size);
			return false;
		}
		node = &list->memory[list->size];
	}

	node->cube = cube;
	node->id = list->idCounter++;
	node->next = 0;

	if(list->end){
		list->end->next = node;
		node->prev = list->end;
		list->end = node;
	}else{
		list->start = node;
		list->end = node;
	}

	list->size++;

	return true;
}

void removeNode(List* list, ListNode* node){
	if(list->start == list->end){
		list->start = 0;
		list->end = 0;
	} else if (node == list->start) {
		list->start = node->next;
		node->next->prev = 0;
	} else if (node == list->end){
		list->end = node->prev;
		node->prev->next = 0;
	} else {
		node->prev->next = node->next;
		node->next->prev = node->prev;
	}

	node->next = list->availableNodes;
	list->availableNodes = node;

	list->size--;
}

void removeNode(List* list, uint64 id){
	if(list->end && list->end->id == id){
		removeNode(list, list->end);
		return;
	}
	ListNode* node = list->start;
	while (node && node->id != id) node = node->next;
	if(node) removeNode(list, node);
}

void clearList(List* list){
	ListNode* memory = list->memory;
	uint64 memorySize = list->memorySize;
	*list = { 0 };
	list->memory = memory;
	list->memorySize = memorySize;
}

#define NUM_CYCLES 6
#define MAX_NODES 10000

/*String text = stringFromConstChar(
R"(.#.
..#
###
)");*/

String text = stringFromConstChar(
R"(##....#.
#.#..#..
...#....
...#.#..
###....#
#.#....#
.#....##
.#.###.#
)");

int main(){
	char* position = text.data;

	List list = createList(MAX_NODES);

	int64 x = 0;
	int64 y = 0;
	while ((position - text.data) < text.size) {
		char c = *(position++);
		if(c == '#'){
			Cube cube = {x, y, 0, 0};
			if (!addCube(&list, cube)){
				return -1;
			}
			x++;
		}else if (c == '\n'){
			x = 0;
			y++;
		} else {
			x++;
		}

	}

	
	List inactiveCubes = createList(MAX_NODES * 4);
	List buffer = createList(MAX_NODES);
	copyList(&buffer, &list);
	
	for(int n = 0; n < NUM_CYCLES; n++){
		for (ListNode* node = list.start; node; node = node->next){
			uint64 neighboorCount = 0;
			for (ListNode* other = list.start; other; other = other->next){
				if(node != other && areNeighboors(node->cube, other->cube)){
					neighboorCount++;
					if(neighboorCount > 3){
						break;
					}
				}
			}

			if(neighboorCount < 2 || neighboorCount > 3){
				removeNode(&buffer, node->id);
			}

			for(int64 x = -1; x <= 1; x++){
				Cube cube;
				cube.x = node->cube.x + x;
				for(int64 y = -1; y <= 1; y++){
					cube.y = node->cube.y + y;
					for(int64 z = -1; z <= 1; z++){
						cube.z = node->cube.z + z;
						for (int64 w = -1; w <= 1; w++) {
							if (x == 0 && y == 0 && z == 0 && w == 0) continue;
							cube.w = node->cube.w + w;

							bool isActive = false;
							for (ListNode* other = list.start; other; other = other->next) {
								if (isSameCube(cube, other->cube)) {
									isActive = true;
									break;
								}
							}

							if (isActive) continue;

							bool exists = false;
							for (ListNode* inactive = inactiveCubes.start; inactive; inactive = inactive->next) {
								Cube other = inactive->cube;
								if (isSameCube(cube, other)) {
									inactive->cube.activeNeighbors++;
									exists = true;
									break;
								}
							}

							if (!exists) {
								cube.activeNeighbors = 1;
								if (!addCube(&inactiveCubes, cube)) {
									return -2;
								}
							}
						}
					}
				}
			}
		}

		for (ListNode* inactive = inactiveCubes.start; inactive; inactive = inactive->next){
			if(inactive->cube.activeNeighbors == 3){
				if(!addCube(&buffer, inactive->cube)){
					return -3;
				}
			}
		}

		clearList(&inactiveCubes);
		copyList(&list, &buffer);
	}

	printf("Number of active cubes: %llu\n", list.size);
	return 0;
}