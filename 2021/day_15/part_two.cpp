#include "common.h"

struct Node {
    int64 cost;
    int64 total_cost;
    Node* prev;

    Node* prev_in_list;
    Node* next_in_list;

    int num_adjacents;
    Node* adjacent_nodes[4];
    bool in_path;
};

#define NUM_REPEATS 5

int main(){
    String text = readFile("input.txt");
    TextFileInfo info = getTextFileInfo(text);
	
    char* position = text.data;

    uint64 num_nodes = info.numberOfLines * info.maxLineSize;
    uint64 total_nodes = num_nodes * NUM_REPEATS * NUM_REPEATS;
    Node* graph = (Node*) calloc(total_nodes, sizeof(Node));

    uint64 current_pos = 0;
    uint64 line_size = info.maxLineSize * NUM_REPEATS;
    while((position - text.data) < text.size){
        int64 number = *(position++) - '0';
        for(int64 y = 0; y < NUM_REPEATS; y++){
            for(int64 x = 0; x < NUM_REPEATS; x++){
                uint64 pos = current_pos + (x * info.maxLineSize) + (y * (num_nodes * NUM_REPEATS));
                Node* node = &graph[pos];
                node->cost = ((number + x + y - 1) % 9) + 1;
                node->total_cost = -1;

                if(pos >= line_size){ 
                    node->adjacent_nodes[node->num_adjacents++] = &graph[pos - line_size];
                }

                if(pos + line_size < total_nodes){
                    node->adjacent_nodes[node->num_adjacents++] = &graph[pos + line_size];
                }

                if(pos % line_size != 0){
                    node->adjacent_nodes[node->num_adjacents++] = &graph[pos - 1];
                }

                if((pos + 1) % line_size != 0){ 
                    node->adjacent_nodes[node->num_adjacents++] = &graph[pos + 1];
                }
            }
        }

        if(*position == '\n') position++;
        current_pos++;
        if(current_pos % info.maxLineSize == 0){
            current_pos += (NUM_REPEATS - 1) * info.maxLineSize;
        }
    }

    Node* current_node = &graph[0];
    current_node->total_cost = 0;
    Node* previous_node = 0;
    Node* end = &graph[total_nodes - 1];

    while(current_node){
        for(int i = 0; i < current_node->num_adjacents; i++){
            Node* adjacent = current_node->adjacent_nodes[i];
            if(adjacent == previous_node) continue;

            int64 total_cost = current_node->total_cost + adjacent->cost;
            if(adjacent->total_cost < 0 || adjacent->total_cost > total_cost){
                adjacent->total_cost = total_cost;
                adjacent->prev = current_node;

                if(adjacent == end){
                    /*for(Node* n = adjacent; n; n = n->prev){
                        n->in_path = true;
                    }

                    for(uint64 i = 0; i < total_nodes; i++){
                        if(i % line_size == 0) printf("\n");
                        if(graph[i].in_path) printf("*");
                        else printf("%lli", graph[i].cost);
                    }*/

                    printf("\nLowest risk path cost: %lli\n", total_cost);
                    return 0;
                }

                if(adjacent->prev_in_list){
                    adjacent->prev_in_list->next_in_list = adjacent->next_in_list;
                    if(adjacent->next_in_list) 
                        adjacent->next_in_list->prev_in_list = adjacent->prev_in_list;
                }

                Node* node = current_node->next_in_list;
                Node* prev = current_node;
                while(node && node->total_cost <= adjacent->total_cost){
                    prev = node;
                    node = node->next_in_list;
                }
                if(node){
                    node->prev_in_list->next_in_list = adjacent;
                    adjacent->prev_in_list = node->prev_in_list;
                    adjacent->next_in_list = node;
                    node->prev_in_list = adjacent;
                }else{
                    prev->next_in_list = adjacent;
                    adjacent->prev_in_list = prev;
                    adjacent->next_in_list = 0;
                }
            }
        }

        previous_node = current_node;
        current_node = current_node->next_in_list;
    }

    printf("\nCouldn't find a path\n");
    return -1;
}