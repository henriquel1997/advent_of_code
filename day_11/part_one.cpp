#include "..\common.h"

struct TextFileInfo{
    uint64 numberOfLines;
    uint64 maxLineSize;
};

TextFileInfo getTextFileInfo(String text){
    uint64 numberOfLines = 0;
    uint64 maxLineSize = 0;
    char* lineStart = text.data;
    for(int i = 0; i < text.size; i++){
        if(text.data[i] == '\n'){
            numberOfLines++;
            char* lineEnd = &text.data[i];
            uint64 lineSize = (uint64)lineEnd - (uint64)lineStart;
            if(lineSize > maxLineSize){
                maxLineSize = lineSize;
            }
            lineStart = lineEnd + 1;
        }
    }
    return {numberOfLines, maxLineSize};
}

enum ChairState {
    CHAIR_STATE_FLOOR,
    CHAIR_STATE_EMPTY,
    CHAIR_STATE_OCCUPIED
};

void updateState(ChairState* currentState, ChairState* newState, uint64 lineSize, uint64 numberOfLines){
    for(int y = 0; y < numberOfLines; y++){
        for(int x = 0; x < lineSize; x++){
            uint64 pos = (y * lineSize) + x;
            ChairState currentChair = currentState[pos];
            ChairState newChair = currentChair;
            if(currentChair == CHAIR_STATE_EMPTY){
                for(int otherY = y - 1; otherY <= y + 1; otherY++){
                    if(otherY >= 0 && otherY < numberOfLines){
                        for(int otherX = x - 1; otherX <= x + 1; otherX++){
                            if(otherX >= 0 && otherX < lineSize && (otherX != x || otherY != y)){
                                if(currentState[(otherY * lineSize) + otherX] == CHAIR_STATE_OCCUPIED){
                                    goto endLoop;
                                }
                            }
                        }
                    }
                }
                newChair = CHAIR_STATE_OCCUPIED;
            }else if(currentChair == CHAIR_STATE_OCCUPIED){
                int occupiedCount = 0;
                for(int otherY = y - 1; otherY <= y + 1; otherY++){
                    if(otherY >= 0 && otherY < numberOfLines){
                        for(int otherX = x - 1; otherX <= x + 1; otherX++){
                            if(otherX >= 0 && otherX < lineSize && (otherX != x || otherY != y)){
                                if(currentState[(otherY * lineSize) + otherX] == CHAIR_STATE_OCCUPIED){
                                    occupiedCount++;
                                    if(occupiedCount >= 4){
                                        newChair = CHAIR_STATE_EMPTY;
                                        goto endLoop;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            endLoop: newState[pos] = newChair;
        }
    }
}

bool areStatesEqual(ChairState* state1, ChairState* state2, uint64 stateSize){
    for(uint64 i = 0; i < stateSize; i++){
        if(state1[i] != state2[i]){
            return false;
        }
    }
    return false;
}

void printState(ChairState* state, uint64 lineSize, uint64 numberOfLines){
    uint64 occupiedSeats = 0;
    for(uint64 i = 0; i < (lineSize * numberOfLines); i++){
        switch(state[i]){
            case CHAIR_STATE_OCCUPIED:{
                printf("#");
                occupiedSeats++;
                break;
            }
            case CHAIR_STATE_EMPTY:{
                printf("L");
                break;
            }
            default:
            case CHAIR_STATE_FLOOR:{
                printf(".");
                break;
            }
        }

        if((i + 1) % lineSize == 0){
            printf("\n");
        }
    }
    printf("Number of occupied seats: %llu\n", occupiedSeats);
}

int main(){
    String text = readFile("input.txt");
    TextFileInfo info = getTextFileInfo(text);

    uint64 bufferSize = 0;
    uint64 numBytesBuffer = info.maxLineSize * info.numberOfLines * sizeof(ChairState);
    ChairState* buffer = (ChairState*)malloc(numBytesBuffer);
    
    for(int i = 0; i < text.size; i++){
        if(text.data[i] == 'L'){
            buffer[bufferSize++] = CHAIR_STATE_EMPTY;
        }else if(text.data[i] == '.'){
            buffer[bufferSize++] = CHAIR_STATE_FLOOR;
        }
    }

    ChairState* secondBuffer = (ChairState*)malloc(numBytesBuffer);
    
    do{
        memcpy(secondBuffer, buffer, numBytesBuffer);
        updateState(buffer, secondBuffer, info.maxLineSize, info.numberOfLines);
        ChairState* aux = buffer;
        buffer = secondBuffer;
        secondBuffer = aux;
    }while(memcmp(buffer, secondBuffer, numBytesBuffer));

    printState(buffer, info.maxLineSize, info.numberOfLines);
    return 0;
}