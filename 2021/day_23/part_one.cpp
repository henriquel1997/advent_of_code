#include "common.h"

#define HALLWAY_SIZE 11
#define MAX_AMPHIPODS 8

enum AmphipodType {
	A = 0, 
	B = 1, 
	C = 2, 
	D = 3
};

struct Amphipod {
	int room_index; //-1 if in hallway
	int position;
	AmphipodType type;
};

struct State{
	Amphipod amphipods[MAX_AMPHIPODS];
	uint64 energy_used;
	State* next_in_list;
};

bool is_win_state(State state){
	for(int i = 0; i < MAX_AMPHIPODS; i++){
		Amphipod a = state.amphipods[i];
		if(a.room_index != a.type){
			return false;
		}
	}
	return true;
}

bool can_move(State state, int amp_index){
	Amphipod a = state.amphipods[amp_index];
	if(a.room_index < 0) return true;
	if(a.room_index == a.type){
		if (a.position == 0){
			return false;
		} else {
			for(int i = 0; i < MAX_AMPHIPODS; i++){
				if(i == amp_index) continue;
				Amphipod b = state.amphipods[i];
				if(b.type == a.type){
					return b.room_index != b.type;
				}
			}

			return false;
		}
	}
	
	if(a.position == 1) return true;

	for(int i = 0; i < MAX_AMPHIPODS; i++){
		if(i == amp_index) continue;
		Amphipod b = state.amphipods[i];
		if(b.room_index == a.room_index){
			return false;
		}
	}

	return true;
}

int get_room_available_position(State state, int room_index){
	int pos = 0;
	for(int i = 0; i < MAX_AMPHIPODS; i++){
		Amphipod a = state.amphipods[i];
		if(a.room_index == room_index){
			if(a.position == 1){
				return -1;
			}else{
				if(a.type != room_index) return -1;
				pos = 1;
			}
		}
	}
	return pos;
}

uint64 get_energy_multiplier(AmphipodType type){
	switch(type){
		case A: return 1;
		case B: return 10;
		case C: return 100;
		case D: return 1000;
	}
}

int get_room_position_in_hallway(int room_index){
	return (room_index + 1) * 2;
}

bool is_hallway_path_clear(State state, int path_start, int path_end, int amp_index){
	int start = min(path_start, path_end);
	int end = max(path_start, path_end);
	for (int i = 0; i < MAX_AMPHIPODS; i++){
		if (i == amp_index) continue;
		Amphipod a = state.amphipods[i];
		if (a.room_index < 0 && a.position >= start && a.position <= end){
			return false;
		}
	}
	return true;
}

State find_best_solution(State state, bool test = false);

bool try_to_move_into_target_room(State* state, int amp_index){
	Amphipod* a = &state->amphipods[amp_index];
	int room_pos = get_room_available_position(*state, a->type);
	if(room_pos >= 0){
		int room_entrance = get_room_position_in_hallway(a->type);
		if (is_hallway_path_clear(*state, a->position, room_entrance, amp_index)) {
			uint64 mult = get_energy_multiplier(a->type);
			int dist = abs(a->position - room_entrance);
			state->energy_used += (dist + (room_pos ? 1 : 2)) * mult;
			a->room_index = a->type;
			a->position = room_pos;

			*state = find_best_solution(*state);
			return true;
		}
	}
	return false;
}

State find_best_solution(State state, bool test){
	if (is_win_state(state)) return state;

	State best = state;
	best.energy_used = ULLONG_MAX;
	for(int i = 0; i < MAX_AMPHIPODS; i++){
		if(can_move(state, i)){
			State new_state = state;
			Amphipod* a = &new_state.amphipods[i];
			if(a->room_index >= 0){
				//Is in a room, try to move it to the hallway
				uint64 mult = get_energy_multiplier(a->type);
				new_state.energy_used += (a->position ? 1 : 2) * mult;
				a->position = get_room_position_in_hallway(a->room_index);
				a->room_index = -1;
				
				if(a->room_index != a->type){
					if(try_to_move_into_target_room(&new_state, i)){
						if(is_win_state(new_state) && new_state.energy_used < best.energy_used){
							best = new_state;
						}
						continue;
					}
				}
				
				//Test moving this amphipod to every position in the hallway
				for(int pos = 0; pos < 11; pos++){
					if (pos != 0 && pos != 10 && (pos % 2 == 0)) continue;
					if (!is_hallway_path_clear(new_state, a->position, pos, i)) continue;

					bool available = true;
					for(int j = 0; j < MAX_AMPHIPODS; j++){
						Amphipod a = state.amphipods[j];
						if(a.room_index < 0 && a.position == pos){
							available = false;
							break;
						}
					}
					
					if(available){
						State other_state = new_state;
						int dist = abs(a->position - pos);
						other_state.energy_used += dist * mult;
						other_state.amphipods[i].position = pos;
						
						State result = find_best_solution(other_state);
						if(is_win_state(result) && result.energy_used < best.energy_used){
							best = result;
						}
					}
				}
			}else{
				//In the hallway, try to move into the target room
				if(try_to_move_into_target_room(&new_state, i)){
					if(is_win_state(new_state) && new_state.energy_used < best.energy_used){
						best = new_state;
					}
				}
			}
		}
	}
	
	return best;
}

int main(){
	String text = readFile("input.txt");
	
	char* position = text.data;
	
	State state = { 0 };
	
	int room_index = 0;
	int room_height = 1;
	int num_amphipods = 0;
	for (char* position = text.data; (position - text.data) < text.size; position++) {
		if(isLetterUpperCase(*position)){
			Amphipod a;
			a.room_index = room_index++;
			a.position = room_height;
			a.type = (AmphipodType)(*position - 'A');
			state.amphipods[num_amphipods++] = a;
			
			if(room_index % 4 == 0){
				room_index = 0;
				room_height--;
			}
		}
	}

	State result = find_best_solution(state, true);
	printf("Solution energy: %llu\n", result.energy_used);
	return 0;
}