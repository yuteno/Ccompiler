#include<stdio.h>
#include<stdlib.h>
#include <ctype.h>
#include <string.h>


enum {
	TK_NUM = 256,
	TK_EOF,
};


typedef struct {
	int ty;
	int val;
	char *input;
} Token;

//extern Token tokens[100];

void tokenize(char *p);
void tokenize_vec(char *p);