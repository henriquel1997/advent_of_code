#include "common.h"

int main(){
    String text = readFile("input.txt");
    TextFileInfo info = getTextFileInfo(text);
	
    char* position = text.data;

    uint64 stack_size = 0;
    char* stack = (char*) malloc(info.maxLineSize * sizeof(char));

    uint64 error_score = 0;
    while((position - text.data) < text.size){
        char cur_char = *(position++);

        if(cur_char == '\n'){
            stack_size = 0;
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
                //Corrupted
                if(cur_char == ')'){
                    error_score += 3;
                }else if(cur_char == ']'){
                    error_score += 57;
                }else if(cur_char == '}'){
                    error_score += 1197;
                }else if(cur_char == '>'){
                    error_score += 25137;
                }else{
                    printf("Not a valid closing char %c\n", cur_char);
                    return -1000;
                }

                while(*position != '\n') position++;
                continue;
            }
            stack_size--;
        }
    }

    printf("Syntax error score: %llu\n", error_score);
}