#include "..\common.h"

#define MAX_INGREDIENTS_PER_FOOD 100
#define MAX_ALLERGENS_PER_FOOD 30
struct Food {
	uint64 numIngredients;
	uint64 ingredients[MAX_INGREDIENTS_PER_FOOD];
	uint64 numAllergens;
	uint64 allergens[MAX_ALLERGENS_PER_FOOD];
};

#define MAX_INGREDIENTS 2500
#define MAX_ALLERGENS 500
#define MAX_FOODS 50

int main(){
	String text = readFile("input.txt");

	uint64 totalIngredients = 0;
	String ingredients[MAX_INGREDIENTS];
	uint64 totalAllergens = 0; ;
	String allergens[MAX_ALLERGENS];

	uint64 totalFoods = 0;
	Food foods[MAX_FOODS];
	
	char* position = text.data;
	const char* allergenBegin = "(contains ";
	while((position - text.data) < text.size){
		if(totalFoods >= MAX_FOODS){
			printf("Food array is too small\n");
			return -1;
		}

		Food food = { 0 };

		//For some reason this is way slower than the parsers in the other days, 
		//but it still works, maybe it would be interesting to profile this.
		while(!strings_equal(position, allergenBegin) && *position != '\n'){
			if(totalIngredients >= MAX_INGREDIENTS){
				printf("Ingredient array is too small\n");
				return -2;
			}

			char* ingredientStart = position;
			uint64 ingredientIndex = MAX_INGREDIENTS;
			for(uint64 i = 0; i < totalIngredients; i++){
				if(strings_equal_not_const(ingredients[i], ingredientStart)){
					ingredientIndex = i;
					break;
				}
			}

			if(ingredientIndex == MAX_INGREDIENTS){
				if(food.numIngredients >= MAX_INGREDIENTS_PER_FOOD){
					printf("Food ingredient array is too small\n");
					return -3;
				}

				while(*position != ' ') position++;
				String string = {(long)(position - ingredientStart), ingredientStart};

				ingredientIndex = totalIngredients;
				ingredients[totalIngredients++] = string;
			}
			position++;

			food.ingredients[food.numIngredients++] = ingredientIndex;
		}

		if(strings_equal(position, allergenBegin)){
			position += strlen(allergenBegin);
			while(*position != '\n'){
				if(totalAllergens >= MAX_ALLERGENS){
					printf("Allergen array is too small\n");
					return -4;
				}

				char* allergenStart = position;
				uint64 allergenIndex = MAX_ALLERGENS;
				for(uint64 i = 0; i < totalAllergens; i++){
					if(strings_equal_not_const(allergens[i], allergenStart)){
						allergenIndex = i;
						break;
					}
				}
				
				if(allergenIndex == MAX_ALLERGENS){
					if(food.numAllergens >= MAX_ALLERGENS_PER_FOOD){
						printf("Food allergen array is too small\n");
						return -5;
					}

					while(*position != ',' && *position != ')') position++;
					String string = {(long)(position - allergenStart), allergenStart};
					
					allergenIndex = totalAllergens;
					allergens[totalAllergens++] = string;
				} else {
					position += allergens[allergenIndex].size;
				}
				while(*position == ',' || *position == ' ' || *position == ')')
					position++;
				
				food.allergens[food.numAllergens++] = allergenIndex;
			}
		}
		position++;

		foods[totalFoods++] = food;
	}

	//Printing the foods
	for(uint64 i = 0; i < totalFoods; i++){
		printf("%llu: ", i + 1);
		Food food = foods[i];
		for (uint64 j = 0; j < food.numIngredients; j++) {
			uint64 ingredientId = food.ingredients[j];
			String ingredient = ingredients[ingredientId];
			printf("%.*s(%llu) ", (int)ingredient.size, ingredient.data, ingredientId);
		}
		if(food.numAllergens > 0){
			printf("(contains ");
			for (uint64 j = 0; j < food.numAllergens; j++) {
				uint64 allergenId = food.allergens[j];
				String allergen = allergens[allergenId];
				if (j == 0){
					printf("%.*s(%llu)", (int)allergen.size, allergen.data, allergenId);
				} else {
					printf(", %.*s(%llu)", (int)allergen.size, allergen.data, allergenId);
				}
			}
			printf(")");
		}
		printf("\n");
	}

	//Searching for ingredient-allergen matches
	uint64 allergensFound = 0;
	int64 ingredientForAllergenId[MAX_INGREDIENTS];
	for(uint64 i = 0; i < MAX_INGREDIENTS; i++) ingredientForAllergenId[i] = -1;

	while(allergensFound < totalAllergens){
		uint64 initialAllergensFound = allergensFound;
		for (uint64 i = 0; i < totalFoods; i++){
			Food food = foods[i];
			uint64 numAllergensNotFound = 0;
			uint64 allergenId;
			for (uint64 j = 0; j < food.numAllergens; j++) {
				uint64 id = food.allergens[j];
				if(ingredientForAllergenId[food.allergens[j]] < 0){
					numAllergensNotFound++;
					allergenId = id;
				}
			}

			if(numAllergensNotFound == 1){

				uint64 numFoodsWithSameAllergen = 0;
				uint64 timesEachIngredientAppeared [MAX_INGREDIENTS] = { 0 };

				for (uint64 f = 0; f < totalFoods; f++){
					if(i != f){
						Food other = foods[f];
						bool containsSameAllergen = false;
						for (uint64 j = 0; j < other.numAllergens; j++) {
							if(food.allergens[j] == allergenId){
								containsSameAllergen = true;
								break;
							}
						}

						if(containsSameAllergen){
							numFoodsWithSameAllergen++;

							for (uint64 k = 0; k < food.numIngredients; k++) {
								uint64 foodIng = food.ingredients[k];
								for (uint64 n = 0; n < other.numIngredients; n++) {
									uint64 otherIng = other.ingredients[n];
									if(foodIng == otherIng){
										timesEachIngredientAppeared[foodIng]++;
										break;
									}
								}
							}
						}
					}
				}

				if(numFoodsWithSameAllergen > 0){
					for (uint64 j = 0; j < totalIngredients; j++){
						if(timesEachIngredientAppeared[j] == numFoodsWithSameAllergen){
							allergensFound++;
							ingredientForAllergenId[allergenId] = j;
							break;
						}
					}
				}
			}
		}

		if(allergensFound <= initialAllergensFound){
			printf("No new allergens found stopping the search\n");
			break;
		}
	}

	printf("Finished\n");
	return 0;
}