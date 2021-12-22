#include "common.h"

struct Cuboid{
    bool on;
    int64 min_x;
    int64 max_x;
    int64 min_y;
    int64 max_y;
    int64 min_z;
    int64 max_z;
};

#define MAX_CUBOIDS 10000
#define MAX_NEW_CUBOIDS 1000
#define is_in_range(x, range_min, range_max) (((x) >= (range_min)) && ((x) <= (range_max)))

bool intersects(Cuboid c1, Cuboid c2){
    return (is_in_range(c1.min_x, c2.min_x, c2.max_x) || is_in_range(c1.max_x, c2.min_x, c2.max_x)) &&
    (is_in_range(c1.min_y, c2.min_y, c2.max_y) || is_in_range(c1.max_y, c2.min_y, c2.max_y)) &&
    (is_in_range(c1.min_z, c2.min_z, c2.max_z) || is_in_range(c1.max_z, c2.min_z, c2.max_z));
}

bool intersection_result(Cuboid cuboid, Cuboid remover, uint64* num_cuboids, Cuboid* cuboids){
    uint64 start_num_cuboids = (*num_cuboids);

    //Top
    int64 top_plane = min(cuboid.max_y + 1, max(cuboid.min_y - 1, remover.min_y));
    if(is_in_range(top_plane, cuboid.min_y, cuboid.max_y)){
        Cuboid top = cuboid;
        top.max_y = top_plane;
        cuboids[(*num_cuboids)++] = top;
    }

    //Bottom
    int64 bottom_plane = max(cuboid.min_y - 1, min(cuboid.max_y + 1, remover.max_y));
    if(is_in_range(bottom_plane, cuboid.min_y, cuboid.max_y)){
        Cuboid bottom = cuboid;
        bottom.min_y = bottom_plane;
        cuboids[(*num_cuboids)++] = bottom;
    }

    //Left
    int64 left_plane = min(cuboid.max_x + 1, max(cuboid.min_x - 1, remover.min_x));
    if(is_in_range(left_plane, cuboid.min_x, cuboid.max_x)){
        Cuboid left = cuboid;
        left.min_y = top_plane;
        left.max_y = bottom_plane;
        left.max_x = left_plane;
        cuboids[(*num_cuboids)++] = left;
    }

    //Right
    int64 right_plane = max(cuboid.min_x - 1, min(cuboid.max_x + 1, remover.max_x));
    if(is_in_range(right_plane, cuboid.min_x, cuboid.max_x)){
        Cuboid right = cuboid;
        right.min_y = top_plane;
        right.max_y = bottom_plane;
        right.min_x = right_plane;
        cuboids[(*num_cuboids)++] = right;
    }

    //Front
    int64 front_plane = min(cuboid.max_z + 1, max(cuboid.min_z - 1, remover.min_z));
    if(is_in_range(front_plane, cuboid.min_z, cuboid.max_z)){
        Cuboid front = cuboid;
        front.min_y = top_plane;
        front.max_y = bottom_plane;
        front.min_x = left_plane;
        front.max_x = right_plane;
        front.max_z = front_plane;
        cuboids[(*num_cuboids)++] = front;
    }

    //Back
    int64 back_plane = max(cuboid.min_z - 1, min(cuboid.max_z + 1, remover.max_z));
    if(is_in_range(back_plane, cuboid.min_z, cuboid.max_z)){
        Cuboid back = cuboid;
        back.min_y = top_plane;
        back.max_y = bottom_plane;
        back.min_x = left_plane;
        back.max_x = right_plane;
        back.min_z = back_plane;
        cuboids[(*num_cuboids)++] = back;
    }

    return start_num_cuboids < (*num_cuboids);
}

int64 get_cuboid_volume(Cuboid cuboid){
    return abs(cuboid.max_x - cuboid.min_x + 1) * abs(cuboid.max_y - cuboid.min_y + 1) * abs(cuboid.max_z - cuboid.min_z + 1);
}

