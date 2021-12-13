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

    printf("Num dots: %llu\n", num_dots);

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
    
    while(*position != 'x' && *position != 'y') position++;
    
    bool fold_is_x = *position == 'x';
    while(!isDigit(*position)) position++;
    uint64 fold_value = strtol(position, &position, 10);
    position++;

    uint64 result_size = 0;
    Dot* result = (Dot*) malloc(num_dots * sizeof(Dot));

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

    printf("Dots after first fold: %llu\n", result_size);

    return 0;
}