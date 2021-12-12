#include "common.h"

struct Node {
	String name;
	uint64 num_others;
	Node** others;
	bool is_small;
	bool visited;
};

String read_name(char** position){
	char* nameStart = *position;
	while(**position != '-' && **position != '\n') (*position)++;
	long nameSize = (*position) - nameStart;
	(*position)++;
	return { nameSize, nameStart };
}

uint64 get_number_of_paths(Node* node){
	if (strings_equal(node->name, "end")) return 1;

	if (node->is_small) node->visited = true;

	uint64 num_paths = 0;

	for (uint64 i = 0; i < node->num_others; i++){
		Node* other = node->others[i];
		if(other->visited) continue;

		num_paths += get_number_of_paths(other);
	}

	node->visited = false;
	return num_paths;
}

int main(){
	String text = readFile("input.txt");
	TextFileInfo info = getTextFileInfo(text);

	uint64 max_nodes = info.numberOfLines * 2;
	uint64 nodes_size = 0;
	Node* nodes = (Node*) malloc(max_nodes * sizeof(Node));
	uint64 node_pointers_size = 0;
	Node** node_pointers = (Node**) malloc(max_nodes * max_nodes * sizeof(Node*));

	Node* start;
	
	char* position = text.data;
	while((position - text.data) < text.size){
		String name = read_name(&position);
		bool found = false;
		for(uint64 i = 0; i < nodes_size; i++){
			if(strings_equal(nodes[i].name, name)){
				found = true;
				break;
			}
		}

		if(!found){
			bool is_small = true;
			for (long i = 0; i < name.size; i++){
				char c = name.data[i];
				if(c < 97 || c > 122){
					is_small = false;
					break;
				}
			}
			nodes[nodes_size] = { name, 0, 0, is_small, false };
			if (strings_equal(name, "start")) {
				start = &nodes[nodes_size];
			}
			nodes_size++;
		}
	}

	for (uint64 index = 0; index < nodes_size; index++) {
		Node* current_node = &nodes[index];
		current_node->others = &node_pointers[node_pointers_size];

		position = text.data;
		while ((position - text.data) < text.size) {
			String name = read_name(&position);
			uint64 i;
			for (i = 0; i < nodes_size; i++) {
				if (strings_equal(nodes[i].name, name)) break;
			}

			Node* first_node = &nodes[i];

			name = read_name(&position);
			for (i = 0; i < nodes_size; i++) {
				if (strings_equal(nodes[i].name, name)) break;
			}

			Node* second_node = &nodes[i];

			if (strings_equal(current_node->name, first_node->name)){
				current_node->others[current_node->num_others++] = second_node;
				node_pointers_size++;
			}else if (strings_equal(current_node->name, second_node->name)){
				current_node->others[current_node->num_others++] = first_node;
				node_pointers_size++;
			}
		}
	}

	uint64 result = get_number_of_paths(start);
	printf("Total paths: %llu\n", result);
}