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

    long sumTotal = 2020;
    for(int i = 0; i < num_values; i++){
        long first = values[i];
        for(int j = 0; j < num_values; j++){
            if((i != j) && (i + j < sumTotal)){
                long second = values[j];
                for(int k = 0; k < num_values; k++){
                    if(i != k){
                        long third = values[k];

                        if(first + second + third == sumTotal){
                            printf("Part two answer:\n");
                            printf("%ld + %ld + %ld = %ld\n", first, second, third, sumTotal);
                            printf("%ld * %ld * %ld = %ld\n", first, second, third, first * second * third);
                            return 0;
                        }
                    }
                }
            }
        }
    }

    printf("No values sum to 2020\n");
    return -1;
}