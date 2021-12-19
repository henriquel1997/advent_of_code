#include "common.h"

struct Beacon {
	int64 x;
	int64 y;
	int64 z;
	Beacon* next;
};

struct Scanner {
	bool done;
	Beacon position;
	uint64 num_beacons;
	Beacon* beacons;
};

#define MAX_SCANNERS 30
#define MAX_BEACONS 1000
#define MIN_BEACONS_MATCH 12

Scanner parse_scanner(char** position, uint64* num_beacons, Beacon* beacons){
	Scanner scanner = { 0 };

	while (!isEndOfLine(**position)) (*position)++;
	(*position)++;

	while (**position == '-' || isDigit(**position)) {
		Beacon beacon = { 0 };
		beacon.x = strtoll(*position, position, 10);
		while (**position != '-' && !isDigit(**position)) (*position)++;
		beacon.y = strtoll(*position, position, 10);
		while (**position != '-' && !isDigit(**position)) (*position)++;
		beacon.z = strtoll(*position, position, 10);
	
		(*position)++;

		beacons[*num_beacons] = beacon;
		printf("%lli,%lli,%lli\n", beacon.x, beacon.y, beacon.z);

		if (!scanner.beacons){
			scanner.beacons = &beacons[*num_beacons];
		} else {
			Beacon* prev = scanner.beacons;
			while (prev->next) prev = prev->next;
			prev->next = &beacons[*num_beacons];
		}

		(*num_beacons)++;
		scanner.num_beacons++;
	}

	(*position)++;
	printf("\nNum beacons: %llu\n\n", scanner.num_beacons);

	return scanner;
}

bool equals(Beacon beacon1, Beacon beacon2){
	return beacon1.x == beacon2.x && beacon1.y == beacon2.y && beacon1.z == beacon2.z;
}

int sin(int x){
	switch(x){
		case 90: return 1;
		case 270: return -1;
		default: return 0;
	}
}

int cos(int x){
	switch(x){
		case 0: return 1;
		case 180: return -1;
		default: return 0;
	}
}

Beacon rotate(Beacon beacon, int rot_x, int rot_y, int rot_z) {
	Beacon result;
	
	result.x = (cos(rot_z) * cos(rot_y) * beacon.x) + 
	(((cos(rot_z) * sin(rot_y) * sin(rot_x)) - (sin(rot_z) * cos(rot_x))) * beacon.y) +
	(((cos(rot_z) * sin(rot_y) * cos(rot_x)) + (sin(rot_z) * sin(rot_x))) * beacon.z);
	
	result.y = (sin(rot_z) * cos(rot_y) * beacon.x) + 
	(((sin(rot_z) * sin(rot_y) * sin(rot_x)) + (cos(rot_z) * cos(rot_x))) * beacon.y) +
	(((sin(rot_z) * sin(rot_y) * cos(rot_x)) - (cos(rot_z) * sin(rot_x))) * beacon.z);
	
	result.z = (-sin(rot_y) * beacon.x) + 
	(cos(rot_y) * sin(rot_x) * beacon.y) +
	(cos(rot_y) * cos(rot_x) * beacon.z);
	
	return result;
}

Beacon convert_to_new_origin(Beacon beacon, Beacon new_origin){
	beacon.x -= new_origin.x;
	beacon.y -= new_origin.y;
	beacon.z -= new_origin.z;
	return beacon;
}

Beacon convert_to_coordinate_system(
	Beacon this_beacon, 
	Beacon this_beacon_origin, 
	Beacon other_scanner_coordinate, 
	int rot_x, int rot_y, int rot_z
){
	Beacon result = convert_to_new_origin(this_beacon, this_beacon_origin);
	result = rotate(result, rot_x, rot_y, rot_z);
	result.x += other_scanner_coordinate.x;
	result.y += other_scanner_coordinate.y;
	result.z += other_scanner_coordinate.z;
	return result;
}

