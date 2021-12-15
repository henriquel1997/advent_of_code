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

int main(){
    String text = readFile("input.txt");
    TextFileInfo info = getTextFileInfo(text);
	
    char* position = text.data;

    uint64 num_nodes = info.numberOfLines * info.maxLineSize;
    Node* graph = (Node*) calloc(num_nodes, sizeof(Node));

    uint64 current_pos = 0;
    while((position - text.data) < text.size){
        int64 number = *(position++) - '0';
        Node* node = &graph[current_pos];
        node->cost = number;
        node->total_cost = -1;

        if(current_pos >= info.maxLineSize){ 
            node->adjacent_nodes[node->num_adjacents++] = &graph[current_pos - info.maxLineSize];
        }
        
        if(current_pos + info.maxLineSize < num_nodes){
            node->adjacent_nodes[node->num_adjacents++] = &graph[current_pos + info.maxLineSize];
        }

        if(current_pos % info.maxLineSize != 0){
            node->adjacent_nodes[node->num_adjacents++] = &graph[current_pos - 1];
        }

        if((current_pos + 1) % info.maxLineSize != 0){ 
            node->adjacent_nodes[node->num_adjacents++] = &graph[current_pos + 1];
        }

        if(*position == '\n') position++;
        current_pos++;
    }

    Node* current_node = &graph[0];
    current_node->total_cost = 0;
    Node* previous_node = 0;
    Node* end = &graph[current_pos - 1];

    while(current_node){
        for(int i = 0; i < current_node->num_adjacents; i++){
            Node* adjacent = current_node->adjacent_nodes[i];
            if(adjacent == previous_node) continue;

            int64 total_cost = current_node->total_cost + adjacent->cost;
            if(adjacent->total_cost < 0 || adjacent->total_cost > total_cost){
                adjacent->total_cost = total_cost;
                adjacent->prev = current_node;

                if(adjacent == end){
                    for(Node* n = adjacent; n; n = n->prev){
                        n->in_path = true;
                    }

                    for(uint64 i = 0; i < num_nodes; i++){
                        if(i % info.maxLineSize == 0) printf("\n");
                        if(graph[i].in_path) printf("*");
                        else printf("%lli", graph[i].cost);
                    }

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

    printf("Couldn't find a path\n");
    return -1;
}