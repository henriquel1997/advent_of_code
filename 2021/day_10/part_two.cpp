#include "common.h"

int main(){
    String text = readFile("input.txt");
    TextFileInfo info = getTextFileInfo(text);
	
    char* position = text.data;

    uint64 stack_size = 0;
    char* stack = (char*) malloc(info.maxLineSize * sizeof(char));

    uint64 score_array_size = 0;
    uint64* score_array = (uint64*) calloc(info.numberOfLines, sizeof(uint64));

    uint64 line = 1;
    while((position - text.data) < text.size){
        char cur_char = *(position++);

        if(cur_char == '\n'){
            if(stack_size != 0){
                //Incomplete
                uint64 score = 0;
                for(uint64 i = stack_size; i > 0; i--){
                    char c = stack[i - 1];
                    score *= 5;
                    if(c == ')'){
                        score += 1;
                    }else if(c == ']'){
                        score += 2;
                    }else if(c == '}'){
                        score += 3;
                    }else if(c == '>'){
                        score += 4;
                    }else{
                        printf("Not a valid closing char on stack %c\n", cur_char);
                        return -1000;
                    }
                }

                printf("Line %llu - Score: %llu\n", line, score);

                uint64 index = 0;
                for(; index < score_array_size; index++){
                    if(score > score_array[index]) break;
                }

                for(uint64 i = score_array_size; i > index; i--){
                    score_array[i] = score_array[i - 1];
                }
                score_array[index] = score;
                score_array_size++;

                stack_size = 0;
                line++;
            }
            continue;    
        }

        if(cur_char == '('){
            stack[stack_size++] = ')';
        }else if(cur_char == '['){
            stack[stack_size++] = ']';
        }else if(cur_char == '{'){
            stack[stack_size++] = '}';
        }else if(cur_char == '<'){
            stack[stack_size++] = '>';
        }else{
            if(stack_size == 0 || stack[stack_size - 1] != cur_char){
                //Ignoring corrupted lines
                while(*position != '\n') position++;
                position++;
                stack_size = 0;
                printf("Line %llu - Corrupted\n", line++);
                continue;
            }
            stack_size--;
        }
    }

    printf("\nScore array:\n");
    for(uint64 i = 0; i < score_array_size; i++){
        printf("%llu - Score: %llu\n", i, score_array[i]);
    }
    printf("Autocomplete score: %llu\n", score_array[score_array_size/2]);
}