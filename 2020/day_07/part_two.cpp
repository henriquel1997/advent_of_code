#include "..\common.h"

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
        bags[numBags++] = {name, children};
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

        childrens[numChildrens] = {index, quantity, previousChildren};
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