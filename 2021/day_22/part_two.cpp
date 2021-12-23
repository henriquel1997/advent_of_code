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
#define is_in_range(x, range_min, range_max) (((x) >= (range_min)) && ((x) <= (range_max)))

bool intersects(Cuboid c1, Cuboid c2){
    return (
		is_in_range(c1.min_x, c2.min_x, c2.max_x) || is_in_range(c1.max_x, c2.min_x, c2.max_x) || 
		is_in_range(c2.min_x, c1.min_x, c1.max_x) || is_in_range(c2.max_x, c1.min_x, c1.max_x)
	) && (
		is_in_range(c1.min_y, c2.min_y, c2.max_y) || is_in_range(c1.max_y, c2.min_y, c2.max_y) || 
		is_in_range(c2.min_y, c1.min_y, c1.max_y) || is_in_range(c2.max_y, c1.min_y, c1.max_y)
	) && (
		is_in_range(c1.min_z, c2.min_z, c2.max_z) || is_in_range(c1.max_z, c2.min_z, c2.max_z) || 
		is_in_range(c2.min_z, c1.min_z, c1.max_z) || is_in_range(c2.max_z, c1.min_z, c1.max_z)
	);
}

Cuboid get_intersection_cuboid(Cuboid c1, Cuboid c2){
	Cuboid intersection;
	intersection.min_x = max(c1.min_x, c2.min_x);
	intersection.max_x = min(c1.max_x, c2.max_x);
	intersection.min_y = max(c1.min_y, c2.min_y);
	intersection.max_y = min(c1.max_y, c2.max_y);
	intersection.min_z = max(c1.min_z, c2.min_z);
	intersection.max_z = min(c1.max_z, c2.max_z);
	return intersection;
}

void add_cuboid_parts_to_array(Cuboid cuboid, Cuboid remover, uint64* num_cuboids, Cuboid* cuboids){
	Cuboid inter = get_intersection_cuboid(cuboid, remover);

	if (inter.min_x > cuboid.min_x){
		Cuboid left = inter;
		left.min_x = cuboid.min_x;
		left.max_x = inter.min_x - 1;
		cuboids[(*num_cuboids)++] = left;
	}

	if (inter.max_x < cuboid.max_x){
		Cuboid right = inter;
		right.min_x = inter.max_x + 1;
		right.max_x = cuboid.max_x;
		cuboids[(*num_cuboids)++] = right;
	}

	if (inter.min_y > cuboid.min_y){
		Cuboid top = cuboid;
		top.max_y = inter.min_y - 1;
		cuboids[(*num_cuboids)++] = top;
	}
	
	if (inter.max_y < cuboid.max_y){
		Cuboid bottom = cuboid;
		bottom.min_y = inter.max_y + 1;
		cuboids[(*num_cuboids)++] = bottom;
	}

	if (inter.min_z > cuboid.min_z){
		Cuboid front = cuboid;
		front.max_z = inter.min_z - 1;
		front.min_y = inter.min_y;
		front.max_y = inter.max_y;
		cuboids[(*num_cuboids)++] = front;
	}
	
	if (inter.max_z < cuboid.max_z){
		Cuboid back = cuboid;
		back.min_z = inter.max_z + 1;
		back.min_y = inter.min_y;
		back.max_y = inter.max_y;
		cuboids[(*num_cuboids)++] = back;
	}
}

int64 get_cuboid_volume(Cuboid cuboid){
	return abs(cuboid.max_x - cuboid.min_x + 1) * abs(cuboid.max_y - cuboid.min_y + 1) * abs(cuboid.max_z - cuboid.min_z + 1);
}

int main(){
	String text = readFile("input.txt");
	
	char* position = text.data;

	uint64 num_cuboids = 0;
	Cuboid* cuboids = (Cuboid*) malloc(MAX_CUBOIDS * sizeof(Cuboid));

	while ((position - text.data) < text.size) {
		Cuboid cuboid;
		cuboid.on = startsWith(position, "on");
		printf(cuboid.on ? "On\n" : "Off\n");

		while (*position != '-' && !isDigit(*position)) position++;
		cuboid.min_x = strtoll(position, &position, 10);
		printf("Min x: %lli\n", cuboid.min_x);

		while (*position != '-' && !isDigit(*position)) position++;
		cuboid.max_x = strtoll(position, &position, 10);
		printf("Max x: %lli\n", cuboid.max_x);

		while (*position != '-' && !isDigit(*position)) position++;
		cuboid.min_y = strtoll(position, &position, 10);
		printf("Min y: %lli\n", cuboid.min_y);

		while (*position != '-' && !isDigit(*position)) position++;
		cuboid.max_y = strtoll(position, &position, 10);
		printf("Max y: %lli\n", cuboid.max_y);

		while (*position != '-' && !isDigit(*position)) position++;
		cuboid.min_z = strtoll(position, &position, 10);
		printf("Min z: %lli\n", cuboid.min_z);

		while (*position != '-' && !isDigit(*position)) position++;
		cuboid.max_z = strtoll(position, &position, 10);
		printf("Max z: %lli\n", cuboid.max_z);

		printf("Volume: %lli\n", get_cuboid_volume(cuboid));
		{
			uint64 index = 0;
			uint64 num_cuboids_start = num_cuboids;
			while (index < num_cuboids_start) {
				bool intersected = intersects(cuboid, cuboids[index]);
				if (intersected){
					add_cuboid_parts_to_array(cuboids[index], cuboid, &num_cuboids, cuboids);
					for(uint64 i = index; i < (num_cuboids - 1); i++){
						cuboids[i] = cuboids[i + 1];
					}
					num_cuboids--;
					num_cuboids_start--;
				}
				if(!intersected) index++;
			}

			if (cuboid.on){
				cuboids[num_cuboids++] = cuboid;
			}
		}
	  	
		while(*position != '\n') position++;
		position++;
   }

	uint64 num_cubes = 0;
	for(uint64 i = 0; i < num_cuboids; i++){
		num_cubes += get_cuboid_volume(cuboids[i]);
	}
	printf("Num cubes: %llu\n", num_cubes);
	return 0;
}