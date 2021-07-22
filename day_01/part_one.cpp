#include "..\common.h"

int main(){
    String text = readFile("input.txt");

    int num_values = 0;
    long values [10000];

    char* position = text.data;
    while(true){
        long value = strtol(position, &position, 10);
        if(value){
            values[num_values++] = value;
            printf("%ld\n", values[num_values - 1]);
        }else{
            break;
        }
    }

    for(int i = 0; i < num_values; i++){
        long value = values[i];
        for(int j = 0; j < num_values; j++){
            if(i != j){
                long other = values[j];
                if(value + other == 2020){
                    printf("Part one answer:\n");
                    printf("%ld + %ld = 2020\n", value, other);
                    printf("%ld * %ld = %ld\n", value, other, value * other);
                    return 0;
                }
            }
        }
    }

    printf("No values sum to 2020\n");
    return -1;
}