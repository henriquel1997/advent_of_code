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

struct Cube {
    int64 x;
    int64 y;
    int64 z;
};

#define MAX_CUBES 10000000

bool equals(Cube c1, Cube c2){
    return c1.x == c2.x && c1.y == c2.y && c1.z == c2.z;
}

int main(){
    String text = readFile("input.txt");
	
	char* position = text.data;

    uint64 num_cubes = 0;
    Cube* cubes = (Cube*) malloc(MAX_CUBES * sizeof(Cube));

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
        
        printf("Volume: %lli\n", abs(cuboid.max_x - cuboid.min_x + 1) * abs(cuboid.max_y - cuboid.min_y + 1) * abs(cuboid.max_z - cuboid.min_z + 1));
        
        for(int64 x = cuboid.min_x; x <= cuboid.max_x; x++){
            for(int64 y = cuboid.min_y; y <= cuboid.max_y; y++){
                for(int64 z = cuboid.min_z; z <= cuboid.max_z; z++){
                    Cube cube = {x, y, z};
                    if(cuboid.on){
                        bool already_exists = false;
                        for(uint64 i = 0; i < num_cubes; i++){
                            if(equals(cubes[i], cube)){
                                already_exists = true;
                                break;
                            }
                        }
                        if(!already_exists){
                            cubes[num_cubes++] = cube;
                            if(num_cubes >= MAX_CUBES){
                                printf("Cube array is too small\n");
                                return -1;
                            }
                        }
                    }else{
                        uint64 index = MAX_CUBES;
                        for(uint64 i = 0; i < num_cubes; i++){
                            if(equals(cubes[i], cube)){
                                index = i;
                                break;
                            }
                        }

                        if(index == MAX_CUBES) continue;
                        for(uint64 i = index; i < num_cubes - 1; i++){
                            cubes[i] = cubes[i + 1];
                        }
                        num_cubes--;
                    }
                }
            }
        }
        printf("Num cubes: %llu\n", num_cubes);

        end:
        while(!isEndOfLine(*position)) position++;
        position++;
    }

    printf("Num cubes: %llu\n", num_cubes);
    return 0;
}