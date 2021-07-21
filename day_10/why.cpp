#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* textString = "1\n2\n3\n4\n7\n10\n11\n12\n13\n14\n17\n18\n19\n20\n21\n24\n25\n26\n27\n28\n31\n32\n33\n34\n35\n38\n39\n40\n43\n46\n47\n48\n49\n52\n53\n54\n55\n56\n59\n62\n63\n64\n65\n66\n69\n72\n73\n76\n77\n78\n81\n82\n83\n84\n87\n88\n89\n90\n93\n94\n95\n96\n99\n102\n103\n104\n105\n108\n109\n110\n111\n114\n117\n120\n121\n122\n123\n126\n127\n128\n129\n132\n135\n138\n139\n140\n141\n142\n145\n146\n147\n148\n151\n152\n153\n154\n155\n158\n159\n160\n161\n";

struct String {
    long size;
    char* data;
};

#define uint64 unsigned long long
#define NUMBERS_IN_LIST 101

uint64 numberOfPaths[NUMBERS_IN_LIST] = {0};

uint64 getNumberOfPathsForIndex(long* array, int size, int index){
    if(index >= size - 1){
        return 1;
    }

    uint64 totalNumberOfPaths = 0;
    long value = array[index];

    for(int i = 1; i <= 3; i++){
        int id = index + i;
        if(id >= size) break;

        long difference = array[id] - value;
        if(difference <= 3){
            if(numberOfPaths[id]){
                totalNumberOfPaths += numberOfPaths[id];
            }else{
                uint64 paths = getNumberOfPathsForIndex(array, size, id);
                numberOfPaths[id] = paths;
                totalNumberOfPaths += paths;
            }
        }else{
            break;
        }
    }
    return totalNumberOfPaths;
}

int main(){
    String text = {(long)strlen(textString), (char*)textString};

    char* position = text.data;
    long numbers[NUMBERS_IN_LIST + 2];
    int size = 0;

    numbers[size++] = 0;
    
    //Insertion sort
    while(*(position + 1) != '\0' && size < (NUMBERS_IN_LIST + 2) && ((position - text.data) < text.size)){
        long value = strtol(position, &position, 10);

        int pos = size;
        for(int i = 0; i < size; i++){
            if(value < numbers[i]){
                pos = i;
                break;
            }
        }

        for(int i = size; i > pos; i--){
            numbers[i] = numbers[i - 1];
        }

        numbers[pos] = value;
        size++;
    }

    numbers[size] = numbers[size - 1] + 3;
    size++;

    uint64 answer = getNumberOfPathsForIndex(&numbers[0], size, 0);
    numberOfPaths[0] = answer;

    printf("Total number of combinations: %llu\n", answer);
    return 0;
}