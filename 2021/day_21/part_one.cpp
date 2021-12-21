#include "common.h"

uint64 roll_dice(uint64* dice_number){
    uint64 result = (*dice_number)++;
    if(*dice_number > 100) *dice_number -= 100;
    return result;
}

int main(){
    uint64 players_pos[2] = {8, 10};
    uint64 players_score[2] = { 0 };
    uint64 num_dice_rolls = 0;

    uint64 dice_number = 1;

    for(int player_index = 0; players_score[0] < 1000 && players_score[1] < 1000; player_index = (player_index + 1) % 2){
        uint64 roll1 = roll_dice(&dice_number);
        uint64 roll2 = roll_dice(&dice_number);
        uint64 roll3 = roll_dice(&dice_number);
        uint64 total_dice = roll1 + roll2 + roll3;

        players_pos[player_index] = ((players_pos[player_index] + total_dice - 1) % 10) + 1;
        players_score[player_index] += players_pos[player_index];
        num_dice_rolls += 3;
        
        printf("Player %i rolls %llu+%llu+%llu and moves to space %llu for a total score of %llu.\n", player_index + 1, roll1, roll2, roll3, players_pos[player_index], players_score[player_index]);
    }

    uint64 loser_index = (players_score[0] < 1000) ? 0 : 1;
    printf("Loser: %llu\n", loser_index + 1);
    printf("Loser score: %llu\n", players_score[loser_index]);
    printf("Num dice rolls: %llu\n", num_dice_rolls);
    printf("Loser score * Num dice rolls = %llu\n", players_score[loser_index] * num_dice_rolls);
    return 0;
}