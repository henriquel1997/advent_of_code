#include "raylib.h"
#include "common.h"

struct AluState{
	bool x;
	int64 w;
	int64 z;
	int64 z_percent;
};

#define NUM_DIGITS 14

int64 array0[NUM_DIGITS] = { 12, 12, 12, -9, -9, 14, 14, -10, 15, -2, 11, -15, -9, -3 };
int64 array1[NUM_DIGITS] = {  1,  1,  1, 26, 26,  1,  1,  26,  1, 26,  1,  26, 26, 26 };
int64 array2[NUM_DIGITS] = {  9,  4,  2,  5,  1,  6, 11,  15,  7, 12, 15,   9, 12, 12 };

AluState run_program(uint64 input, int iteration) {
	uint64 reverse = 0;
	while (input){
		reverse *= 10;
		reverse += input % 10;
		input /= 10;
	}

	if (iteration >= NUM_DIGITS) iteration = NUM_DIGITS - 1;
	
	AluState state = { 0 };
	
	for (int i = 0; i <= iteration; i++) {
		
		state.w = reverse % 10;
		reverse /= 10;

		state.z_percent = state.z % 26;
		state.x = (state.z_percent + array0[i]) != state.w;
		state.z /= array1[i];
		if (state.x){
			state.z *= 26;
			state.z += state.w + array2[i];
		}
	}
	
	return state;
}

int main(void){
	// Initialization
	//--------------------------------------------------------------------------------------
	const float screenWidth = 800;
	const float screenHeight = 450;
	
	InitWindow((int)screenWidth, (int)screenHeight, "ALU Calculator");
	
	SetTargetFPS(60);

	int current_digit = 0;
	char digits [14] = { 0 };
	float last_back_space_time = 0;
	//--------------------------------------------------------------------------------------
	
	// Main loop
	while (!WindowShouldClose()) { // Detect window close button or ESC key
		// Update
		//----------------------------------------------------------------------------------
		float time = GetTime();
		if (IsKeyDown(KEY_BACKSPACE) && (time - last_back_space_time) >= 0.125f){
			last_back_space_time = time;
			current_digit = max(0, current_digit - 1);
		} else if(current_digit < NUM_DIGITS) {
			char c = GetCharPressed();
			if (isDigit(c) && c != '0'){
				digits[current_digit++] = c - '0';
			}
		}

		uint64 input = 0;
		char input_text[NUM_DIGITS + 1];
		for (int i = 0; i < current_digit; i++){
			input *= 10;
			input += digits[i];
			input_text[i] = digits[i] + '0';
		}
		input_text[current_digit] = '\0';
		
		//----------------------------------------------------------------------------------
		
		// Draw
		//----------------------------------------------------------------------------------

		float font_size = 20;

		BeginDrawing();
		
		ClearBackground(RAYWHITE);

		bool x = false;

		AluState state = { 0 };
		char state_text[1000];
		for (int i = 0; i < current_digit; i++) {
			state = run_program(input, i);
			sprintf(state_text, "(%i) w = %lli / z = %lli // array0: %lli / array1: %lli / array2: %lli / X: %s", i, state.w, state.z, array0[i], array1[i], array2[i], state.x ? "true" : "false");
			DrawText(state_text, 0, i * font_size * 1.25f, font_size, BLUE);
		}

		font_size = 40;
		DrawText(input_text, 0, screenHeight - font_size, font_size, BLACK);

		float right_padding = 250;
		sprintf(state_text, "X: %s", state.x ? "true" : "false");
		DrawText(state_text, screenWidth - right_padding, screenHeight - font_size, font_size, RED);
		
		sprintf(state_text, "z %% 26 = %lli", state.z_percent);
		DrawText(state_text, screenWidth - right_padding, screenHeight - (2 * font_size * 1.25f), font_size, RED);

		EndDrawing();
		//----------------------------------------------------------------------------------
	}
	
	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------
	
	return 0;
}