int main(){
	String text = readFile("input.txt");
	
	char* position = text.data;

	uint64 num_scanners = 0;
	Scanner* scanners = (Scanner*) malloc(MAX_SCANNERS * sizeof(Scanner));
	uint64 num_beacons = 0;
	Beacon* beacons = (Beacon*) malloc(MAX_BEACONS * sizeof(Beacon));

	while ((position - text.data) < text.size) {
		printf("--- scanner %llu ---\n", num_scanners);
		scanners[num_scanners++] = parse_scanner(&position, &num_beacons, beacons);
	}
	
	next_scanner:
	for (uint64 scanner_index = 1; scanner_index < num_scanners; scanner_index++) {
		Scanner* scanner = &scanners[scanner_index];
		if (scanner->done) continue;
	
		for (Beacon* origin = scanner->beacons; origin; origin = origin->next) {
			for (int z_rot = 0; z_rot < 360; z_rot += 90) {
				for (int y_rot = 0; y_rot < 360; y_rot += 90) {
					for (int x_rot = 0; x_rot < 360; x_rot += 90) {
						for (Beacon* first_scanner_origin = scanners[0].beacons; first_scanner_origin; first_scanner_origin = first_scanner_origin->next) {
	
							uint64 num_matches = 0;
							for (uint64 j = 0; j < scanner->num_beacons; j++) {
								if (equals(*origin, scanner->beacons[j])) continue;
	
								Beacon result = convert_to_coordinate_system(scanner->beacons[j], *origin, *first_scanner_origin, x_rot, y_rot, z_rot);
	
								for (Beacon* b = scanners[0].beacons; b; b = b->next) {
									if (equals(result, *b)) {
										num_matches++;
										break;
									}
								}
	
								if (num_matches >= MIN_BEACONS_MATCH - 1) break;
							}
	
							if (num_matches >= MIN_BEACONS_MATCH - 1) {
								//Found matching beacons
	
								//Keep only the new beacons that aren't already included
								Beacon* next;
								for (Beacon* b = scanner->beacons; b; b = next) {
									next = b->next;
									if(equals(*b, *origin)) continue;
	
									Beacon result = convert_to_coordinate_system(*b, *origin, *first_scanner_origin, x_rot, y_rot, z_rot);
	
									bool already_included = false;
									for (Beacon* b0 = scanners[0].beacons; b0; b0 = b0->next) {
										if (equals(result, *b0)) {
											already_included = true;
											break;
										}
									}
	
									if (!already_included) {
										Beacon* prev = scanners[0].beacons;
										while (prev->next) prev = prev->next;
										*b = result;
										b->next = 0;
										prev->next = b;
										scanners[0].num_beacons++;
									}
								}
	
								scanner->done = true;
								scanner->position = convert_to_coordinate_system(scanner->position, *origin, *first_scanner_origin, x_rot, y_rot, z_rot);
	
								printf("Scanner %llu position: %lli, %lli, %lli\n", scanner_index, scanner->position.x, scanner->position.y, scanner->position.z);
								printf("Current beacons: %llu\n", scanners[0].num_beacons);
								goto next_scanner;
							}
						}
					}
				}
			}
		}
	}

	printf("Total beacons: %llu\n", scanners[0].num_beacons);

	uint64 longest_distance = 0;
	for (uint64 i = 0; i < num_scanners; i++){
		Scanner s1 = scanners[i];
		for (uint64 j = 0; j < num_scanners; j++) {
			if (i == j) continue;

			Scanner s2 = scanners[j];
			uint64 distance = abs(s1.position.x - s2.position.x) + abs(s1.position.y - s2.position.y) + abs(s1.position.z - s2.position.z);
			printf("Distance between %llu and %llu: %llu\n", i, j, distance);
			if (distance > longest_distance) longest_distance = distance;
		}
	}

	printf("Longest Manhattan distance: %llu\n", longest_distance);
	return 0;
}