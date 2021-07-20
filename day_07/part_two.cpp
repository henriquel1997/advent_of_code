#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct String {
    long size;
    char* data;
};

String readFile(const char* fileName){
    FILE *f = fopen(fileName, "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* string = (char*) malloc(fsize + 1);
    fread(string, 1, fsize, f);
    fclose(f);

    string[fsize] = 0;
    return (String){fsize, string};
}

bool strings_equal(char* str1, char* str2, int length){
    for(int i = 0; i < length; i++){
        if(str1[i] != str2[i]){
            return false;
        }
    }
    return true;
}

bool strings_equal(char* str1, const char* str2){
    return strings_equal(str1, (char*) str2, strlen(str2));
}

bool strings_equal(String str1, const char* str2){
    long constLength = strlen(str2);
    if(str1.size != constLength){
        return false;
    }
    return strings_equal(str1.data, (char*)str2, constLength);
}

bool strings_equal(String str1, String str2){
    if(str1.size != str2.size){
        return false;
    }
    return strings_equal(str1.data, str2.data, str1.size);
}

bool isDigit(char c){
    return c >= '0' && c <= '9';
}

bool isLetter(char c){
    return c >= 'a' && c <= 'z';
}

struct BagChildren {
    int index;
    long quantity;
    BagChildren* next;
};

struct Bag {
    String name;
    BagChildren* children;
};

#define MAX_BAGS 10000
int numBags = 0;
Bag bags [MAX_BAGS] = {0};

#define MAX_CHILDREN 100000
int numChildrens = 0;
BagChildren childrens [MAX_CHILDREN] = {0};

int createBag(String name, BagChildren* children = 0){
    int index = -1;
    for(int i = 0; i < numBags; i++){
        if(strings_equal(bags[i].name, name)){
            index = i;
            break;
        }
    }

    if(index < 0){
        index = numBags;
        bags[numBags++] = (Bag){name, children};
    }else{
        if(children){
            bags[index].children = children;
        }
    }

    return index;
}

int parseBag(char** position){
    while(!isLetter(**position)) (*position)++;

    char* bagName = *position;
    while(!strings_equal(*position, " bags")) (*position)++;
    long nameLength = (*position) - bagName;
    
    String name = {nameLength, bagName};

    BagChildren* previousChildren = 0;
    while((**position) != '.'){
        bool noBag = false;
        while(!isDigit(**position)){
            if(strings_equal(*position, "no other bag")){
                noBag = true;
                break;
            }
            (*position)++;
        }

        if(noBag){
            while((**position) != '.') (*position)++;
            break;
        }

        long quantity = strtol(*position, position, 10);

        while(!isLetter(**position)) (*position)++;

        char* childrenNamePointer = *position;
        while(!strings_equal(*position, " bag")) (*position)++;
        long childrenNameLength = (*position) - childrenNamePointer;

        String childrenName = {childrenNameLength, childrenNamePointer};

        int index = createBag(childrenName);

        childrens[numChildrens] = (BagChildren){index, quantity, previousChildren};
        previousChildren = &childrens[numChildrens++];

        while((**position) != ',' && (**position) != '.') (*position)++;
    }

    (*position)++;
    (*position)++;

    return createBag(name, previousChildren);
}

void printBag(int index){
    Bag bag = bags[index];
    printf("Bag name: %.*s\n", (int)bag.name.size, bag.name.data);

    BagChildren* children = bag.children;
    if(!children){
        printf("\t- No children\n");
        return;
    }
    while(children){
        Bag child = bags[children->index];
        printf("\t- %ld %.*s\n", children->quantity, (int)child.name.size, child.name.data);
        children = children->next;
    }
}

long getNumberOfBagsInside(Bag bag){
    long bagCount = 0;
    BagChildren* children = bag.children;
    while(children){
        bagCount += children->quantity;
        bagCount += children->quantity * getNumberOfBagsInside(bags[children->index]);
        children = children->next;
    }
    return bagCount;
}

int main(){
    String text = readFile("input.txt");

    char* position = text.data;

    int shinyGoldIndex = -1;
    while(*position != '\0'){
        int index = parseBag(&position);
        if(strings_equal(bags[index].name, "shiny gold")){
            shinyGoldIndex = index;
        }
        printBag(index);
    }

    
    if(shinyGoldIndex >= 0){
        printf("Number of bags contained inside a shiny gold bag: %ld\n", getNumberOfBagsInside(bags[shinyGoldIndex]));
    }else{
        printf("Shiny gold bag not in the input\n");
    }

    return 0;
}