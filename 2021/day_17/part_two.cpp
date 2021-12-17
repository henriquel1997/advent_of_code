#include "common.h"

int64 read_next_number(char** position){
    while(**position != '-' && !isDigit(**position)) (*position)++;
    return strtoll(*position, position, 10);
}

int main(){
    String text = readFile("input.txt");
	
    char* position = text.data;

    int64 min_x = read_next_number(&position);
    int64 max_x = read_next_number(&position);
    int64 min_y = read_next_number(&position);
    int64 max_y = read_next_number(&position);

    printf("x=%lli..%lli, y=%lli..%lli\n", min_x, max_x, min_y, max_y);


    uint64 num_hits = 0;
    int64 sign_x = sign((min_x + max_x) / 2);
    for(int64 start_dir_y = -130; start_dir_y <= 130; start_dir_y++){
        for(int64 start_dir_x = sign_x; abs(start_dir_x) <= max(min_x, max_x); start_dir_x += sign_x){
            int64 dir_x = start_dir_x;
            int64 dir_y = start_dir_y;
            int64 pos_x = 0;
            int64 pos_y = 0;

            printf("Simulate (%lli, %lli) (%lli, %lli) (%lli, %lli)\n", start_dir_x, start_dir_y, dir_x, dir_y, pos_x, pos_y);

            while(pos_y > min(min_y, max_y)){
                //Simulate
                pos_x += dir_x;
                pos_y += dir_y;

                dir_x = max(0, dir_x - sign(dir_x)); //drag
                dir_y--; //gravity

                printf("Simulate (%lli, %lli) (%lli, %lli) (%lli, %lli)\n", start_dir_x, start_dir_y, dir_x, dir_y, pos_x, pos_y);

                if(
                    pos_x >= min(min_x, max_x) && pos_x <= max(min_x, max_x) && 
                    pos_y >= min(min_y, max_y) && pos_y <= max(min_y, max_y)
                ){
                    printf("Hit the target\n");
                    num_hits++;
                    break;
                }
            }
        }
    }

    printf("Num hits=%llu\n", num_hits);
}