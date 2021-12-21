#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define int64 long long
#define uint64 unsigned long long

#define array_size(array) (sizeof(array)/sizeof(array[0]))

#define abs(x) ((x) >= 0 ? (x) : -(x))
#define sign(x) ((x) >= 0 ? (1) : -1)
#define max(x, y) ((x) > (y) ? (x) : (y))
#define min(x, y) ((x) < (y) ? (x) : (y))

#ifndef LLONG_MIN
    #define LLONG_MIN -9223372036854775807
#endif
#ifndef LLONG_MAX
    #define LLONG_MAX 9223372036854775807
#endif
#ifndef ULLONG_MAX
    #define ULLONG_MAX 18446744073709551615
#endif

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

String stringFromConstChar(const char* text){
	return (String){(long)strlen(text), (char*)text};
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

bool strings_equal_not_const(String str1, char* str2){
	return strings_equal(str1.data, (char*)str2, str1.size);
}

bool strings_equal(String str1, String str2){
    if(str1.size != str2.size){
        return false;
    }
    return strings_equal(str1.data, str2.data, str1.size);
}

bool startsWith(String str1, const char* str2){
	long constLength = strlen(str2);
	if(str1.size < constLength){
		return false;
	}
	return strings_equal(str1.data, (char*)str2, constLength);
}

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
    if(text.data[text.size - 1] != '\n') numberOfLines++;
	return (TextFileInfo){numberOfLines, maxLineSize};
}

bool isDigit(char c){
    return c >= '0' && c <= '9';
}

bool isLetter(char c){
    return c >= 'a' && c <= 'z';
}

bool isLetterUpperCase(char c){
    return c >= 'A' && c <= 'Z';
}

bool isWhiteSpace(char c){
	return c == ' ' || c == '\t';
}

void eatWhiteSpace(char** text){
	while (isWhiteSpace(**text)) (*text)++;
}

bool isEndOfLine(char c){
    return c == '\r' || c == '\n';
}

uint64 getLineSize(char* position){
    char* lineStart = position;
    while(!isEndOfLine(*position)) position++;
    return (uint64)position - (uint64)lineStart;
}