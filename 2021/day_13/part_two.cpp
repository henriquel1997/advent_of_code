#include "common.h"

struct Dot {
    uint64 x;
    uint64 y;
};

int main(){
    String text = readFile("input.txt");
	
    char* position = text.data;

    uint64 num_dots = 0;
    while(!isWhiteSpace(*position)){
        if(*position == '\n') num_dots++;
        position++;
    }

    Dot* dots = (Dot*) malloc(num_dots * sizeof(Dot));
    num_dots = 0;

    position = text.data;

    while(!isEndOfLine(*position)){
        long int x = strtol(position, &position, 10);
        position++;
        long int y = strtol(position, &position, 10);
        position++;
        dots[num_dots++] = (Dot){x, y};
    }

    Dot* result = (Dot*) malloc(num_dots * sizeof(Dot));
    

    //Do folds
    while ((position - text.data) < text.size){
        while(*position != 'x' && *position != 'y') position++;

        bool fold_is_x = *position == 'x';
        while(!isDigit(*position)) position++;
        uint64 fold_value = strtol(position, &position, 10);
        position++;

        uint64 result_size = 0;

        for(uint64 i = 0; i < num_dots; i++){
            Dot dot = dots[i];
            if(fold_is_x){
                if(dot.x == fold_value) continue;
                if(dot.x > fold_value) dot.x -= 2 * (dot.x - fold_value);
            }else{
                if(dot.y == fold_value) continue;
                if(dot.y > fold_value) dot.y -= 2 * (dot.y - fold_value);
            }

            bool already_exists = false;
            for(uint64 j = 0; j < result_size; j++){
                Dot other = result[j];
                if(other.x == dot.x && other.y == dot.y){
                    already_exists = true;
                    break;
                }
            }

            if(!already_exists) result[result_size++] = dot;
        }

        Dot* aux = dots;
        dots = result;
        result = aux;

        num_dots = result_size;
    }

    //Print result
    uint64 width = 0;
    uint64 height = 0;
    for(uint64 i = 0; i < num_dots; i++){
        Dot dot = dots[i];
        if(dot.x > width) width = dot.x;
        if(dot.y > height) height = dot.y;
    }

    uint64 grid_size = (++width) * (++height);
    bool* grid = (bool*) calloc(grid_size, sizeof(bool));

    for(uint64 i = 0; i < num_dots; i++){
        Dot dot = dots[i];
        grid[(dot.y * width) + dot.x] = true;
    }

    for(uint64 i = 0; i < grid_size; i++){
        if(i % width == 0) printf("\n");
        printf(grid[i] ? "#" : ".");
    }
    return 0;
}