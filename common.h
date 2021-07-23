#include <stdio.h>
#include <stdlib.h>
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
    return {fsize, string};
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

#define uint64 unsigned long long

#define array_size(array) (sizeof(array)/sizeof(array[0]))