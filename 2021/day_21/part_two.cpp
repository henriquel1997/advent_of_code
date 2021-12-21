#include "common.h"

struct GameState{
    uint64 players_pos[2];
    uint64 players_score[2];
    uint64 round;
    uint64 players_wins[2];

    GameState* next_in_list;
};
#define GAME_STATE_ARRAY_SIZE 1000000
#define WIN_SCORE 21

uint64 num_states = 0;
GameState* state_array;
GameState** state_table;

void print_state(GameState state){
    printf("Player 0 pos: %llu\n", state.players_pos[0]);
    printf("Player 1 pos: %llu\n", state.players_pos[1]);
    printf("Player 0 score: %llu\n", state.players_score[0]);
    printf("Player 1 score: %llu\n", state.players_score[1]);
    printf("Round: %llu\n", state.round);
    printf("Player 0 wins: %llu\n", state.players_wins[0]);
    printf("Player 1 wins: %llu\n", state.players_wins[1]);
}

//Really bad hashing function, but good enough to not have too many collisions
uint64 get_state_index(GameState state){
    uint64 pos_number = (((state.players_pos[0] - 1) * 10) + (state.players_pos[1] - 1));
    uint64 score_number = ((min(21, state.players_score[0]) * 22) + min(21, state.players_score[1]));
    uint64 round_number = (state.round % 6);

    uint64 index = (pos_number * ((GAME_STATE_ARRAY_SIZE)/100)) +
        (score_number * ((((GAME_STATE_ARRAY_SIZE)/100)/82)/44)) +
        (round_number * (((((GAME_STATE_ARRAY_SIZE)/100)/82)/44)/6));
    
    index %= GAME_STATE_ARRAY_SIZE;
    return index;
}

void add_state(GameState state){
    if(num_states >= GAME_STATE_ARRAY_SIZE){
        printf("Array is too small\n");
        exit(1);
    }

    state.next_in_list = 0;
    state_array[num_states] = state;

    uint64 index = get_state_index(state);
    if(!state_table[index]){
        state_table[index] = &state_array[num_states];
    }else{
        GameState* prev = state_table[index];
        while(prev->next_in_list) prev = prev->next_in_list;
        prev->next_in_list = &state_array[num_states];
    }

    num_states++;
}

bool equals(GameState state1, GameState state2){
    return (state1.players_pos[0] == state2.players_pos[0]) &&
    (state1.players_pos[1] == state2.players_pos[1]) &&
    (state1.players_score[0] == state2.players_score[0]) &&
    (state1.players_score[1] == state2.players_score[1]) &&
    ((state1.round % 2) == (state2.round % 2));
}

GameState* get_state_with_wins(GameState state){
    GameState* array_state = state_table[get_state_index(state)];
    while(array_state && !equals(state, *array_state)){
        array_state = array_state->next_in_list;
    }
    return array_state;
}

GameState sum_wins(GameState state1, GameState state2){
    state1.players_wins[0] += state2.players_wins[0];
    state1.players_wins[1] += state2.players_wins[1];
    return state1;
}

GameState simulate_universe(GameState state, int dice_number);

GameState simulate_all_universes(GameState state){
	for(int roll1 = 1; roll1 <= 3; roll1++){
		for (int roll2 = 1; roll2 <= 3; roll2++) {
			for (int roll3 = 1; roll3 <= 3; roll3++) {
				uint64 roll_sum = roll1 + roll2 + roll3;
				GameState result = state;
				result.players_wins[0] = 0;
				result.players_wins[1] = 0;
				state = sum_wins(state, simulate_universe(result, roll_sum));
			}
		}
	}
	return state;
}

GameState simulate_universe(GameState state, int dice){
    uint64 index = (state.round++) % 2;

    if(state.round == ULLONG_MAX){
        printf("Round reached %llu\n", state.round);
        exit(1);
    }

	state.players_pos[index] = ((state.players_pos[index] + dice - 1) % 10) + 1;
	
	state.players_score[index] += state.players_pos[index];
	
	if(state.players_score[index] >= WIN_SCORE){
		state.players_wins[index] = 1;
		state.players_wins[(index + 1) % 2] = 0;
		add_state(state);
		return state;
	}

    GameState* computed_state = get_state_with_wins(state);
    if(computed_state){
        return *computed_state;
    }

    GameState result = state;
    result.players_wins[0] = 0;
    result.players_wins[1] = 0;
    result = simulate_all_universes(result);
    add_state(result);

	
    return result;
}

int main(){
    GameState state = { 0 };
    state.players_pos[0] = 8;
    state.players_pos[1] = 10;
    printf("State \n");
    print_state(state);

    state_array = (GameState*) malloc(GAME_STATE_ARRAY_SIZE * sizeof(GameState));
    state_table = (GameState**) calloc(GAME_STATE_ARRAY_SIZE, sizeof(GameState*));

    state = simulate_all_universes(state);

    printf("\nResult state \n");
    print_state(state);
    return 0;
}