int main(){
    String text = readFile("input.txt");
	
	char* position = text.data;

    uint64 num_cuboids = 0;
    Cuboid cuboids[MAX_CUBOIDS];

    while ((position - text.data) < text.size) {
        Cuboid cuboid;
        cuboid.on = startsWith(position, "on");
        printf(cuboid.on ? "On\n" : "Off\n");

        while(*position != '-' && !isDigit(*position)) position++;
        cuboid.min_x = strtoll(position, &position, 10);
        if(cuboid.min_x < -50 || cuboid.min_x > 50) goto end;
        printf("Min x: %lli\n", cuboid.min_x);
        
        while(*position != '-' && !isDigit(*position)) position++;
        cuboid.max_x = strtoll(position, &position, 10);
        if(cuboid.max_x < -50 || cuboid.max_x > 50) goto end;
        printf("Max x: %lli\n", cuboid.max_x);
        
        while(*position != '-' && !isDigit(*position)) position++;
        cuboid.min_y = strtoll(position, &position, 10);
        if(cuboid.min_y < -50 || cuboid.min_y > 50) goto end;
        printf("Min y: %lli\n", cuboid.min_y);
        
        while(*position != '-' && !isDigit(*position)) position++;
        cuboid.max_y = strtoll(position, &position, 10);
        if(cuboid.max_y < -50 || cuboid.max_y > 50) goto end;
        printf("Max y: %lli\n", cuboid.max_y);
        
        while(*position != '-' && !isDigit(*position)) position++;
        cuboid.min_z = strtoll(position, &position, 10);
        if(cuboid.min_z < -50 || cuboid.min_z > 50) goto end;
        printf("Min z: %lli\n", cuboid.min_z);
        
        while(*position != '-' && !isDigit(*position)) position++;
        cuboid.max_z = strtoll(position, &position, 10);
        if(cuboid.max_z < -50 || cuboid.max_z > 50) goto end;
        printf("Max z: %lli\n", cuboid.max_z);
        
        printf("Volume: %lli\n", get_cuboid_volume(cuboid));

        if(cuboid.on){
            printf("On\n");

            uint64 num_new_cuboids = 0;
            Cuboid new_cuboids[MAX_NEW_CUBOIDS];
            new_cuboids[num_new_cuboids++] = cuboid;
            printf("Num new cuboids: %llu\n", num_new_cuboids);

            for(uint64 i = 0; i < num_cuboids; i++){
                uint64 num_new_cuboids_start = num_new_cuboids;
                for(uint64 j = 0; j < num_new_cuboids_start; j++){
                    if(intersects(cuboids[i], new_cuboids[j])){
                        intersection_result(new_cuboids[j], cuboids[i], &num_new_cuboids, new_cuboids);
                        if(num_new_cuboids >= MAX_NEW_CUBOIDS){
                            printf("New cuboid array is too small\n");
                            return -1;
                        }
                        for(uint64 index = j; index < num_new_cuboids - 1; index++){
                            new_cuboids[index] = new_cuboids[index + 1];
                        }
                        num_new_cuboids_start--;
                        num_new_cuboids--;
                        printf("(%llu, %llu) Num new cuboids: %llu\n", i, j, num_new_cuboids);
                    }
                }
            }

            printf("Num cuboids: %llu\n", num_cuboids);
            
            for(uint64 i = 0; i < num_new_cuboids; i++){
                cuboids[num_cuboids++] = new_cuboids[i];
                if(num_cuboids >= MAX_CUBOIDS){
                    printf("Cuboid array is too small\n");
                    return -1;
                }
            }

            printf("Num cuboids: %llu\n", num_cuboids);
        }else{
            printf("Off\n");
            uint64 num_cuboids_start = num_cuboids;
            for(uint64 i = 0; i < num_cuboids_start; i++){
                if(intersects(cuboids[i], cuboid) && intersection_result(cuboids[i], cuboid, &num_cuboids, cuboids)){
                    if(num_cuboids >= MAX_NEW_CUBOIDS){
                        printf("Cuboid array is too small\n");
                        return -1;
                    }
                    for(uint64 index = i; index < num_cuboids - 1; index++){
                        cuboids[index] = cuboids[index + 1];
                    }
                    num_cuboids_start--;
                    num_cuboids--;
                }
            }
        }

        end:
        while(!isEndOfLine(*position)) position++;
        position++;
    }

    printf("Num cuboids: %llu\n", num_cuboids);
    uint64 num_cubes = 0;
    for(uint64 i = 0; i < num_cuboids; i++){
        Cuboid cuboid = cuboids[i];
        num_cubes += get_cuboid_volume(cuboid);
    }

    printf("Num cubes: %llu\n", num_cubes);
    return 0;
}