#include "..\common.h"

#define NUMBERS_IN_LIST 101

int main(){
    String text = readFile("input.txt");

    char* position = text.data;
    long numbers[NUMBERS_IN_LIST];
    int index = 0;
    
    //Insertion sort
    while(*(position + 1) != '\0' && index < NUMBERS_IN_LIST && ((position - text.data) < text.size)){
        long value = strtol(position, &position, 10);

        int pos = index;
        for(int i = 0; i < index; i++){
            if(value < numbers[i]){
                pos = i;
                break;
            }
        }

        for(int i = index; i > pos; i--){
            numbers[i] = numbers[i - 1];
        }

        numbers[pos] = value;
        index++;
    }

    int differencesOf1 = 1; //Starts with 1 because there will always be a difference of 1 from the outlet to the first adapter
    int differencesOf3 = 1; //Starts with 1 because there will always be a difference of 3 from the last number to the device
    for(int i = 0; i < index - 1; i++){
        long difference = numbers[i + 1] - numbers[i];
        if(difference == 1) differencesOf1++;
        else if(difference == 3) differencesOf3++;
        printf("%i - %ld (%ld)\n", i + 1, numbers[i], difference);
    }
    printf("%i - %ld (%i)\n", index, numbers[index - 1], 3);

    printf("Differences of 1: %i\n", differencesOf1);
    printf("Differences of 3: %i\n", differencesOf3);
    printf("%i * %i = %i\n", differencesOf1, differencesOf3, differencesOf1 * differencesOf3);
    return 0;
}