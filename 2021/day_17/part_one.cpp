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


    int64 max_height = 0;
    int64 max_height_dir_x = 0;
    int64 max_height_dir_y = 0;
    int64 sign_x = sign((min_x + max_x) / 2);
    for(int64 start_dir_y = 1; start_dir_y <= 130; start_dir_y++){
        for(int64 start_dir_x = sign_x; abs(start_dir_x) <= max(min_x, max_x); start_dir_x += sign_x){
            int64 dir_x = start_dir_x;
            int64 dir_y = start_dir_y;
            int64 pos_x = 0;
            int64 pos_y = 0;

            int64 current_max_height = max_height;
            printf("Simulate (%lli) (%lli, %lli) (%lli, %lli) (%lli, %lli)\n", max_height, start_dir_x, start_dir_y, dir_x, dir_y, pos_x, pos_y);

            while(pos_y > min(min_y, max_y)){
                //Simulate
                pos_x += dir_x;
                pos_y += dir_y;

                dir_x = max(0, dir_x - sign(dir_x)); //drag
                dir_y--; //gravity

                printf("Simulate (%lli) (%lli, %lli) (%lli, %lli) (%lli, %lli)\n", max_height, start_dir_x, start_dir_y, dir_x, dir_y, pos_x, pos_y);

                if(pos_y > current_max_height) current_max_height = pos_y;

                if(
                    pos_x >= min(min_x, max_x) && pos_x <= max(min_x, max_x) && 
                    pos_y >= min(min_y, max_y) && pos_y <= max(min_y, max_y)
                ){
                    printf("Hit the target\n");
                    if(current_max_height > max_height){
                        max_height = current_max_height;
                        max_height_dir_x = start_dir_x;
                        max_height_dir_y = start_dir_y;
                    }
                    break;
                }
            }
        }
    }

    printf("max height=%lli\n", max_height);
    printf("dir x=%lli\n", max_height_dir_x);
    printf("dir y=%lli\n", max_height_dir_y);
}